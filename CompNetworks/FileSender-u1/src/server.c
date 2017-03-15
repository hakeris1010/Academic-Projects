
//#undef UNICODE
//#define WIN32_LEAN_AND_MEAN

/*#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>*/

#include <stdlib.h>
#include <stdio.h>
#include "gryltools/grylthread.h"
#include "gryltools/gsrvsocks.h"

// Need to link with Ws2_32.lib
//#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define GSRV_DEFAULT_PORT "4440"

#define GBANG_VERSION 1

#define GBANG_ERROR_NOFILE  "E_NOFILE "
#define GBANG_ERROR_SOCKS   "E_SOCKS "

#define GBANG_DATA_START    "START_DATA "
#define GBANG_DATA_SENDING  "DATA "
#define GBANG_DATA_END      "END_DATA "

#define GBANG_REQUEST_FILE      "FILE "
#define GBANG_REQUEST_DIR       "DIR "
#define GBANG_REQUEST_SYSINFO   "SYST "
#define GBANG_REQUEST_EXIT      "EXIT "
#define GBANG_REQUEST_SHUTDOWN  "SHUTDOWN "
 
#define GBANG_HEADER_SIZE   16
#define GBANG_DATA_SIZE     1472

// Statuses
#define GSRV_STATUS_ACTIVE          1
#define GSRV_STATUS_RECEIVE_PENDING 2
#define GSRV_STATUS_SEND_PENDING    4
#define GSRV_STATUS_IDLE            8

#define GSRV_MAX_CLIENTS 32
// Accept this much connections
#define GSRV_CONNECTIONS_TO_ACCEPT 4

struct __attribute__((__packed__)) GrylBangProtoData // Set no padding for the accurate packet size.
{
    //Header
    char version;
    char commandString[ GBANG_HEADER_SIZE - 1 ];
    //Data
    char data[ GBANG_DATA_SIZE ];
};

typedef struct 
{
    SOCKET sock;
    struct GrylBangProtoData prot;
    short status;
} GrylSockStruct;

typedef struct
{
    GrylSockStruct* sockStruct;
    GrThreadHandle threadHandle;
} ClientThread;

typedef struct 
{
    volatile char needToClose;
    volatile size_t threadCount;
} GlobalDescStructureXXX;

static GlobalDescStructureXXX GlobalDesc = { 0 };

// Functions. 

int sendPacket(GrylSockStruct* st, const char* command, size_t dataLen, char cleanupOnError)
{
    if(!st) return 0;

    (st->prot).version = GBANG_VERSION;
    if(command)
        strcpy((st->prot).commandString, command);

    int iSendResult = send( st->sock, (char*)&(st->prot), GBANG_HEADER_SIZE + dataLen, 0 );
    if (iSendResult == SOCKET_ERROR && cleanupOnError) {
        gsockErrorCleanup(st->sock, NULL, "send failed with error", 0, 0);
        return -1;
    }

    return iSendResult;
}

int sendFile(GrylSockStruct* sock, const char* fname)
{
    if(!sock) return -2;
    
    // Try to open file.
    printf("Trying to open file: %s|\n", fname);
    int iSendResult;

    FILE* inputFile = fopen(fname, "rb");

    if(!inputFile){
        printf("File requested can't be opened. Terminating.\n");

        if( (iSendResult = sendPacket(sock, GBANG_ERROR_NOFILE, 0, 1)) < 0 )
            return -1;
        printf("Bytes sent: %d\n", iSendResult);
    }
    else{ // File exists.
        // Send file data Begin message
        if( (iSendResult = sendPacket(sock, GBANG_DATA_START, 0, 1)) < 0 )
            return -1;

        size_t bytesRead;
        do{
            // Read it directly into the packet buffer
            size_t bytesRead = fread((sock->prot).data, 1, sizeof((sock->prot).data), inputFile);

            if(bytesRead > 0){
                // Send the packet with a file data.
                if( (iSendResult = sendPacket(sock, GBANG_DATA_SENDING, bytesRead, 1)) < 0 ){
                    sendPacket(sock, GBANG_ERROR_SOCKS, 0, 1);
                    return -1;
                }

                printf("Bytes sent: %d\n", iSendResult);
            }
        } while(!feof(inputFile) && !ferror(inputFile));
        
        fclose(inputFile);

        // Send file data End message
        if( (iSendResult = sendPacket(sock, GBANG_DATA_END, 0, 1)) < 0 )
            return -1;
    }

    return 0;
}

void nullifyFnameEnd(char* fname, int size)
{
    if(!fname || size<=0) return;
    fname[size] = 0;

    for(int i=size-1; i>0; i--){
        if(fname[i] < 32) // invalid
            fname[i] = 0;
    }
}

void runClient(void* param)
{
    if(!param) return;

    GrylSockStruct* cliSock = (GrylSockStruct*)param;

    int iResult, datalen;
    char* command = (cliSock->prot).commandString;
    char* databuf = (cliSock->prot).data;

    printf("\n* * * * *\nRunning the client loop...\n\n");
    // Receive until the peer shuts down the connection
    do {
        // Receive whole Bang protocol structure at once.
        iResult = recv(cliSock->sock, (char*)&(cliSock->prot), sizeof(struct GrylBangProtoData), 0);

        if (iResult > 0) { // Got bytes. iResult: how many bytes got.
            printf("Packet received from Socket:%d. Bytes received: %d\n", cliSock->sock, iResult);
            //printf("Packet data:\n%.*s\n", iResult, &(cliSock->prot));
            datalen = iResult - GBANG_HEADER_SIZE;
            
            // Check for the values and do the thing.
            // GBANG_REQUEST_FILE
            if( strncmp( command, GBANG_REQUEST_FILE, strlen(GBANG_REQUEST_FILE) ) == 0 ){
                // Filename is expected on Data. Null-terminate the end of it.
                nullifyFnameEnd(databuf, datalen);
                printf("Got FILE request. Fname: %s\n", databuf);
                
                if(sendFile( cliSock, databuf ) != 0)
                    printf("Error occured while sending file.\n");
                printf("\n");    
            }
            //GBANG_REQUEST_DIR
            else if( strncmp( command, GBANG_REQUEST_DIR, strlen(GBANG_REQUEST_DIR) ) == 0 ){
                // Get directory representation in string. 
            }
            //GBANG_REQUEST_SYSINFO
            else if( strncmp( command, GBANG_REQUEST_SYSINFO, strlen(GBANG_REQUEST_SYSINFO) ) == 0 ){
                //
            }
            //GBANG_REQUEST_EXIT
            else if( strncmp( command, GBANG_REQUEST_EXIT, strlen(GBANG_REQUEST_EXIT) ) == 0 ){
                printf("EXIT command received.\n");
                iResult = 0; // Exit the loop and close client.                
            }
            //ANG_REQUEST_SHUTDOWN
            else if( strncmp( command, GBANG_REQUEST_SHUTDOWN, strlen(GBANG_REQUEST_SHUTDOWN) ) == 0 ){
                GlobalDesc.needToClose = 1; // Set the close flag for all the threads.
            }
        }
        else if (iResult == 0) // Client socket shut down'd properly. Close connection message has been posted (TCP FIN).
            printf("Close message posted.\n");
        else                   // Error occured.
            printf("recv failed with error: %d\n", gsockGetLastError());

    } while (iResult > 0 && !GlobalDesc.needToClose);
    
    printf("Sock. %d : Closing the thread...\n - - - - - \n", cliSock->sock);
    gsockCloseSocket(cliSock->sock);
    cliSock->status &= ~GSRV_STATUS_ACTIVE; // Make inactive (clear specific bit).
}

// Arg: Port number on which we'll listen.
int runServer(const char* port)
{
    printf("Init start vars... ");

    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    struct sockaddr_in sin;
    socklen_t sinlen;

    // ThreadPool.
    ClientThread clientThreadPool[ GSRV_MAX_CLIENTS ];
    memset( clientThreadPool, 0, sizeof(clientThreadPool) ); // Everything == NULL.


    // Initialize  Startup.
    printf("Done.\nInit WinSock... ");

    if(gsockInitSocks() != 0)
        return 1;

    printf("Done.\nInit addrinfo hints...");

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // Use IPv4 socket mode.
    hints.ai_socktype = SOCK_STREAM; // Stream mode. (For UDP, we use Datagram)
    hints.ai_protocol = IPPROTO_TCP; // Use TCP for communicating.
    hints.ai_flags = AI_PASSIVE;     // Use it for listening.

    //==============================================//
    // Resolve the server address and port with the specified hints.
    printf("Done.\nCalling getAddrInfo, with specified port number as a service... ");

    iResult = getaddrinfo(NULL, port, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        gsockSockCleanup();
        return 1;
    }

    // Create a listening SOCKET for connecting to a client.
    printf("Done.\nCreate a ListenSocket (socket())...");
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        return gsockErrorCleanup(INVALID_SOCKET, result, "socket failed with error", 1, 1);
    }

    // Setup the TCP listening socket, bind it to a local server address.
    printf("Done.\nBinding ListenSocket... ");
    iResult = bind( ListenSocket, result->ai_addr, (int)(result->ai_addrlen));
    if (iResult == SOCKET_ERROR) {
        return gsockErrorCleanup(ListenSocket, result, "bind failed with error", 1, 1);
    }

    printf("Done.\nFreeAddrInfo()... ");
    freeaddrinfo(result);

    // Mark the ListenSocket as the socket willing to accept incoming connections.
    // Max pending connections: SOMAXCONN. This is defined by system.
    printf("Done.\nlisten(ListenSocket)... ");
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        return gsockErrorCleanup(ListenSocket, NULL, "listen failed with error", 1, 1);
    }

    //===================================================//
    // Print on which port the socket is listening.
    printf("Done.\nGetSockName()... ");
        if (getsockname(ListenSocket, (struct sockaddr *)&sin, &sinlen) == -1)
            printf("getsockname err. can't get port.\n");
    else
        printf("\nThe server is listening on port: %d\n", ntohs(sin.sin_port));


    // Initialize and start accept/receive loop.
    size_t connectionsAccepted = 0;
    char exitLoop = 0;
    
    printf("Done.\n\nStarting Loop... \n");
    while(!exitLoop && !GlobalDesc.needToClose) // Run a server loop.
    {
        // Extract first request from a connection queue.
        // Blocks the thread until connection is received.
        printf("\nWaiting for the connection.....\n");
        
        SOCKET ClientSocket = accept(ListenSocket, (struct sockaddr *)&sin, (socklen_t*)&sinlen);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", gsockGetLastError());
            break;
        }

        printf("Connection accepted!\n Client IP: %s\n Port: %d\n\n", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
        
        for(int i=0; i<GSRV_MAX_CLIENTS; i++)
        {
            if(!clientThreadPool[i].threadHandle && !clientThreadPool[i].sockStruct){ //is still empty
                printf("Found empty position at %d. Adding client thread...\n", i);

                // Just create a new thread and GrylSockStruct for a client
                clientThreadPool[i].sockStruct = (GrylSockStruct*)calloc( 1, sizeof(GrylSockStruct) );
                clientThreadPool[i].sockStruct->status |= GSRV_STATUS_ACTIVE;
                clientThreadPool[i].sockStruct->sock = ClientSocket;
                // Spawn this thread. Param - GrylSockStruct client structure
                clientThreadPool[i].threadHandle = procToThread(runClient, clientThreadPool[i].sockStruct); 
                break;
            }

            //After this the clientThreadPool[i] is populated.
                
            if(!isThreadRunning( clientThreadPool[i].threadHandle )){
                printf("Found closed thread at %d. Adding client thread...\n", i);

                // If thread is not running, we can assign a new thread here.
                joinThread( clientThreadPool[i].threadHandle );
                // Repopulate the sockStruct
                clientThreadPool[i].sockStruct->status |= GSRV_STATUS_ACTIVE;
                clientThreadPool[i].sockStruct->sock = ClientSocket;
                // Spawn the thread
                clientThreadPool[i].threadHandle = procToThread(runClient, clientThreadPool[i].sockStruct); 
                break;
            }
        }
    }
    
    printf("\nLoop Ended. Cleaning up......\n");
    // Cleanup the memory.
    for(int i=0; i<GSRV_MAX_CLIENTS; i++){
        if(clientThreadPool[i].threadHandle){
            joinThread( clientThreadPool[i].threadHandle );
        }
        if(clientThreadPool[i].sockStruct){
            free(clientThreadPool[i].sockStruct);
        }
    }

    // cleanup
    closesocket(ListenSocket);
    gsockSockCleanup();

    return 0;
}

int main(int argc, char** argv)
{
    printf("Nyaaaa >.<\n");
    return runServer( argc>1 ? (const char*)argv[1] : GSRV_DEFAULT_PORT );
}

