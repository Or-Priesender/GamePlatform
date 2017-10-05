/*
 * SDKProtocol.h
 *
 *  Created on: Aug 10, 2017
 *      Author: user
 */

#ifndef SRC_SDKPROTOCOL_H_
#define SRC_SDKPROTOCOL_H_



/**
 * SDK Platform protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
#define SDK_PORT 3346
//#define GAME_STARTING_PORT 4035

//#define REGISTER 					1
//#define REGISTER_FAILED				11
//#define LOGIN 						2
//#define LOGIN_APPROVED 				22
//#define LOGIN_FAILED 				33
//#define LIST_USERS 					3
//#define LIST_USERS_REPLY			4
//#define LIST_SCORES					5
//#define LIST_SCORES_REPLY			6
//#define START_GAME_WITH_USER		7
//#define START_GAME_AUTO				8
//#define WAITING_FOR_PLAYER			44
//#define CANCEL_AUTO_REQ				88
//#define GAME_ACCEPTED				9
//#define GAME_REFUSED				10
//#define NO_SUCH_USER				11
//#define END_GAME					12 //
//#define END_GAME_SLAVE				122
//#define MOVE_TO_NEW_PORT			13 // -> port
//#define GAME_INVITATION				66  // -> username size -> username
//#define DISCONNECT 					15
//#define MASTER_USER					98
//#define SLAVE_USER					99
//#define SERVER_IP 				"127.0.0.1"

#define REGISTER 					1
#define REGISTER_FAILED				2
#define LOGIN 						3
#define LOGIN_APPROVED 				4
#define LOGIN_FAILED 				5
#define LIST_USERS 					6
#define LIST_USERS_REPLY			7
#define LIST_SCORES					8
#define LIST_SCORES_REPLY			9
#define START_GAME_WITH_USER		10
#define START_GAME_AUTO				11
#define WAITING_FOR_PLAYER			12
#define CANCEL_AUTO_REQ				13
#define GAME_ACCEPTED				14
#define GAME_REFUSED				15
#define NO_SUCH_USER				16
#define END_GAME					17
#define END_GAME_SLAVE				18
#define MOVE_TO_NEW_PORT			19
#define GAME_INVITATION				20
#define DISCONNECT 					21
#define MASTER_USER					98
#define SLAVE_USER					99
#define SERVER_IP 				"127.0.0.1"





#endif /* SRC_SDKPROTOCOL_H_ */
