
#include"gpsparser.h"

#include <QDebug>



void GpsDataToDecimalConverter(GPSdata  & gpsData);

SOCKET UDPrxSocketConstructor( int Port ) {

    WSADATA            wsaData;
    SOCKADDR_IN        ReceiverAddr;

    // Initialize Winsock version 2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Server: WSAStartup failed with error %ld\n", WSAGetLastError());
        system("pause"); //return -1;
    }
    else
        printf("Server: The Winsock DLL status is %s.\n", wsaData.szSystemStatus);

    SOCKET rxSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (rxSocket == INVALID_SOCKET)
    {
        printf("Server: Error at socket(): %ld\n", WSAGetLastError());
        // Clean up
        WSACleanup();
        system("pause");// return -1;
    }
    else
        printf("Server: socket OK!\n");


    // IPv4
    ReceiverAddr.sin_family = AF_INET;
    // Port no.
    ReceiverAddr.sin_port = htons(Port);
    // From all interfaces (0.0.0.0)
    ReceiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);


    if (bind(rxSocket, (SOCKADDR *)&ReceiverAddr, sizeof(ReceiverAddr)) == SOCKET_ERROR)
    {
        printf("Server: bind() failed! Error: %ld.\n", WSAGetLastError());
        // Close the socket
        closesocket(rxSocket);
        // Do the clean up
        WSACleanup();
        // and exit with error
        system("pause");
        //return -1;
    }
    else
        printf("Server: bind successful!\n");


//    struct timeval timeoutUDP;
//    timeoutUDP.tv_sec = 0;
//    timeoutUDP.tv_usec = 100000;
//    int ms = 10;
//    if (setsockopt(rxSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&ms, sizeof(int)) < 0) {
//        qInfo() << "Something went wrong.";
//    }

    // Info on receiver side...

    getsockname(rxSocket, (SOCKADDR *)&ReceiverAddr, (int *)sizeof(ReceiverAddr));

    printf("Server: Receiving IP used: %s\n", inet_ntoa(ReceiverAddr.sin_addr));
    printf("Server: Receiving port used: %d\n", htons(ReceiverAddr.sin_port));

    printf("Server: Ready to receive...\n");

    return rxSocket;
}


vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
vector<string> splitData(const string s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

void interpretHDT(string HDTString, GPSdata & gpsData) {
    vector<string> AllData = splitData(HDTString, ',');
    gpsData.Heading = ::atof(AllData[1].c_str()); // + 180deg antenna offset
    if (gpsData.Heading >= 360){
        gpsData.Heading = gpsData.Heading - 360;
    }
}
void interpretROT(string ROTString, GPSdata & gpsData) {
    vector<string> AllData = splitData(ROTString, ',');
    gpsData.RateOfTurn = ::atof(AllData[1].c_str());
}
void interpretVTG(string VTGString, GPSdata & gpsData) {
    vector<string> AllData = splitData(VTGString, ',');
    gpsData.TrueTrack = ::atof(AllData[1].c_str());
    gpsData.MagneticTrack = ::atof(AllData[3].c_str());
    gpsData.GroundSpeedKmh = ::atof(AllData[7].c_str());
}
void interpretGGA(string GGAString, GPSdata & gpsData) {
    vector<string> AllData = splitData(GGAString, ',');
    gpsData.Time = ::atof(AllData[1].c_str());
    gpsData.Latitude = ::atof(AllData[2].c_str());
    gpsData.Longitude = ::atof(AllData[4].c_str());
    gpsData.FixQuality = ::atof(AllData[6].c_str());
    GpsDataToDecimalConverter(gpsData);
}
void extractGPSdata(vector<string> elements, int size, GPSdata & gpsData) {
    string DataType;
    for (int i = 1; i <size; i++) {
        DataType = elements[i].substr(2, 3);
        if (DataType == "VTG") {
            interpretVTG(elements[i], gpsData);
        }
        if (DataType == "HDT") {
            interpretHDT(elements[i], gpsData);
        }
        if (DataType == "ROT") {
            interpretROT(elements[i], gpsData);
        }
        if (DataType == "GGA") {
            interpretGGA(elements[i], gpsData);
        }
        if (DataType == "ZDA") {
            //interpretZDA(elements[i])
        }
    }
}

void extractCTRLdata(vector<string> elements, int size, CTRLdata & ctrlData) {
    string DataType;
    for (int i = 1; i <size; i++) {
        vector<string> controlInfo = splitData(elements[i], ',');
        if (controlInfo[0] == "LEFT_MOTOR") {
            ctrlData.leftInput = ::atof(controlInfo[1].c_str());
        }
        if (controlInfo[0] == "RIGHT_MOTOR") {
            ctrlData.rightInput = ::atof(controlInfo[1].c_str());
        }
        if (controlInfo[0] == "HEADING_REF") {
            ctrlData.headingRef = ::atof(controlInfo[1].c_str());
        }
        if (controlInfo[0] == "SPEED_REF") {
            ctrlData.speedRef = ::atof(controlInfo[1].c_str());
        }
        if (controlInfo[0] == "HEADING") {
            ctrlData.heading = ::atof(controlInfo[1].c_str());
        }
        if (controlInfo[0] == "NGHDC") {
            ctrlData.Kph = ::atof(controlInfo[1].c_str());
            ctrlData.Kih = ::atof(controlInfo[2].c_str());
            ctrlData.Kdh = ::atof(controlInfo[3].c_str());
        }
        if (controlInfo[0] == "NGSPD") {
            ctrlData.Kps = ::atof(controlInfo[1].c_str());
            ctrlData.Kis = ::atof(controlInfo[2].c_str());
            ctrlData.Kds = ::atof(controlInfo[3].c_str());
        }
        if (controlInfo[0] == "WPGGA") {
            ctrlData.nextWPlat = ::atof(controlInfo[1].c_str());
            ctrlData.nextWPlong = ::atof(controlInfo[2].c_str());
        }
        if (controlInfo[0] == "NDLOS") {
            ctrlData.LOSdistance = ::atof(controlInfo[1].c_str());
        }
    }
}

void printData(GPSdata & gpsData) {
    cout << "Latitude: \t" << gpsData.Latitude << endl;
    cout << "Longitude: \t" << gpsData.Longitude << endl;
    cout << "Time: \t\t" << gpsData.Time << endl;
    cout << "Fix Quality: \t"<<gpsData.FixQuality << endl;
    cout << "True track: \t" << gpsData.TrueTrack << endl;
    cout << "Magnetic track: " << gpsData.MagneticTrack << endl;
    cout << "Speed (km/h): \t" << gpsData.GroundSpeedKmh << endl;
    cout << "Heading: \t" << gpsData.Heading << endl;
    cout << "Rate of turn: \t" << gpsData.RateOfTurn << endl;
}

void receiveGPSdata(SOCKET rxSocket, GPSdata & gpsData) {
    char               ReceiveBuf[500];
    string             ReceiveString;
    int                BufLength = 500;
    SOCKADDR_IN        SenderAddr;
    int                SenderAddrSize = sizeof(SenderAddr);
    int                ByteReceived = 5;

    fd_set fds ;
    int n ;
    struct timeval tv ;

    // Set up the file descriptor set.
    FD_ZERO(&fds) ;
    FD_SET(rxSocket, &fds) ;

    // Set up the struct timeval for the timeout.
    tv.tv_sec = 0 ;
    tv.tv_usec = 10000 ;

    // Wait until timeout or data received.
    n = select ( rxSocket, &fds, NULL, NULL, &tv ) ;
    if ( n == 0)
    {
      // Timeout...
      return;
    }
    else if( n == -1 )
    {
      printf("Error..\n");
      return;
    }

    ByteReceived = recvfrom(rxSocket, ReceiveBuf, BufLength, 0,
        (SOCKADDR *)&SenderAddr, &SenderAddrSize);
    if (ByteReceived > 0)
    {
        ReceiveString = ReceiveBuf;
        ReceiveString = ReceiveString.substr(0, ByteReceived - 1);
        vector<string> elements = splitData(ReceiveString, '$');
        extractGPSdata(elements, elements.size(), gpsData);
    }
    else if (ByteReceived <= 0)
        ;
        //printf("Server: Connection closed with error code: %ld\n", WSAGetLastError());
    else
        printf("Server: recvfrom() failed with error code: %d\n", WSAGetLastError());

    getpeername(rxSocket, (SOCKADDR *)&SenderAddr, &SenderAddrSize);
}

void receiveCTRLdata(SOCKET rxSocket, CTRLdata & ctrlData) {
    char               ReceiveBuf[500];
    string             ReceiveString;
    int                BufLength = 500;
    SOCKADDR_IN        SenderAddr;
    int                SenderAddrSize = sizeof(SenderAddr);
    int                ByteReceived = 5;

    fd_set fds ;
    int n ;
    struct timeval tv ;

    // Set up the file descriptor set.
    FD_ZERO(&fds) ;
    FD_SET(rxSocket, &fds) ;

    // Set up the struct timeval for the timeout.
    tv.tv_sec = 0 ;
    tv.tv_usec = 10000 ;

    // Wait until timeout or data received.
    n = select ( rxSocket, &fds, NULL, NULL, &tv ) ;
    if ( n == 0)
    {
      //printf("Timeout..\n");
      return;
    }
    else if( n == -1 )
    {
      printf("Error..\n");
      return;
    }

    ByteReceived = recvfrom(rxSocket, ReceiveBuf, BufLength, 0,
        (SOCKADDR *)&SenderAddr, &SenderAddrSize);
    if (ByteReceived > 0)
    {
        ReceiveString = ReceiveBuf;
        ReceiveString = ReceiveString.substr(0, ByteReceived - 1);
        vector<string> elements = splitData(ReceiveString, '$');
        extractCTRLdata(elements, elements.size(), ctrlData);
    }
    else if (ByteReceived <= 0)
        ;
        // printf("Server: Connection closed with error code: %ld\n", WSAGetLastError());
    else
        printf("Server: recvfrom() failed with error code: %d\n", WSAGetLastError());

    getpeername(rxSocket, (SOCKADDR *)&SenderAddr, &SenderAddrSize);
}


void GpsDataToDecimalConverter(GPSdata  & gpsData) {
    //Converts the data received from the GPS to decimaldegrees
   /* int LatitudeDegrees = int(gpsData.Latitude / 100);
    int LongitudeDegrees = int(gpsData.Longitude / 100);
    float LatitudeMinutes = gpsData.Latitude - LatitudeDegrees * 100;
    float LongitudeMinutes = gpsData.Longitude - LongitudeDegrees * 100;
    float Latitude = LatitudeDegrees + LatitudeMinutes / 60;
    float Longitude = LongitudeDegrees + LongitudeMinutes / 60;
    gpsData.Latitude = Latitude;
    gpsData.Longitude = Longitude;*/
}
