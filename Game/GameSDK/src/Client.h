/*
 * Client.h
 *
 *  Created on: Aug 20, 2017
 *      Author: user
 */

#ifndef SRC_CLIENT_H_
#define SRC_CLIENT_H_

#include "Login.h"
#include "UserInterface.h"
#include "GameUser.h"
#include "GameSession.h"
#include "SDKProtocol.h"
#include "GamePlay.h"
#include "InvitationHandler.h"

using namespace std;
using namespace Lab;

//Client facade class
class Client {
	GameUser* user;
	GamePlay* gamePlay;
	UserInterface* ui;

	//Abstract class to handle invitations
	InvitationHandler* handler;

	Login* logger;
	bool loggedIn;

public:
	Client(InvitationHandler* handler);
	virtual ~Client();

	//both initialize the user data member and instanciate the user interface
	bool loginUser(std::string username,std::string password);
	bool registerUser(std::string username,std::string password);

	//phase 2 functions
	vector<std::string> listUsers();
	vector<std::string> scoreBoard();
	bool startGameWithUser(std::string username);
	bool autoStartGame();
	void cancelAutoGame(){if(ui!=NULL) ui->cancelAutoGame();}

	//phase 3 functions
	int sendMsg(char* msg, int size);
	int recvMsg(char* msg);//call back
	void endGame(std::string playerOne,int playerOneScore,std::string playerTwo,int playerTwoScore);

	std::string getUsername(){if(user != NULL) return user->getUsername(); else return "";}
	bool isInGame(){if(ui == NULL) return false; else return ui->isInGame();}
	int getGameRole(){if(ui != NULL)return ui->getGameRole(); else return 0;}
	std::string getLastOpponent(){if(ui!=NULL)return ui->getLastOpponent(); else return "";}
	void disconnect(){if(ui != NULL) ui->disconnect();}

};

#endif /* SRC_CLIENT_H_ */
