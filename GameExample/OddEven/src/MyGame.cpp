/*
 * MyGame.cpp
 *
 *  Created on: Aug 31, 2017
 *      Author: user
 */

#include "MyGame.h"

MyGame::MyGame(Client* client) {
	this->client = client;
	this->busy = false;
	this->dummy = new UDPSocket(0);
	this->dummyPort = dummy->getLocalPort();
	this->waiting = false;
}

MyGame::~MyGame() {
	delete client;
}

//handles login command from the user
bool MyGame::handleLogin(int command) {
	std::string username, password;
	bool res;
	cout << "Enter username and password" << endl;
	cin.clear();
	cin >> username >> password;
	if (command == 1) {//login
		res = client->loginUser(username, password);
		return res;
	} else if (command == 2) {//register
		res = client->registerUser(username, password);
		return res;
	} else {
		cout << "UNKNOWN COMMAND" << endl;
		return false;
	}
}


void MyGame::handleInterface(int command) {
	std::string username, password;
	vector<std::string> strings;
	bool res = false;
	switch (command) {
	case 1://request users list
		strings = client->listUsers();
		cout <<endl<< "Users list : " << endl << endl;
		this->printVector(strings);
		strings.clear();
		break;
	case 2://request score board
		strings = client->scoreBoard();
		cout <<endl<< "Score board : " << endl << endl;
		this->printVector(strings);
		strings.clear();
		break;
	case 3://start game with user
		cout << "Please enter the username of the requested peer" << endl;
		fflush(stdin);
		cin >> username;
		this->startGameWithUser(username);
		break;
	case 4://start auto game
		this->startGameAuto();
		break;
	default:
		break;
	}
}

//handles the game with the role given from the server
void MyGame::handleGame() {
	this->busy = true;
	sleep(2);
	if (client->getGameRole() == MASTER_USER) {
		this->playAsMaster();
	} else if (client->getGameRole() == SLAVE_USER) {
		this->playAsSlave();
	} else {
		cout << "Game was not defined properly, try again ..  " << endl;
	}
}

//prints a vector
void MyGame::printVector(vector<std::string> v) {
	cout << "------------------" << endl;
	for (unsigned int i = 0; i < v.size(); i++){
		cout << "\t"<<v[i] << endl;
		cout << "------------------" << endl;
	}
	cout << endl << endl;
}


bool MyGame::startGameWithUser(std::string username) {
	int res = client->startGameWithUser(username);
	if (res) {
		cout << "Game Accepted ! starting .. " << endl;
		this->handleGame();
		return true;
	} else {
		cout << "Game Refused Or User Doesn't Exist" << endl;
		return false;
	}
}

//master user controls the game and the scores
void MyGame::playAsMaster() {
	sleep(1);
	char buffer[100] = { 0 };
	int number, opNumber;
	int opPick;
	int p1_score = 0, p2_score = 0;
	std::string opponent;
	int size = client->getUsername().size();

	//construct my username
	char* username = new char[size+1];
	strcpy(username,client->getUsername().c_str());
	username[size] = '\0';

	//send the username to the peer, and receive his username
	client->sendMsg(username, strlen(username));
	client->recvMsg(buffer);
	opponent.assign(buffer);
	cout << "You are now playing with " << opponent << endl;

	memset(buffer,0,100);
	//tell the users who is choosing even or odd
	cout << "Opponent is choosing even or odd" << endl;
	client->sendMsg("Insert a number to choose even or odd",
			strlen("Insert a number to choose even or odd"));

	//get the peers choice
	client->recvMsg((char*) &opPick);

	//if the peers choice is even
	if (opPick % 2 == 0) { //opponent is even

		//tell the users what he chose
		cout << "Opponent chose even !" << endl;
		client->sendMsg("You chose even !", strlen("You chose even !"));

		for (int i = 0; i < 3; i++) {
			//tell the users which round is it
			memset(buffer, 0, 100);
			sprintf(buffer, "ROUND %d/3! insert a number !", i + 1);
			client->sendMsg(buffer, strlen(buffer));
			cout << buffer << endl;

			//get the numbers from the users
			number = this->getIntFromUser();
			client->recvMsg((char*) &opNumber);

			//tell the users which numbers they chose
			memset(buffer,0,100);
			sprintf(buffer,"Numbers are: %d:%d",number,opNumber);
			cout << buffer << endl;
			client->sendMsg(buffer, strlen(buffer));

			//check who won
			if (((number + opNumber) % 2) == 0) { //opponent won - even
				p2_score++;
				cout << "Opponent won!" << endl;
				client->sendMsg("You won!", strlen("You won!"));
			} else { //we won - odd
				p1_score++;
				cout << "You won!" << endl;
				client->sendMsg("Opponent won!", strlen("Opponent won!"));
			}
		}
	} else { //opponent is odd
		cout << "Opponent chose odd !" << endl;
		client->sendMsg("You chose odd !", strlen("You chose odd !"));

		for (int i = 0; i < 3; i++) {
			memset(buffer, 0, 100);
			sprintf(buffer, "ROUND %d/3! insert a number !", i + 1);
			cout << buffer << endl;
			client->sendMsg(buffer, strlen(buffer));
			number = this->getIntFromUser();
			client->recvMsg((char*) &opNumber);

			memset(buffer,0,100);
			sprintf(buffer,"Numbers are: %d:%d",number,opNumber);
			cout << buffer << endl;
			client->sendMsg(buffer, strlen(buffer));

			if (((number + opNumber) % 2) != 0) { //opponent won - odd
				p2_score++;
				cout << "Opponent won!" << endl;
				client->sendMsg("You won!", strlen("You won!"));
			} else { //we won - even
				p1_score++;
				cout << "You won!" << endl;
				client->sendMsg("Opponent won!", strlen("Opponent won!"));
			}
		}
	}
	if (p1_score > p2_score) {
		cout << "YOU ARE THE WINNER !" << endl;
		client->sendMsg("YOU LOST ! ", strlen("YOU LOST ! "));
		client->endGame(client->getUsername(), 1, opponent, 0);
		this->busy = false;
	} else {
		cout << "YOU LOST ! " << endl;
		client->sendMsg("YOU ARE THE WINNER !", strlen("YOU ARE THE WINNER !"));
		client->endGame(client->getUsername(), 0, opponent, 1);
		this->busy = false;
	}

}

void MyGame::playAsSlave() {
	sleep(1);
	char buffer[200];
	int pick, number;
	std::string opponent;

	//construct my username
	int size = client->getUsername().size();
	char* username = new char[size+1];
	strcpy(username,client->getUsername().c_str());
	username[size] = '\0';

	//receive and send the usernames
	client->recvMsg(buffer);
	opponent.assign(buffer);
	client->sendMsg(username, strlen(username));
	memset(buffer,0,100);
	cout << "You are now playing with " << opponent << endl;

	//receive start message
	client->recvMsg(buffer);
	cout << buffer << endl;

	//choose even or odd by inserting a number
	pick = this->getIntFromUser();
	client->sendMsg((char*) &pick, 4);

	//get what you chose message
	client->recvMsg(buffer);
	cout << buffer << endl;
	for (int i = 0; i < 3; i++) {

		//get starting round message
		client->recvMsg(buffer);
		cout << buffer << endl;
		number = this->getIntFromUser();
		client->sendMsg((char*) &number, 4);

		//which numnbers were chosen
		client->recvMsg(buffer);
		cout << buffer << endl;

		//get who won message
		client->recvMsg(buffer);
		cout << buffer << endl;
	}
	client->recvMsg(buffer);
	cout << buffer << endl;
	//slave only writes END_GAME_SLAVE to server, and does not send the scores
	client->endGame(opponent, 0, client->getUsername(), 0);
	this->busy = false;
}

//start an auto game
bool MyGame::startGameAuto() {
	std::string line;
	char c = '\0';
	int res = client->autoStartGame();
	if (res) {
		cout << "Game is starting !" << endl;
		this->handleGame();
		return true;
	} else {//wait for another user
		cout << "Waiting for another user .. Press any key to cancel" << endl;
		waiting = true;
		c = this->getCharOrBlockUntilInterupted();//returns '/0' if is interrupted
		if (c != '\0') { //cancel auto game
			client->cancelAutoGame();
			cout << "Auto game aborted" << endl;
			waiting = false;
			return true;
		}else{ //start a game
			waiting = false;
			cout << "User is found ! starting .. " << endl;
			this->handleGame();
			return true;
		}
	}

}

//handle an invitation
bool MyGame::handleInvitation(std::string username) {
	std::string answer;
	this->busy = true;
	cout << "Game invitation accepted from " << username
			<< "! do you wish to accept?" << endl;
	cin >> answer;
	this->busy = false;
	if (answer.compare("yes") == 0)
		return true;
	else
		return false;

}


char MyGame::getCharOrBlockUntilInterupted() {
	char command = '\0';
	std::string temp;
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	FD_SET(dummy->getDescriptor(), &fds);
	//select with no timeout
	select(dummy->getDescriptor() + 1, &fds, NULL, NULL, NULL);
	if (FD_ISSET(0, &fds)) {
		cin >> command;
		while (std::cin.fail()) {
			std::cout << "Please insert a valid character" << std::endl;
			std::cin.clear();
			std::cin.ignore(256, '\n');
			std::cin >> command;
		}
	} else if (FD_ISSET(dummy->getDescriptor(), &fds)) {
		cout << "WAITING WAS INTERRUPTED" << endl;
		int num;
		dummy->read(&num);
	}
	return command;
}

//interrupt waiting for auto game
void MyGame::interrupt() {
	if(waiting){
		UDPSocket sock(0);
		sock.connect("127.0.0.1", dummyPort);
		sock.write(1);
	}
}

//get an integer from the user with error management
int MyGame::getIntFromUser() {
	int command;
	cin >> command;
	while (std::cin.fail()) {
		std::cout << "INSERT A NUMBER INSTEAD" << std::endl;
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cin >> command;
	}
	return command;
}
