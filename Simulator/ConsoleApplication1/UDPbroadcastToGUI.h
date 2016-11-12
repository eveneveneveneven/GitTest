
/*	
Det ser ut til å være nyttig å kunne analysere kontrollerens oppførsel under testing.
Denne modulen åpner for enkel UDP broadcasting av viktige kontrollerdata slik at en
PC på land kan analysere i real-time. 
*/


#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")

#define GPS_UDP_PORT		31036
#define CONTROLLER_UDP_PORT 32123

struct broadcastSocket {
	SOCKET sock;
	struct sockaddr_in Sender_addr;
	struct sockaddr_in Recv_addr;
};

broadcastSocket UDPbroadCastSocketInit(int port);
void UDPbroadcastString(broadcastSocket bcSocket, const char *data);

// Broadcasts some important controller data for analyzing on shore.
// GUI on shore will receive a string on the following (example) form:
// $LEFT_MOTOR,99$RIGHT_MOTOR,-99$HEADING_REF,359$SPEED_REF,5.5
void UDPbroadcastControlParams(broadcastSocket bcSocket, float leftInput, float rightInput, float headingRef, float speedRefKmh, double heading, double currentSpeed, double currentDir);