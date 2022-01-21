//--------------------------------------------------------------------
// Client.cpp
// Driver file for Client execution
// Author: Chase Howard
// Class: CSS 503B [University of Washington Bothell]
// Date: 2021-06-06
//-------------------------------------------------------------------- 
//	Description: 
//    See attached documentation for in depth description

#include <limits.h>

#include "Timer.h"
#include "TcpSocket.h"

#define BUFSIZE 1500
#define INT_BYTES 32

using namespace std;

int main(int argc, char *argv[])
{
   // -------------------------------------------------------------------------
   //                              validate input
   // -------------------------------------------------------------------------
   if (argc != 7)
   {
      cerr << "usage: ./client serverName port repetition nbufs " <<
              "bufsize type"  << endl;
      return -1;
   }

   char *serverName = argv[1];
   int port = atoi(argv[2]);
   int repetitions = atoi(argv[3]);
   int nbufs = atoi(argv[4]);
   int bufsize = atoi(argv[5]);
   int type = atoi(argv[6]);

   if(nbufs * bufsize != 1500)
   {
      cerr << "usage: nbufs * bufsize must equal 1500" << endl;
      return -1;
   }

   if(port < 5001)
   {
      cerr << "usage: port > 5000" << endl;
      return -1;
   }

   if(repetitions < 0 || repetitions > UINT_MAX)
   {
      cerr << "usage: repetitions > 0 && < UINT_MAX" << endl;
      return -1;
   }

   if(type < 1 || type > 3)
   {
      cerr << "usage: type must be either [1, 2, or 3]" << endl;
      return -1;
   }

   // -------------------------------------------------------------------------
   //                           Open Connection
   // ------------------------------------------------------------------------ 
   TcpSocket clientSock;
   int clientSd = clientSock.openSocket(serverName, port);

   if (clientSd < 0)
      return -1;

   // -------------------------------------------------------------------------
   //                    Build / Send Repetition Message
   // -------------------------------------------------------------------------
   // int to string
   string intStr = to_string(repetitions);
   int len = intStr.length();
   // to cstring
   char cStr[++len];
   strcpy(cStr, intStr.c_str());

   // Send reps to server, error checking 
   if(INT_BYTES != write(clientSd, (void *) cStr, INT_BYTES))
   {
      cerr << "Repetitions write fail" << endl;
      return -1;
   }

   // -------------------------------------------------------------------------
   //                        Build / Send Messages
   // -------------------------------------------------------------------------
 
   //start timer
   Timer timer;
   
   // initialize nbuf
   char databuf[nbufs][bufsize];
   memset(databuf, '0', nbufs * bufsize);

   // build messages
   for(int i = 0; i < repetitions; i++)
   {
      switch(type)
      {
         case 1: // multiple writes
         {
            for (int j = 0; j < nbufs; j++)
            {
               write(clientSd, databuf[j], bufsize);
            }
            break;
         }
         case 2: // using writev
         {
            struct iovec vector[nbufs];
            for (int j = 0; j < nbufs; j++)
            {
               vector[j].iov_base = databuf[j];
               vector[j].iov_len = bufsize;
            }

            writev(clientSd, vector, nbufs);
            break;
         }
         case 3: // bulk messaging
         {
            write(clientSd, databuf, nbufs * bufsize);
            break;
         }
         default:
         {
            cerr << "invalid type: accepted vals [1, 2, 3]" << endl;
            return -1;
         }
      }
   }

   // -------------------------------------------------------------------------
   //                 Wait for Response + Print Results
   // -------------------------------------------------------------------------

   // wait for response
   char countRD[BUFSIZE];
   read(clientSd, countRD, BUFSIZE);
   
   // stop timer
   double sec_elapsed = timer.finish();

   // print results
   printf("Test %d: time = %0.0f µs, #reads = %s, throughput = %0.2f Gbps\n",
            type, 
            sec_elapsed * 1000000, 
            countRD, 
            (repetitions * BUFSIZE * 8) / (sec_elapsed * 1000000000)
         );
}

