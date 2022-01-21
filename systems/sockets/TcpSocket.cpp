//--------------------------------------------------------------------
// TcpSocket.h
// Implementation file for TcpSocket class
// Author: Chase Howard
// Class: CSS 503B [University of Washington Bothell]
// Date: 2021-06-06
//-------------------------------------------------------------------- 
//	Description: 
//    See header file for description

#include "TcpSocket.h"

using namespace std;

//-----------------------------------------------------------------------
// ~TcpSocket: Destructor
//-----------------------------------------------------------------------
// Pre: none
// Post: Closes socket if valid
TcpSocket::~TcpSocket()
{
   if (this->socketSd != NULL_SD)
      close(this->socketSd);
}


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
int TcpSocket::openSocket(char *serverName, int port)
{ 
   if(setServerInfo(serverName, port) < 0)
   {
      cerr << "Error retrieving server info" << endl;
      return -1;
   }
   
   if(setSocketSd() < 0)
   {
      cerr << "Error setting Socket parameters" << endl;
      return -1;
   }

   // differing behavior for servers and clients
   if(this->type == SERVER)
   {
      if (bindSd() < 0)
      {
         cerr << "Failure to bind server to socket" << endl;
         return -1;
      }
   }
   else // this is a client
   {
      if (connectSd() < 0)
      {
         cerr << "Failure connecting Client to socket" << endl;
         return -1;
      }
   }

   return this->socketSd;
}

//-----------------------------------------------------------------------
// getClient: Listens for and accepts new client (Server Only)
//-----------------------------------------------------------------------
// Pre: calling object must be of SERVER type
// Post: Server holds, waiting for a client, and accepts the client.
//    upon sucess, will return clients socketSd, otherwise, returns -1
int TcpSocket::getClient()
{
   if(this->type != SERVER)
   {
      cerr << "Clients cannot call this method" << endl;
      return -1;
   }

   //listen for clients
   if(listen(this->socketSd, BACKLOG) < 0)
   {
      cerr << "listen error" << endl;
      return -1;
   }
   // accept new connection
   return accept(this->socketSd, 
                (struct sockaddr *) &(this->serverInfo->ai_addr),
                (socklen_t *) &(this->serverInfo->ai_addrlen));
}

//-----------------------------------------------------------------------
// setServerInfo: sets information regarding server in 'serverInfo' 
//-----------------------------------------------------------------------
// Pre: none
// Post: information for server is stored in member variable 
//   'serverInfo'. returns -1 on failure with a print of error 
//    flags are set for IPv4 addresses and TCP Streams
int TcpSocket::setServerInfo(char *serverName, int port)
{
   // add a case for accepting null serverName for server
   if (serverName == NULL)
      this->type = SERVER;

   struct addrinfo *storageInfo;
   struct addrinfo hints;

   // char* strPort = itocs(port);
   string intStr = to_string(port);
   int len = intStr.length();

   char strPort[++len];
   strcpy(strPort, intStr.c_str());

   memset(&hints, 0, sizeof(hints));
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE;

   if(getaddrinfo(serverName, strPort, &hints, &storageInfo) < 0)
   {
      cerr << "Failure to get server information" << endl;
      return -1;
   }

   // -------------------------------------------------------------------------
   //                        Get Valid Hostname
   // -------------------------------------------------------------------------
   for(this->serverInfo = storageInfo; 
       this->serverInfo != NULL; 
       this->serverInfo = serverInfo->ai_next)
   {
      char hostname[1024];
      if (getnameinfo(serverInfo->ai_addr, 
                      serverInfo->ai_addrlen,
                      hostname, 
                      1024, 
                      NULL, 0, 0) != 0)
      {
         fprintf(stderr, "Error in getnameinfo\n");
         continue;
      }

      if (*hostname != '\0') // found proper hostname, leave LL here
         break;
   }

   // hostname found? return accordingly
   return (serverInfo == NULL) ? -1 : 0;
}

//-----------------------------------------------------------------------
// setSocket: sets socket paramters and 
//-----------------------------------------------------------------------
// Pre: previously called setServerInfo()
// Post: Socket now bound, returns 0 on success, -1 otherwise, also 
//    has free'd serverInfo  
int TcpSocket::setSocketSd()
{
   this->socketSd = socket(serverInfo->ai_family,
                           serverInfo->ai_socktype,
                           serverInfo->ai_protocol);

   if(this->socketSd < 0)
   {
      cerr << "Failure to open client socket" << endl;
      return -1;
   }

   if(this->type == SERVER)
   {
      const int on = 1;
 	   setsockopt(this->socketSd, SOL_SOCKET, SO_REUSEADDR,
                (char *)&on, sizeof(int));
   }

   freeaddrinfo(this->serverInfo);
   return this->socketSd;
}

//-----------------------------------------------------------------------
// connectSd: connects Client/Server to socket
//-----------------------------------------------------------------------
// Pre: previously called setServerInfo()
// Post: Socket now bound, returns 0 on success, -1 otherwise, also 
//    has free'd serverInfo
int TcpSocket::connectSd()
{
   if(serverInfo == NULL)
   {
      cerr << "Requires successful call to getServerInfo" << endl;
      return -1;
   }

   if(connect(this->socketSd,
             (struct sockaddr *) this->serverInfo->ai_addr,
              this->serverInfo->ai_addrlen) < 0)
   {
      cerr << "Failure to connect to server" << endl;
      return -1;
   }

   return 0;
}

//-----------------------------------------------------------------------
// bind: binds server to socket
//-----------------------------------------------------------------------
// Pre: Server type, previously called setServerInfo() and setSocketSd
// Post: Server socket is now bound, returns -1 on failure, returns 0 on
//    sucess 
int TcpSocket::bindSd()
{
   return bind(this->socketSd, 
               this->serverInfo->ai_addr,
               this->serverInfo->ai_addrlen);
}





