/*
 * UserInterface.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: user
 */

#include "UserInterface.h"


UserInterface::UserInterface(GameUser* user,InvitationHandler* invHandler) {
	this->user = user;
	this->gamePlay = NULL;
	this->invHandler = invHandler;
	this->stop = true;
	this->waitingForAutoGame = false;
	this->inGame = false;
	listener.addSocket(user->getSocket());
}

UserInterface::~UserInterface() {
	this->stopListening();
	//Client class deletes user
	delete gamePlay;
}
//request and construct the connected users list from the server
vector<std::string> UserInterface::listUsers() {
	this->stopListening();
	TCPSocket* sock = user->getSocket();
	int command,listSize,usernameSize;
	char buffer[100];

	vector<std::string> users;

	//request the list
	sock->write(LIST_USERS);
	sock->read(&command);
	if(command == LIST_USERS_REPLY){
		//get the list size
		sock->read(&listSize);
		for(int i=0;i<listSize;i++){
			//get each user and construct the users vector
			sock->read(&usernameSize);
			sock->read(buffer,usernameSize);
			std::string username(buffer,usernameSize);
			users.push_back(username);
		}
	}
	this->startListening();
	return users;
}

//request and construct the score board
vector<std::string> UserInterface::getScoreBoard() {
	this->stopListening();
	TCPSocket* sock = user->getSocket();
	int command,listSize,scoreSize;
	char buffer[100];

	vector<std::string> scores;
	//request the score board
	sock->write(LIST_SCORES);
	sock->read(&command);
	if(command == LIST_SCORES_REPLY){
		//get the list size
		sock->read(&listSize);
		for(int i=0;i<listSize;i++){
			//get each score line and construct the scores vector
			sock->read(&scoreSize);
			sock->read(buffer,scoreSize);
			std::string score(buffer,scoreSize);
			scores.push_back(score);
		}
	}
	this->startListening();
	return scores;
}

//start game with a specific user
bool UserInterface::startGame(std::string username) {
	this->stopListening();
	TCPSocket* sock = user->getSocket();
	int result;

	//request to play with the user
	sock->write(START_GAME_WITH_USER);
	sock->write(username.size());
	sock->write(username.c_str(),username.size());
	//get the reply from the server
	sock->read(&result);
	if(result == GAME_ACCEPTED){
		lastOpponent.assign(username);
		startGamePlay(sock);
		return true;
	}
	else{
		this->startListening();
		return false;
	}
}

//start game with a random user
bool UserInterface::autoStartGame() {
	this->stopListening();
	TCPSocket* sock = user->getSocket();
	int result;

	//request to auto start game
	sock->write(START_GAME_AUTO);
	//get the reply
	sock->read(&result);
	if(result == GAME_ACCEPTED){
		startGamePlay(sock);
		return true;
	} else if(result == WAITING_FOR_PLAYER){//there is no user available for auto play
		waitingForAutoGame = true;
		this->startListening();
		return false;
	}
	else{
		this->startListening();
		return false;
	}

}

//after the game was accepted, get the connection data from the server and create the gameplay.
void UserInterface::startGamePlay(TCPSocket* userSock) {
	this->stopListening();
	waitingForAutoGame = false;
	int size =0 ,port=0,role=0;
	char buffer[100] = {0};
	//get the connection and game data
	userSock->read(&role);
	userSock->read(&size);
	//get the ip and port
	userSock->read(buffer,size);
	char* ipString = strtok(buffer,":");
	char* remotePortString = strtok(NULL,":");

	if(remotePortString != NULL && ipString != NULL){
		//turn the port string to int
		std::stringstream ss(remotePortString);
		ss >> port;
		inGame=true;
		this->gamePlay = new GamePlay(string(ipString),port,role,userSock);
	}
}

//listening to game invitations
void UserInterface::run() {
	std::string line;
	TCPSocket* ready;
	TCPSocket* userSock = user->getSocket();
	int command,size;
	char buffer[100];
	bool res = false;
	while(!stop && (ready = listener.listenToSocket()) != NULL) {
		command = 0;
		ready->read(&command);
		if(command == 0) break; //server socket was closed
		if(command == GAME_INVITATION){
			//read the username
			ready->read(&size);
			ready->read(buffer,size);
			//if we are waiting for auto game
			if(waitingForAutoGame){
				invHandler->acceptInvitation(ready);
				lastOpponent.assign(buffer,size);
				startGamePlay(userSock);
			}else{
				res = invHandler->handleInvitation(buffer, ready);
				if(res){
					lastOpponent.assign(buffer,size);
					startGamePlay(userSock);
					invHandler->activateAfterGameAccepted();
				}

			}
		}
	}

}

void UserInterface::stopListening() {
	if(!stop){
		stop = true;
		listener.interrupt();
	}
}

int UserInterface::sendMsg(char* msg, int size) {
	if(this->gamePlay != NULL)
		return this->gamePlay->sendMsg(msg, size);
	else return 0;
}

int UserInterface::recvMsg(char* msg) {
	if(this->gamePlay != NULL){
		return this->gamePlay->recvMsg(msg);
	}
	else return 0;
}

void UserInterface::endGame(std::string playerOne, int playerOneScore,	std::string playerTwo, int playerTwoScore) {
	if(this->gamePlay != NULL){
		this->gamePlay->endGame(playerOne, playerOneScore, playerTwo, playerTwoScore);
		this->inGame = false;
		this->gamePlay = NULL;
	}
	this->startListening();
}

//tell the server we no longer want to wait for auto game
void UserInterface::cancelAutoGame() {
	if(user!=NULL){
	TCPSocket* sock = user->getSocket();
	sock->write(CANCEL_AUTO_REQ);
	}
}

void UserInterface::startListening() {
	if(stop){
		stop = false;
		this->start();
	}
}
