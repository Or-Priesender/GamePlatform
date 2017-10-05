/*
 * ScoreBoardManager.h
 *
 *  Created on: Aug 23, 2017
 *      Author: user
 */

#ifndef SCOREBOARDMANAGER_H_
#define SCOREBOARDMANAGER_H_

#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>

using namespace std;

//Handles score board listing in a file
class ScoreBoardManager {
	int extractScore(std::string scoreLine);
public:
	//check what should be private
	ScoreBoardManager();
	virtual ~ScoreBoardManager();
	bool addScoreToUser(std::string username,int addedValue);
	bool reduceScoreFromUser(std::string username, int reducedValue);
	int findUserInBoard(std::string username);
	void addNewUserScore(std::string username, int firstScore);
	vector<std::string> buildVectorFromBoard();

};

#endif /* SCOREBOARDMANAGER_H_ */
