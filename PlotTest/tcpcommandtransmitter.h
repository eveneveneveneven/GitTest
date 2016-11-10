#pragma once

#include <winsock.h>

#define COMMAND_RCV_PORT    27015
#define COMMAND_RCV_IP      "129.241.154.69"     // for Jolner
//#define COMMAND_RCV_IP      "157.237.50.14"         // for Evens PC

bool ConnectToHost(int PortNo, const char* IPAddress);
bool sendCommand(const char* command);
