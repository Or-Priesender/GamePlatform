/*
 * Client.cpp
 *
 *  Created on: Aug 20, 2017
 *      Author: user
 */

#include "Client.h"

Client::Client(InvitationHandler* handler) {
	this->logger = new Login();
	this->ui = NULL;
	this->handler = handler;
	this->user = NULL;
	this->loggedIn = false;
	this->gamePlay = NULL;

}

Client::~Client() {
	delete logger;
	delete ui;
	delete user;
}

//Login to the server and create game user and user interface
bool Client::loginUser(std::string username, std::string password) {
	user = logger->registerOrLoginUser(username, password, "login");
	if(user == NULL)
		return false;
	else {
		ui = new UserInterface(user,handler);
		this->loggedIn = true;

		//start listening to invitations
		ui->startListening();
		return true;
	}
}

//Register to the server and create game user and user interface
bool Client::registerUser(std::string username, std::string password) {
	user = logger->registerOrLoginUser(username, password, "register");
	if(user == NULL)
		return false;
	else {
		ui = new UserInterface(user,handler);
		this->loggedIn = true;

		//start listening to invitations
		ui->startListening();
		return true;
	}
}

//request the connected users list from the server
vector<std::string> Client::listUsers() {
	vector<std::string> v;
	if(ui != NULL && loggedIn)
		return ui->listUsers();
	else return v;
}

//request the score board from the server
vector<std::string> Client::scoreBoard() {
	vector<std::string> v;
	if(ui != NULL && loggedIn)
		return ui->getScoreBoard();
	else return v;
}

//invite a user to the game
bool Client::startGameWithUser(std::string username) {
	if(ui != NULL && loggedIn){
		bool res =  ui->startGame(username);
		return res;
	}
	else return NULL;
}

//start game with a random user
bool Client::autoStartGame() {
	if(ui != NULL && loggedIn){
	 bool res = ui->autoStartGame();
		return res;
	}
	else return NULL;
}

//send a message while in a game
int Client::sendMsg(char* msg, int size) {
	if(this->ui != NULL)
		return this->ui->sendMsg(msg, size);
	else return 0;
}

//receive a message while in a game
int Client::recvMsg(char* msg) {
	if(this->ui != NULL){
		return this->ui->recvMsg(msg);
	}
	else return 0;
}

//end the game
void Client::endGame(std::string playerOne, int playerOneScore,	std::string playerTwo, int playerTwoScore) {
	if(this->ui != NULL){
		this->ui->endGame(playerOne, playerOneScore, playerTwo, playerTwoScore);
	}
}
