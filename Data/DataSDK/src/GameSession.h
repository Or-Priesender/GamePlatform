/*
 * GameSession.h
 *
 *  Created on: Aug 14, 2017
 *      Author: user
 */

#ifndef GAMESESSION_H_
#define GAMESESSION_H_
#include "GameUser.h"

using namespace Lab;

//Container class for players in session
class GameSession {

	GameUser* playerOne;
	GameUser* playerTwo;


public:

	GameSession(GameUser* one,GameUser* two);
	virtual ~GameSession();
	GameUser* getPlayerOne(){return playerOne;}
	GameUser* getPlayerTwo(){return playerTwo;}
};

#endif /* GAMESESSION_H_ */
