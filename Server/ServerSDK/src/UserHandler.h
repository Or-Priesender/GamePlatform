/*
 * UserHandler.h
 *
 *  Created on: Aug 14, 2017
 *      Author: user
 */

#ifndef USERHANDLER_H_
#define USERHANDLER_H_
#include <vector>
#include <map>
#include "MultipleSocketListener.h"
#include "GameUser.h"
#include "MThread.h"
#include "SDKProtocol.h"
#include <time.h>
#include "GameSession.h"
#include <pthread.h>
#include "Guard.h"
#include "ScoreBoardManager.h"
#include <sstream>
using namespace Lab;
using namespace std;

//The main user handler, for all the user needs
class UserHandler : public MThread{

	vector<GameUser*> users;
	vector<GameSession*> sessions;
	int numOfUsers,numOfSessions;
	MultipleSocketListener* listener;
	bool stop;
	GameUser *awaitsAutoGame;
	//lock
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	ScoreBoardManager sbManager;

public:
	UserHandler();
	virtual ~UserHandler();

	bool startGame(std::string peerName,TCPSocket* clientSock);
	bool autoStartGame(TCPSocket* clientSock);

	void addUser(GameUser* user);

	vector<GameUser*> listUsers();
	vector<std::string> scoreBoard();
	void replyWithUserList(TCPSocket* user);
	void replyWithScoreList(TCPSocket* user);

	GameUser* findUserBySocket(TCPSocket* sock);
	GameUser* findUserByName(std::string username);

	void run();
	void stopListening();
	void startListening();
	int inviteToGame(std::string peerName,TCPSocket* clientSock);
	bool startGameWithUser(std::string peerName,TCPSocket* clientSock);
	void cancelAutoGame(TCPSocket* clientSock);

	void endGame(TCPSocket* userSock);
	void endGameSlave(TCPSocket* userSock);

	void disconnectUser(TCPSocket* userSock);
};

#endif /* USERHANDLER_H_ */
