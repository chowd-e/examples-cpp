//--------------------------------------------------------------------
// Server.cpp
// Driver file for Server execution
// Author: Chase Howard
// Class: CSS 503B [University of Washington Bothell]
// Date: 2021-06-06
//-------------------------------------------------------------------- 
//	Description: 
//    See attached documentation for in depth description

#include <pthread.h>

#include "TcpSocket.h"

using namespace std;

#define BACKLOG 5  // maximum size of clinet backlog
#define BUFSIZE 1500
#define INT_BYTES 32

//-----------------------------------------------------------------------
// threadRunner: function for handling clients connected to
//-----------------------------------------------------------------------
// Pre: valid clientSd passed by clientPtr
// Post: read message from client indicating the planned nubmer of repetitions
//    and has processed each of the clients write values. Additionally, sends
//    an acknowledgement back to client with the nubmer of reads performed.
void *threadRunner(void *clientPtr)
{
   // convert void ptr to int
   int *newClientPtr = (int *) clientPtr;
   int clientSd = *newClientPtr;

   if(clientSd < 0)
      return nullptr;

   printf("Accepted Client: %d\n", clientSd);

   // read number of repetitions
   char bufRep[INT_BYTES];
   read(clientSd, (void *)  bufRep, INT_BYTES);
   int repetitions = atoi(bufRep);

   // set buffer expectations
   char buf[BUFSIZE];

   int countRD = 0;  // number of read calls made
   int bytesRD = 0;  // cumulative bytes read within an iteration / buffer pos
   int bytes = 0;    // #bytes read in single read call
   
   for (int i = 0; i < repetitions; i++)
   {
      // read until buffer fills
      while(bytesRD < BUFSIZE)
      {
         bytes = read(clientSd, &buf[bytesRD], BUFSIZE - bytesRD);

         if(bytes < 0)
         {
            cerr << "Server read error" << endl;
            break;
         }

         // increment buffer position and update counters
         bytesRD += bytes; 
         countRD++;
      }

      // reset values for next iteration
      bytesRD = 0;
   }
   
   // -------------------------------------------------------------------------
   //                              Send Response
   // -------------------------------------------------------------------------
   
   // convert to string
   string intStr = to_string(countRD);
   int len = intStr.length();
   
   // convert to c_string
   char cStr[++len];
   strcpy(cStr, intStr.c_str());
   
   write(clientSd, (void *) cStr, len);
   
   close(clientSd);
   printf("Terminated Client: %d\n", clientSd);

   return nullptr;
}

int main(int argc, char *argv[])
{
   // -------------------------------------------------------------------------
   //                              validate input
   // -------------------------------------------------------------------------

   if (argc != 2)
   {
      cerr << "usage: ./server port" << endl;
      return -1;
   }

   int port = atoi(argv[1]);

   if(port < 5001)
   {
      cerr << "Server usage: port > 5000" << endl;
      return -1;
   }

   // -------------------------------------------------------------------------
   //                           Open Connection
   // -------------------------------------------------------------------------
   
   TcpSocket server;
   if (server.openSocket(NULL, port) < 0)
      return -1;

   // -------------------------------------------------------------------------
   //                           LISTEN + ACCEPT
   // -------------------------------------------------------------------------
   while(true)
   {
      cout << "Server listening..." << endl;

      int newClient = server.getClient();
      
      if (newClient < 0)
      {   
         cerr << "Failure to accept" << endl;
         break;
      }  

      // spawn new thread to handle client
      pthread_t clientTd;
      pthread_create(&clientTd, NULL, threadRunner, (void *) &newClient);
   }
   
   // if loop breaks, terminate w/ -1
   return -1;
}




