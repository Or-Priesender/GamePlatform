/*
 * CLI.cpp
 *
 *  Created on: Aug 31, 2017
 *      Author: user
 */

#include "CLI.h"

CLI::CLI() {
	this->game = NULL;
}

CLI::~CLI() {
}

void CLI::run() {
	std::string first, second;
	std::string username;
	bool loginRes = false;
	bool uiRes = true;
	int command = 1, read = 0;

	//client is not logged in
	while (!loginRes) {
		if (command != 0)
			this->showLoginMenu();
		//get command from user
		command = pollCommand();
		if(command == 3)//exit the program
			break;
		if (command != 0) {
			loginRes = game->handleLogin(command);
		}
		//client is logged in
		while (loginRes) {
			if (game->isBusy()) {
				command = 1;
				continue;
			}
			if (command != 0)
				this->showUIMenu();
			//get command from user
			command = pollCommand();
			if(command == 5){//request disconnect
				game->disconnectFromServer();
				loginRes = false;
				break;
			}
			if (command != 0)
				game->handleInterface(command);
		}
	}

}

void CLI::showUIMenu() {
	cout << "---------------------------------" << endl;
	cout << "|" << "*** " << "MENU" << "\t\t\t" << "|" << endl;
	cout << "|" << "*** " << "1.List Connected Users" << "\t" << "|" << endl;
	cout << "|" << "*** " << "2.Show Score Board" << "\t\t" << "|" << endl;
	cout << "|" << "*** " << "3.Start Game With User" << "\t" << "|" << endl;
	cout << "|" << "*** " << "4.Start Game Auto" << "\t\t" << "|" << endl;
	cout << "|" << "*** " << "5.Disconnect " << "\t\t" << "|" << endl;
	cout << "----------------------------------" << endl;
}

void CLI::showLoginMenu() {
	cout << "---------------------------------" << endl;
	cout << "|" << "*** " << "MENU" << "\t\t\t" << "|" << endl;
	cout << "|" << "*** " << "1.Login" << "\t\t\t" << "|" << endl;
	cout << "|" << "*** " << "2.Register" << "\t\t\t" << "|" << endl;
	cout << "|" << "*** " << "3.Exit" << "\t\t\t" << "|" << endl;
	cout << "---------------------------------" << endl;
}

int CLI::pollCommand() {
	int command = 0;
	std::string temp;
	fd_set fds;
	struct timeval t;
	t.tv_sec = 1;
	t.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	//select with 1 second timeout
	select(STDIN_FILENO+1, &fds, NULL, NULL, &t);
	if (FD_ISSET(0, &fds)) {
		cin >> command;
		//if user inserted something other then an integer
		while (std::cin.fail()) {
			std::cout << "Insert a valid number" << std::endl;
			std::cin.clear();
			std::cin.ignore(256, '\n');
			std::cin >> command;
		}
	}
	return command;
}


