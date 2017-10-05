/*
 * ScoreBoardManager.cpp
 *
 *  Created on: Aug 23, 2017
 *      Author: user
 */

#include "ScoreBoardManager.h"

ScoreBoardManager::ScoreBoardManager() {
	// TODO Auto-generated constructor stub

}

ScoreBoardManager::~ScoreBoardManager() {
	// TODO Auto-generated destructor stub
}

//adds the added value to a user in the file, if he doesnt exist, creates it
bool ScoreBoardManager::addScoreToUser(std::string username, int addedValue) {
	int pos = findUserInBoard(username);
	std::string line;
	if (pos < 0) {
		addNewUserScore(username, addedValue);
		return false;
	} else {
		std::ifstream in("scores.txt", std::ifstream::in);
		std::ofstream out("temp.txt", std::ofstream::out);

		while (getline(in, line)) {
			//if this is the username's line
			int userpos = line.find(username);
			if (userpos != std::string::npos) {
				//extract his score and add the value
				int oldScore = extractScore(line);
				int newScore = oldScore + addedValue;
				//write the new value to the new output file
				out << username << ":" << newScore << endl;
			} else
				out << line << endl;
		}
		//overwrite the scores file
		rename("temp.txt", "scores.txt");
		return true;

	}

}

//same as addScoreFromUser() but for reduce
bool ScoreBoardManager::reduceScoreFromUser(std::string username,
		int reducedValue) {

	int pos = findUserInBoard(username);
	std::string line;
	if (pos < 0) {
		addNewUserScore(username, ((-1) * reducedValue));
		return false;
	} else {
		std::ifstream in("scores.txt", std::ifstream::in);
		std::ofstream out("temp.txt", std::ofstream::out);
		//get to the desired location

		while (getline(in, line)) {
			int userpos = line.find(username);
			if (userpos != std::string::npos) {
				int oldScore = extractScore(line);
				int newScore = oldScore - reducedValue;
				out << username << ":" << newScore << endl;
			} else
				out << line << endl;
		}

		rename("temp.txt", "scores.txt");
		return true;

	}
}

//check which line is the user
int ScoreBoardManager::findUserInBoard(std::string username) {
	std::ifstream in("scores.txt", std::ifstream::in);
	std::string line;
	int counter = 0;
	while (std::getline(in, line)) {
		counter++;
		std::size_t pos = line.find(username);
		if (pos != std::string::npos) {
			in.close();
			return counter;
		}
	}
	in.close();
	return -1;
}

//get the score from the score string
int ScoreBoardManager::extractScore(std::string scoreLine) {
	int score;
	int pos = scoreLine.find(":");
	pos++;
	std::string scoreStr = scoreLine.substr(pos, scoreLine.size() - pos);
	//change the score string to an integer
	std::stringstream ss(scoreStr);
	ss >> score;
	return score;

}

//add a new score to the board
void ScoreBoardManager::addNewUserScore(std::string username, int firstScore) {
	std::ofstream out("scores.txt", std::ofstream::out | std::ofstream::app);
	out << username << ":" << firstScore << endl;
}

//build a string vector from the file
vector<std::string> ScoreBoardManager::buildVectorFromBoard() {
	std::ifstream in("scores.txt", std::ifstream::in);
	std::string line;
	vector<std::string> result;

	while (getline(in, line)) {
		result.push_back(line);
	}

	return result;
}
