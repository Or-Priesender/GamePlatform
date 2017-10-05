/*
 * Authenticator.cpp
 *
 *  Created on: Aug 13, 2017
 *      Author: user
 */

#include "Authenticator.h"
#define MAX_INT  2147483647

Authenticator::Authenticator(UserHandler* users) {
	this->users = users;
	userCounter = 0;
	this->initLists();

}

Authenticator::~Authenticator() {
	//doesnt delete server !
}

//registers a user to the system
bool Authenticator::registerUser(std::string username, std::string password,TCPSocket* clientSock) {

	std::ofstream out("user.txt",std::ofstream::out | std::ofstream::app);

	//if user exists you are not allowed to register with that username
	if(username.empty() || password.empty() || this->userExists(username)){
		clientSock->write(REGISTER_FAILED);
		return false;
	}

	char buffer[100];
	//encrypt the password
	int encrypted = encrypt(password);
	//write the details to the file
	out << username <<":"<<encrypted<<endl;
	out.close();
	return performHandshake(username,clientSock);
}

//login existing user to the server
bool Authenticator::loginUser(std::string username, std::string password,TCPSocket* clientSock) {
	std::ifstream in("user.txt",std::ifstream::in);
	std::string res;
	char buffer[100];

	//encrypt the password
	int encrypted = encrypt(password);

	sprintf(buffer,"%s:%d",username.c_str(),encrypted);
	//check username and password according to file
	while(std::getline(in,res)){
		if(strlen(buffer) == res.size()){
			if(strcmp(buffer,res.c_str())==0){
				in.close();
				return performHandshake(username,clientSock);
			}
		}

	}
	clientSock->write(LOGIN_FAILED);
	in.close();
	return false;
}

//move the user to a new port and connect with him
bool Authenticator::performHandshake(std::string username,TCPSocket* client) {
	userCounter++;
	int newport = SDK_PORT + userCounter;
	client->write(LOGIN_APPROVED);

	//open a server socket of this port
	TCPSocket* newSock = new TCPSocket(newport);

	//tell the client to move to a new port
	client->write(MOVE_TO_NEW_PORT);
	client->write(newport);

	//accepts his connection
	TCPSocket* ready = newSock->listenAndAccept();
	if(ready != NULL){
		//add to connected users list
		users->addUser(new GameUser(username,ready));
		return true;
	}
	newSock->close();
	return false;
}

//encrypt a string with a subtitution cypher (every letter is character to a different character)
int Authenticator::encrypt(std::string pass) {
	const char* thePass = pass.c_str();

	char newPass[100] = {0};


	for(unsigned int i=0;i<strlen(thePass);i++){

		for(unsigned int j=0;j<regular.size();j++){
			//the character is found in the normal vector
			if(thePass[i] == regular[j]){
				//subtitute to the cypher character
				newPass[i] = cypher[j];
			}
		}
	}
	newPass[strlen(thePass)] = '\0';
	int passNum =0;
	for(unsigned int i=0;i<strlen(newPass);i++){
		//check if the encryption goes over the max int
		if(passNum > MAX_INT/10 - 126)
			passNum %= 2147483;
		passNum+=newPass[i];
		passNum*=10;
	}
	return passNum;

}

//checks if a user exists
bool Authenticator::userExists(std::string username) {
	std::ifstream in("user.txt",std::ifstream::in);
		std::string line;
		while(std::getline(in,line)){
			std::size_t pos = line.find(username);
			if(pos != std::string::npos){
				in.close();
				return true;
			}
		}
		in.close();
		return false;
}

//initialize the cypher lists
void Authenticator::initLists() {


	for(unsigned int i=33;i<=126;i++){
		regular.push_back(i);
	}

	for(unsigned int i=126;i>=33;i--){
		cypher.push_back(i);
	}

}
