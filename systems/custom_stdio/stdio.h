//--------------------------------------------------------------------
// stdio.h
// Header file for stdio 
// Author: Chase Howard
// Class: CSS 503B [University of Washington Bothell]
// Date: 2021-05-26
//-------------------------------------------------------------------- 
//	Description: 
//    This program facilitates file I/O operations by use of a managed 
//    buffer to limit system calls. Additionally provided modularity
//    to file I/O operations. This is a partial implemtnation of the stdio.h

//    See attached documentation for more in depth description

#ifndef _MY_STDIO_H_
#define _MY_STDIO_H_

#define BUFSIZ 8192 // default buffer size
#define _IONBF 0    // unbuffered
#define _IOLBF 1    // line buffered
#define _IOFBF 2    // fully buffered
#define EOF -1      // end of file

#include <unistd.h>

class FILE 
{
 public:
  FILE() 
  {
     fd = -1;
     pos = 0;
     buffer = (char *) 0;
     size = 0;
     actual_size = 0;
     mode = _IONBF;
     flag = 0;
     bufown = false;
     lastop = 0;  
     eof = false; 
  }

  int fd;          // a Unix file descriptor of an opened file
  int pos;         // the current file position in the buffer
  char *buffer;    // an input or output file stream buffer
  int size;        // the buffer size
  int actual_size; // the actual buffer size when read( ) returns # bytes read
                   // smaller than size
  int mode;        // _IONBF, _IOLBF, _IOFBF
  int flag;        // O_RDONLY = 0
                   // O_RDWR = 2
                   // O_WRONLY | O_CREAT | O_TRUNC = 577
                   // O_WRONLY | O_CREAT | O_APPEND = 1089
                   // O_RDWR   | O_CREAT | O_TRUNC = 578
                   // O_RDWR   | O_CREAT | O_APPEND = 1090
  bool bufown;     // true if allocated by stdio.h or false by a user
  char lastop;     // 'r' or 'w' 
  bool eof;        // true if EOF is reached
};
#include "stdio.cpp"
#endif
