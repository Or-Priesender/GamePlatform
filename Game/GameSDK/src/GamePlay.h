/*
 * GamePlay.h
 *
 *  Created on: Aug 10, 2017
 *      Author: user
 */

#ifndef SRC_GAMEPLAY_H_
#define SRC_GAMEPLAY_H_

#include "UDPSocket.h"
#include "TCPSocket.h"

using namespace std;
using namespace Lab;

//this class is in charge of the gameplay time functions
class GamePlay {
	//socket to talk to the other user
	UDPSocket* gameSock;
	//socket to talk to the server
	TCPSocket* mySock;
	//game role
	int role;
public:
	GamePlay(std::string ip, int remotePort,int role,TCPSocket* mySock);
	virtual ~GamePlay();

	int sendMsg(char* msg, int size);
	int recvMsg(char* msg);//call back
	void endGame(std::string playerOne,int playerOneScore,std::string playerTwo,int playerTwoScore);
	int getGameRole(){return role;}
};

#endif /* SRC_GAMEPLAY_H_ */
