/*
 * UserHandler.cpp
 *
 *  Created on: Aug 14, 2017
 *      Author: user
 */

#include "UserHandler.h"

UserHandler::UserHandler() {
	listener = new MultipleSocketListener();
	stop = true;
	numOfUsers = 0;
	numOfSessions = 0;
	awaitsAutoGame = NULL;
}

UserHandler::~UserHandler() {
	for(unsigned int i=0;i<users.size();i++){
		users[i]->getSocket()->close();
	}
	delete listener;
	delete awaitsAutoGame;
}


//give players the needed details to make a connection with eachother
bool UserHandler::startGame(std::string peerName, TCPSocket* clientSock) {
	char toClient[100];
	char toPeer[100];
	GameUser* client = findUserBySocket(clientSock);
	GameUser* peer = findUserByName(peerName);
	TCPSocket* peerSock = peer->getSocket();

	clientSock->write(GAME_ACCEPTED);

	//construct the details strings
	sprintf(toClient, "%s:%d", peerSock->getLocalIP().c_str(),
			peerSock->getRemotePort());
	sprintf(toPeer, "%s:%d", clientSock->getLocalIP().c_str(),
			clientSock->getRemotePort());

	//write details to client
	clientSock->write(MASTER_USER);
	clientSock->write(strlen(toClient));
	clientSock->write(toClient, strlen(toClient));

	//write details to peer
	peerSock->write(SLAVE_USER);
	peerSock->write(strlen(toPeer));
	peerSock->write(toPeer, strlen(toPeer));

	//set both users in session
	client->setInSession(true);
	peer->setInSession(true);

	//create a new session
	GameSession* newSession = new GameSession(client, peer);
	sessions.push_back(newSession);

	cout << client->getUsername() << " and " << peer->getUsername() << " are starting a game" << endl;
	this->startListening();
	return true;
}

//start game with random user
bool UserHandler::autoStartGame(TCPSocket* clientSock) {
	this->stopListening();
	//if there is no user that waits for an auto game tell the client to wait
	if (awaitsAutoGame == NULL) {
		awaitsAutoGame = findUserBySocket(clientSock);
		clientSock->write(WAITING_FOR_PLAYER);
		this->startListening();
		return false;
	} else { //if there is a user that waits for an auto game, start a game with him

		this->startGameWithUser(awaitsAutoGame->getUsername(), clientSock);
		return true;
	}

}

//start a game with a specific user
bool UserHandler::startGameWithUser(std::string peerName,TCPSocket* clientSock) {
	this->stopListening();

	//sends the user the game invitation and gets his reply
	int answer = inviteToGame(peerName, clientSock);

	//if the peer replied with game accepted
	if (answer == GAME_ACCEPTED) {
		bool res = this->startGame(peerName, clientSock);
		return res;
	} else {//game declined
		clientSock->write(GAME_REFUSED);
		this->startListening();
		return false;
	}
}

//invite a user to the game
int UserHandler::inviteToGame(std::string peerName, TCPSocket* clientSock) {
	//find the user
	GameUser* peer = findUserByName(peerName);
	if (peer == NULL) {//user wasnt found
		clientSock->write(NO_SUCH_USER);
		this->startListening();
		return -1;
	} else if (peer->isInSession()) {//user is in session
		clientSock->write(GAME_REFUSED);
		this->startListening();
		return -1;
	}

	TCPSocket* peerSock = peer->getSocket();
	GameUser* client = findUserBySocket(clientSock);

	int answer;
	//tell the peer that the client wants to play with him
	peerSock->write(GAME_INVITATION);
	peerSock->write(client->getUsername().size());
	peerSock->write(client->getUsername().c_str(),client->getUsername().size());
	sleep(1);
	peerSock->read(&answer);
	return answer;
}

//return the users list
vector<GameUser*> UserHandler::listUsers() {
	return users;
}

//return the scoreboard vector
vector<std::string> UserHandler::scoreBoard() {
	return sbManager.buildVectorFromBoard();
}

//listen to incoming user requests
void UserHandler::run() {
	int command = 0, size = 0;
	char buffer[100];
	vector<GameUser*> users;
	vector<std::string> scores;
	std::string username;
	TCPSocket* ready = NULL;

	//listen to all the client sockets
	while (!stop && (ready = listener->listenToSocket()) != NULL) {
		command = 0;
		ready->read(&command);
		//if user's socket was closed
		if(command == 0) this->disconnectUser(ready);
		switch (command) {
		case LIST_USERS:
			replyWithUserList(ready);
			break;
		case LIST_SCORES:
			replyWithScoreList(ready);
			break;
		case START_GAME_WITH_USER:
			//get the username to start the game with
			ready->read(&size);
			ready->read(buffer, size);
			username.assign(buffer,size);
			this->startGameWithUser(username, ready);
			break;
		case START_GAME_AUTO:
			this->autoStartGame(ready);
			break;
		case CANCEL_AUTO_REQ:
			this->cancelAutoGame(ready);
			break;
		case END_GAME:
			this->endGame(ready);
			break;
		case END_GAME_SLAVE:
			this->endGameSlave(ready);
			break;
		case DISCONNECT:
			this->disconnectUser(ready);
			break;
		default:
			break;
		}
	}

}

//add a user to the connected users list
void UserHandler::addUser(GameUser* user) {
	this->stopListening();
	numOfUsers++;
	listener->addSocket(user->getSocket());
	users.push_back(user);
	cout << user->getUsername() << " is connected" << endl;
	this->startListening();
}

//find a user by his socket descriptor
GameUser* UserHandler::findUserBySocket(TCPSocket* sock) {
	for (unsigned int i = 0; i < users.size(); i++) {
		if (users[i]->getSocket()->getDescriptor() == sock->getDescriptor()) {
			return users[i];
		}
	}
	return NULL;
}

//find user by username
GameUser* UserHandler::findUserByName(std::string username) {
	for (unsigned int i = 0; i < users.size(); i++) {
		if (users[i]->getUsername().compare(username) == 0) {
			return users[i];
		}
	}
	return NULL;
}

//stop listening to sockets
void UserHandler::stopListening() {
	if (!stop) {
		stop = true;
		if (listener != NULL)
			listener->interrupt();
	}

}

//remove user from waiting to an auto game
void UserHandler::cancelAutoGame(TCPSocket* clientSock) {
	GameUser* client = findUserBySocket(clientSock);
	if (strcmp(client->getUsername().c_str(),
			awaitsAutoGame->getUsername().c_str()) == 0) {
		awaitsAutoGame = NULL;
	}
}

//finish the game for the users and write their scores
void UserHandler::endGame(TCPSocket* userSock) {
	this->stopListening();
	int size = 0, playerOneScore = 0, playerTwoScore = 0;
	std::string playerOneStr;
	std::string playerTwoStr;
	char buffer[100];
	//read the end game details
	userSock->read(&size);
	userSock->read(buffer, size);

	//seperate the strings
	char *playerOne = strtok(buffer, ":");
	char *playerOneScoreStr = strtok(NULL, ":");
	char *playerTwo = strtok(NULL, ":");
	char *playerTwoScoreStr = strtok(NULL, ":");

	//add the score for player one
	if (playerOne != NULL && playerOneScoreStr != NULL) {
		playerOneStr.assign(playerOne, strlen(playerOne));
		stringstream s1(playerOneScoreStr);
		s1 >> playerOneScore;
		sbManager.addScoreToUser(playerOneStr, playerOneScore);
	}
	//add the score for player two
	if (playerTwo != NULL && playerTwoScoreStr != NULL) {
		playerTwoStr.assign(playerTwo, strlen(playerTwo));
		stringstream s2(playerTwoScoreStr);
		s2 >> playerTwoScore;
		sbManager.addScoreToUser(playerTwoStr, playerTwoScore);
	}

	//set the users availbility
	GameUser* c = findUserByName(playerOneStr);
	GameUser* p = findUserByName(playerTwoStr);
	c->setInSession(false);
	p->setInSession(false);
	this->startListening();
}

//reply to user's request for the connected users list
void UserHandler::replyWithUserList(TCPSocket* user) {
	vector<GameUser*> users = listUsers();
	user->write(LIST_USERS_REPLY);
	user->write(users.size());	//how many users

	for (unsigned int i = 0; i < users.size(); i++) {
		user->write(users[i]->getUsername().size());	//size of each username
		user->write(users[i]->getUsername().c_str(),
				users[i]->getUsername().size());	//the username itself
	}

}

//reply to user's request for the score board
void UserHandler::replyWithScoreList(TCPSocket* user) {
	vector<std::string> scores = scoreBoard();
	user->write(LIST_SCORES_REPLY);
	user->write(scores.size());	//how many scores

	for (unsigned int i = 0; i < scores.size(); i++) {
		user->write(scores[i].size());	//size of each score line
		user->write(scores[i].c_str(), scores[i].size());//the score line itself
	}
}

void UserHandler::endGameSlave(TCPSocket* userSock) {

	GameUser* slave = findUserBySocket(userSock);
	slave->setInSession(false);
}

//
void UserHandler::disconnectUser(TCPSocket* userSock) {
	this->stopListening();
	GameUser* user = findUserBySocket(userSock);
	cout << user->getUsername() << " is disconnecting" << endl;
	//delete the user from the users list
	if (user != NULL) {
		//go over the users list and find the user
		for (unsigned int i = 0; i < users.size(); i++) {
			if (users[i]->getUsername().compare(user->getUsername()) == 0) {
				listener->removeSocket(userSock);
				users.erase(users.begin() + i);
				userSock->close();
			}
		}

	}
	this->startListening();
}

//start listening to users
void UserHandler::startListening() {
	if (stop) {
		stop = false;
		this->start();
	}
}
