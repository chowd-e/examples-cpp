//--------------------------------------------------------------------
// TcpSocket.h
// Implementation file for TcpSocket class
// Author: Chase Howard
// Class: CSS 503B [University of Washington Bothell]
// Date: 2021-06-06
//-------------------------------------------------------------------- 
//	Description: 
//    This class opens and maintains a TcpConnection and can interface with
//    a client program through the use of the socketSd member variable, in 
//    which the client may use sysetm calls such as recv / send / read / write
//    to pass data to the socket opened

// Usage:
//    This class allows for the TcpSocket to function either as a client or
//    a server which is specified by the 'type' member variable and assigned
//    when opeing a socket, depending on whethere the parameter for 
//    'serverName' is NULL or not

#include <iostream>
#include <unistd.h>
#include <strings.h>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1500
#define NULL_SD -1
#define BACKLOG 5

using namespace std;

class TcpSocket
{
   enum Type {SERVER, CLIENT};

   public:
      //-----------------------------------------------------------------------
      // TcpSocket: Default Constructor
      //-----------------------------------------------------------------------
      // Pre: none
      // Post: Object created and initialized as Client 
      TcpSocket() : socketSd(NULL_SD), type(CLIENT){};

      //-----------------------------------------------------------------------
      // ~TcpSocket: Destructor
      //-----------------------------------------------------------------------
      // Pre: none
      // Post: Closes socket if valid
      ~TcpSocket();

      //-----------------------------------------------------------------------
      // openSocket: Creates and connects to specified socket
      //-----------------------------------------------------------------------
      // Pre: none
      // Post: Can open socket as a server or Client depending on parameter
      //    passed for serverName
      //
      //    serverName = NULL -> Function as a server
      //       getServerInfo(), setSocketSd(), bindSd();
      //
      //    serverName != NULL -> function as a client
      //       getServerInfo(), setSocketSd(), connectsd();
      //
      //    returns socketSd on sucess, returns -1 on error and prints to cout
      //       error inforamtion
      int openSocket(char *serverName, int port);

      //-----------------------------------------------------------------------
      // getClient: Listens for and accepts new client (Server Only)
      //-----------------------------------------------------------------------
      // Pre: calling object must be of SERVER type
      // Post: Server holds, waiting for a client, and accepts the client.
      //    upon sucess, will return clients socketSd, otherwise, returns -1
      int getClient();

      //-----------------------------------------------------------------------
      // getSocketSd: return socketSd for this object
      //-----------------------------------------------------------------------
      // Pre: none
      // Post: Returns value stored in member variable socketSd
      int getSocketSd() const {return this->socketSd;};
      
   private:
      int socketSd;
      int type; // indicates if object is CLIENT or SERVER
      addrinfo *serverInfo; // server address structure 

      //-----------------------------------------------------------------------
      // setServerInfo: sets information regarding server in 'serverInfo' 
      //-----------------------------------------------------------------------
      // Pre: none
      // Post: information for server is stored in member variable 
      //   'serverInfo'. returns -1 on failure with a print of error 
      int setServerInfo(char *serverName, int port);

      //-----------------------------------------------------------------------
      // setSocket: sets socket paramters and 
      //-----------------------------------------------------------------------
      // Pre: previously called setServerInfo()
      // Post: Socket now bound, returns 0 on success, -1 otherwise, also 
      //    has free'd serverInfo  
      int setSocketSd();

      //-----------------------------------------------------------------------
      // connectSd: connects Client/Server to socket
      //-----------------------------------------------------------------------
      // Pre: previously called setServerInfo()
      // Post: Socket now bound, returns 0 on success, -1 otherwise, also 
      //    has free'd serverInfo
      int connectSd();

      //-----------------------------------------------------------------------
      // bind: binds server to socket
      //-----------------------------------------------------------------------
      // Pre: Server type, previously called setServerInfo() and setSocketSd
      // Post: Server socket is now bound, returns -1 on failure, returns 0 on
      //    sucess 
      int bindSd();
};