/*
 * GamePlay.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: user
 */

#include "GamePlay.h"
#include "SDKProtocol.h"


GamePlay::GamePlay(std::string ip, int remotePort,int role,TCPSocket* mySock) {

	//listen to the port connected to the server
	this->gameSock = new UDPSocket(mySock->getRemotePort());

	//connect to the other user
	this->gameSock->connect(ip, remotePort);
	this->mySock = mySock;
	this->role = role;
}

GamePlay::~GamePlay() {
	gameSock->close();
	delete gameSock;
}

int GamePlay::sendMsg(char* msg, int size) {
	gameSock->write(size);
	int result = gameSock->write(msg,size);
	return result;
}

int GamePlay::recvMsg(char* msg) {
	int msgSize;
	gameSock->read(&msgSize);
	int result = gameSock->read(msg, msgSize);
	msg[msgSize] = '\0';
	return result;
}

//tells the server the game is ended and posts the scores
void GamePlay::endGame(std::string playerOne,int playerOneScore,std::string playerTwo,int playerTwoScore) {

	if(this->role == MASTER_USER)
	{
		char buffer[100];
		//create the score string
		sprintf(buffer,"%s:%d:%s:%d",playerOne.c_str(),playerOneScore,playerTwo.c_str(),playerTwoScore);
		int size = strlen(buffer);
		mySock->write(END_GAME);
		mySock->write(size);
		mySock->write(buffer,size);

	} else mySock->write(END_GAME_SLAVE);

	this->gameSock->close();
}
