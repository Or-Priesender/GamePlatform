/*
 * UserInterface.h
 *
 *  Created on: Aug 10, 2017
 *      Author: user
 */

#ifndef SRC_USERINTERFACE_H_
#define SRC_USERINTERFACE_H_

#include <vector>
#include <stdlib.h>
#include "GameUser.h"
#include "GameSession.h"
#include "GamePlay.h"
#include "InvitationHandler.h"
#include <sstream>
#include "MThread.h"
#include "MultipleSocketListener.h"

using namespace std;
using namespace Lab;

//handles the connected user menu and listening to game invitations
class UserInterface : public MThread{
	GameUser* user;
	GamePlay* gamePlay;
	MultipleSocketListener listener;
	InvitationHandler* invHandler;
	bool stop,waitingForAutoGame,inGame;
	std::string lastOpponent;
public:
	UserInterface(GameUser* user,InvitationHandler* invHandler);
	virtual ~UserInterface();
	vector<std::string> listUsers();
	vector<std::string> getScoreBoard();
	bool startGame(std::string username);
	bool autoStartGame();
	void cancelAutoGame();
	void startGamePlay(TCPSocket* userSock);
	void run();
	void stopListening();
	void startListening();

	int sendMsg(char* msg, int size);
	int recvMsg(char* msg);//call back
	void endGame(std::string playerOne,int playerOneScore,std::string playerTwo,int playerTwoScore);
	void disconnect(){user->getSocket()->write(DISCONNECT);user->getSocket()->close();}
	bool isInGame(){return inGame;}
	int getGameRole(){if(gamePlay!=NULL) return gamePlay->getGameRole(); else return 0;}
	std::string getLastOpponent(){return lastOpponent;}
};

#endif /* SRC_USERINTERFACE_H_ */
