#include "UDPbroadcastToGUI.h"

using namespace std;


broadcastSocket UDPbroadCastSocketInit(int port) {
	broadcastSocket bcSocket;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	bcSocket.sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (bcSocket.sock == INVALID_SOCKET)
	{
		perror("socket creation");
	}

	BOOL enabled = TRUE;
	if (setsockopt(bcSocket.sock, SOL_SOCKET, SO_BROADCAST, (char*)&enabled, sizeof(BOOL)) < 0)
	{
		perror("broadcast options");
		closesocket(bcSocket.sock);
	}

	bcSocket.Sender_addr.sin_family = AF_INET;
	bcSocket.Sender_addr.sin_port = htons(port);
	bcSocket.Sender_addr.sin_addr.s_addr = inet_addr("Broadcast IP Here");

	bcSocket.Recv_addr.sin_family = AF_INET;
	bcSocket.Recv_addr.sin_port = htons(port-1);
	bcSocket.Recv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(bcSocket.sock, (sockaddr*)&bcSocket.Recv_addr, sizeof(bcSocket.Recv_addr)) < 0)
	{
		perror("bind");
		closesocket(bcSocket.sock);
	}
	return bcSocket;
}


void UDPbroadcastString(broadcastSocket bcSocket, const char *data) {
	if (sendto(bcSocket.sock, data, strlen(data) + 1, 0, (sockaddr *)&bcSocket.Sender_addr, sizeof(bcSocket.Sender_addr)) < 0)
	{
		perror("borhot send: ");
		closesocket(bcSocket.sock);
	}
	//cout << "sent: " << data << endl;
}


void UDPbroadcastControlParams(broadcastSocket bcSocket, float leftInput, float rightInput, float headingRef, float speedRefKmh, double heading) {
	stringstream ss;
	ss << '$' << "LEFT_MOTOR," << leftInput << "$RIGHT_MOTOR," << rightInput << "$HEADING_REF," << headingRef << "$SPEED_REF," << speedRefKmh << "$HEADING," << heading << "$HEADING_REF," << headingRef << "$SPEED_REF," << speedRefKmh;

	string controlParams = ss.str();
	UDPbroadcastString(bcSocket, controlParams.c_str());
}