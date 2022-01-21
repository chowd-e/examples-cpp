//--------------------------------------------------------------------
// stdio.cpp
// Implementation file for stdio 
// Author: Chase Howard
// Class: CSS 503B [University of Washington Bothell]
// Date: 2021-05-26
//-------------------------------------------------------------------- 
//	Description: 
//    This program facilitates file I/O operations by use of a managed 
//    buffer to limit system calls. Additionally provided modularity
//    to file I/O operations. This is a partial implemtnation of the stdio.h

//    See attached documentation for more detailed description
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdarg.h>   
#include <stdlib.h> 
#include "stdio.h"

#include <errno.h>

using namespace std;

char decimal[100];

//-----------------------------------------------------------------------------
//   recursive_itoa(): Recursively Convert integer to string. 
//-----------------------------------------------------------------------------
//  Pre: 
//  Post: converted integer value 'arg' to null terminated string and stored
//    the result in array which is returned
int recursive_itoa(int arg) 
{
	int div = arg / 10;
	int mod = arg % 10;
	int index = 0;
	if (div > 0)
	{
		index = recursive_itoa(div);
	}
	decimal[index] = mod + '0';
	return ++index;
}

//-----------------------------------------------------------------------------
//   itoa(): Convert integer to string. 
//-----------------------------------------------------------------------------
//  makes a call to the recursive function 'recursive_itoa()'
//  Pre: 
//  Post: converted integer value 'arg' to null terminated string and stored
//    the result in array which is returned
char *itoa(const int arg) 
{
	bzero(decimal, 100); // copies n bytes, each w/ value of zero, into string
	int order = recursive_itoa(arg); // recursively places integer to order
	char *new_decimal = new char[order + 1]; //
	bcopy(decimal, new_decimal, order + 1); // copies new_dec into decimal,
	return new_decimal;
}

//-----------------------------------------------------------------------------
//   printf(): Print formatted data to stdout. 
//-----------------------------------------------------------------------------
/* Desc: Writes the C string pointed by format to the stream. If format 
      includes format specifiers (subsequences beginning with %), the 
      additional arguments following format are formatted and inserted in the 
      resulting string replacing their respective specifiers.

      After the format parameter, the function expects at least as many 
      additional arguments as specified by format.
   Pre:
   Post: On success, the total number of characters written is returned.

      If a writing error occurs, the error indicator (ferror) is set and a 
      negative number is returned.

      If a multibyte character encoding error occurs while writing wide 
      characters, errno is set to EILSEQ and a negative number is returned.*/
int printf(const void *format, ...) 
{
	va_list list;
	va_start(list, format);

	char *msg = (char *)format;
	char buf[1024];
	int nWritten = 0;

	int i = 0, j = 0; //, k = 0;
	while (msg[i] != '\0') 
	{
		if (msg[i] == '%' && msg[i + 1] == 'd')
		{
			buf[j] = '\0';
			nWritten += write(1, buf, j);
			j = 0;
			i += 2;

			int int_val = va_arg(list, int);
			char *dec = itoa(abs(int_val));
			if (int_val < 0)
			{
				nWritten += write(1, "-", 1);
			}	
			nWritten += write(1, dec, strlen(dec));
			delete dec;
		}
		else
		{
			buf[j++] = msg[i++];
		}	
	}
	if (j > 0)
	{
		nWritten += write(1, buf, j);
	}	
	va_end( list );
	return nWritten;
}

//-----------------------------------------------------------------------------
//   setvbuf(): Change stream buffering. 
//-----------------------------------------------------------------------------
//  Specifies a buffer for stream. The function allows to specify the mode and
//     size of the buffer in bytes. 
//  Pre: file stream created
//  Post: If the buffer is correctly assigned to the file, a zero value is
//     returned otherwise a non-zero value is returned
int setvbuf(FILE *stream, char *buf, int mode, size_t size) 
{
	if (mode != _IONBF && mode != _IOLBF && mode != _IOFBF)
	{
		return -1;
	}	
	stream->mode = mode;
	stream->pos = 0;
	if (stream->buffer != (char *)0 && stream->bufown == true)
	{
		delete stream->buffer;
	}
	
	switch ( mode ) 
	{
		case _IONBF:
			stream->buffer = (char *)0;
			stream->size = 0;
			stream->bufown = false;
			break;
		case _IOLBF:
		case _IOFBF:
			if (buf != (char *)0) 
			{
				stream->buffer = buf;
				stream->size   = size;
				stream->bufown = false;
			}
			else 
			{
				stream->buffer = new char[BUFSIZ];
				stream->size = BUFSIZ;
				stream->bufown = true;
			}
			break;
	}
	return 0;
}

//-----------------------------------------------------------------------------
//   setbuf(): Set stream buffer. 
//-----------------------------------------------------------------------------
/* Desc: specifies the buffer to be used by the stream for I/O operations
      which becomes a fully buffered stream. Or, if buffer is a null pointer,
      buffering is disabled for the stream, and becomes an unbuffered stream.
   Pre: stream exists and associated with file but before any input or output
      operation is performed.
   Post: no values returned */
void setbuf(FILE *stream, char *buf) 
{
	setvbuf(stream, buf, ( buf != (char *)0 ) ? _IOFBF : _IONBF , BUFSIZ);
}

FILE *fopen(const char *path, const char *mode) 
{
	FILE *stream = new FILE();
	setvbuf(stream, (char *)0, _IOFBF, BUFSIZ);
	
	// fopen( ) mode
	// r or rb = O_RDONLY
	// w or wb = O_WRONLY | O_CREAT | O_TRUNC
	// a or ab = O_WRONLY | O_CREAT | O_APPEND
	// r+ or rb+ or r+b = O_RDWR
	// w+ or wb+ or w+b = O_RDWR | O_CREAT | O_TRUNC
	// a+ or ab+ or a+b = O_RDWR | O_CREAT | O_APPEND

  switch(mode[0]) 
  {
  case 'r':
	  if (mode[1] == '\0')            // r
	  {
		  stream->flag = O_RDONLY;
	  }  
	  else if ( mode[1] == 'b' ) 
	  {    
		  if (mode[2] == '\0')          // rb
		  {
			  stream->flag = O_RDONLY;
		  } 
		  else if (mode[2] == '+')      // rb+
		  {
			  stream->flag = O_RDWR;
		  }			  
	  }
	  else if (mode[1] == '+')        // r+  r+b
	  {
		  stream->flag = O_RDWR;
	  }  
	  break;
  case 'w':
	  if (mode[1] == '\0')            // w
	  {
		  stream->flag = O_WRONLY | O_CREAT | O_TRUNC;
	  }	  
	  else if (mode[1] == 'b') 
	  {
		  if (mode[2] == '\0')          // wb
		  {
			  stream->flag = O_WRONLY | O_CREAT | O_TRUNC;
		  }	  
		  else if (mode[2] == '+')      // wb+
		  {
			  stream->flag = O_RDWR | O_CREAT | O_TRUNC;
		  }	  
	  }
	  else if (mode[1] == '+')        // w+  w+b
	  {
		  stream->flag = O_RDWR | O_CREAT | O_TRUNC;
	  }
	  break;
  case 'a':
	  if (mode[1] == '\0')            // a
	  {
		  stream->flag = O_WRONLY | O_CREAT | O_APPEND;
	  } 
	  else if (mode[1] == 'b')
	  {
		  if (mode[2] == '\0')          // ab
		  {
			  stream->flag = O_WRONLY | O_CREAT | O_APPEND;
		  }  
		  else if (mode[2] == '+')      // ab+
		  {
			  stream->flag = O_RDWR | O_CREAT | O_APPEND;
		  }	  
	  }
	  else if (mode[1] == '+')        // a+  a+b
	  {
		  stream->flag = O_RDWR | O_CREAT | O_APPEND;
	  } 
	  break;
  }
  
  mode_t open_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

  if ((stream->fd = open(path, stream->flag, open_mode)) == -1) 
  {
	  delete stream;
	  printf("fopen failed\n");
	  stream = NULL;
  }
  
  return stream;
}

//-----------------------------------------------------------------------------
//   fpurge(): purge a stream. 
//-----------------------------------------------------------------------------
//  Desc: The  function  fpurge()  clears  the buffers of the given stream.
//     data remains in the stream at the memory location but is inaccesable
//     from the program
//  Pre: stream exists
//  Post: upon sucess, this function returns zero, otherwise returns -1
int fpurge(FILE *stream)
{
   // reinitialize buffer
   if (stream == NULL)
      return -1;

   stream->pos = 0;
   stream->actual_size = 0;

   // for security reasons, buffer set to null char
   //memset(stream->buffer, '\0', stream->size); 

	return 0;
}

//-----------------------------------------------------------------------------
//   fflush(): flush a stream. 
//-----------------------------------------------------------------------------
/* Desc: If the given stream was open for writing (or if it was open for 
      updating and the last i/o operation was an output operation) any 
      unwritten data in its output buffer is written to the file.

      If stream is a null pointer, all such streams are flushed.

      Flushing a stream open for reading causes its input buffer to be cleared
      The stream remains open after this call.

   Pre: Stream exits
   Post: upon sucess, this function returns zero, otherwise returns EOF */
int fflush(FILE *stream) 
{
   if(stream == NULL || stream->buffer == (char *) 0) 
      return EOF;

   if(stream->lastop == 'w' && stream->actual_size != 0)
   { // last op was 'w', unwritten data in buffer writen to file
      int writeSz = stream->actual_size - stream->pos; 
      write(stream->fd, &stream->buffer[stream->pos], writeSz);
   } 

   fpurge(stream);

	return 0;
}

//-----------------------------------------------------------------------------
//   fillBuffer(): read from FILE fd and fill buffer fully. 
//-----------------------------------------------------------------------------
/* Desc: 
      Fills buffer from current file position within stream object member [fd]
   Pre:
      Stream exists with properly assigned fd an
   Post:
      returns EOF [-1] on read error, otherwise returns the number of bytes
      read into the buffer
*/
int fillBuffer(FILE *stream)
{
   
   if (stream == NULL || stream->fd < 0)
      return EOF;
   // refill buffer
   stream->actual_size = read(stream->fd, stream->buffer, stream->size);
   stream->pos = 0;

   // failed read call or end of file
   if(stream->actual_size <= 0) 
   {
      stream->eof = (stream->actual_size == 0) ? true : false;
      return EOF;
   }

   // no errors - reset position and size
   stream->eof = false;
   return stream->actual_size;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) ;

//-----------------------------------------------------------------------------
//   fgetc(): get character from stream. 
//-----------------------------------------------------------------------------
/* Desc: Returns the character currently pointed by the internal file position
      indicator of the specified stream. The internal file position indicator 
      is then advanced to the next character.

      If the stream is at the end-of-file when called, the function returns EOF
      and sets the end-of-file indicator for the stream (feof).

      If a read error occurs, the function returns EOF and sets the error 
      indicator for the stream (ferror).
   Pre:  Stream exists
   Post: On success, the character read is returned (promoted to an int value).
         The return type is int to accommodate for the special value EOF, which
         indicates failure:
         If the position indicator was at the end-of-file, the function returns
         EOF and sets the eof indicator (feof) of stream. */
int fgetc(FILE *stream) 
{
   // invalid input
   if(stream == NULL || stream->fd < 0 || stream->flag == 577
      || stream->flag == 1089 )
      return EOF;

   if(stream->lastop == 'w') // flush stream if unwritten data
      fflush(stream);
   
   stream->lastop = 'r';

   // re-fill buffer if needed
   if(stream->pos >= stream->actual_size)
   {
      // fills buffer and resets position in buffer
      fillBuffer(stream); // will flip eof flag if nothing to fill

      if (stream->eof)
         return EOF;
   }
   // get character and move position pointer in buffer
   int ch = stream->buffer[stream->pos++];
   return ch;

   // char *ch;
   // size_t readBytes = 0;
   // readBytes = fread(ch, 1, 1, stream);
	// return (readBytes > 0) ? (int) *ch : EOF;
}

//-----------------------------------------------------------------------------
//   fgets(): Get string from stream. 
//-----------------------------------------------------------------------------
/* Desc:  Reads characters from stream and stores them as a C string into str 
      until (num-1) characters have been read or either a newline or the 
      end-of-file is reached, whichever happens first.

      A newline character makes fgets stop reading, but it is considered a
      valid character by the function and included in the string copied to str.

      A terminating null character is automatically appended after the
      characters copied to str.
   Pre: size >= 0, stream exists, str is not nullptr
   Post: On success, the function returns str. If the end-of-file is
      encountered while attempting to read a character, the eof indicator is
      set (feof). If this happens before any characters could be read, the 
      pointer returned is a null pointer (and the contents of str remain 
      unchanged).

      If a read error occurs, the error indicator (ferror) is set and a null 
      pointer is also returned (but the contents pointed by str may have 
      changed).*/
char *fgets(char *str, int size, FILE *stream) 
{
   // check for invalid input or improper file access
   if ( str == nullptr || stream == NULL || size < 0 || stream->fd < 0 
      || stream->buffer == NULL || stream->flag == 577
      || stream->flag == 1089)
      return NULL;

   int strPos = 0; // tracking number of char's written to str
   
   for (int i = 0; i < size; i++)
   {
      int ch = fgetc(stream);

      if (ch == EOF)
         break;
      
      // get char and move string position pointer
      str[strPos++] = ch;

      if (ch == '\n') // include newlines, break after thouhg
         break;
   }

   if(strPos == 0) // read error, don't return string
      return NULL;

   str[strPos] = '\0'; // terminate string
	return str;
}

//-----------------------------------------------------------------------------
//   fread(): binary stream Input. 
//-----------------------------------------------------------------------------
/* Desc: Reads an array of count elements, each one with a size of size bytes,
      from the stream and stores them in the block of memory specified by ptr.

      The position indicator of the stream is advanced by the total amount of 
      bytes read.

      The total amount of bytes read if successful is (size*count).
   Pre: non-null ptr passed, stream exists, nmemb and size > 0, *ptr has space
      to store requested number of bytes
   Post: The total number of elements successfully read is returned. 
      If this number differs from the count parameter, either a reading error 
      occurred or the end-of-file was reached while reading. 
      If either size or count is zero, the function returns zero and both the 
      stream state and the content pointed by ptr remain unchanged.*/
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
   // check for invalid inputs or write only files
   if ( ptr == NULL || stream == NULL || size < 1 || nmemb < 1 
      || stream->fd < 0 || stream->buffer == NULL || stream->eof 
      || stream->flag == 577 || stream->flag == 1089)
      return 0;

   // flush buffer if any unwritten data
   if(stream->lastop == 'w')
      fflush(stream);

   int bytesRequested = size * nmemb; // total size

   // void ptrs for tracking position in buffer and client buffer
   void *readPos = (void *) &(stream->buffer[stream->pos]);
   // duplicate write pointer with char for incrememnting
   char *charPtr = (char *) ptr; 
   void *clientPos = charPtr; // void for use of memcpy
   int bytesWritten = 0; // tracking total bytes written
   int readSize = stream->actual_size - stream->pos; // single iteration size

   // refill buffer if empty
   if(readSize == 0)
   {
      readSize = fillBuffer(stream);
      readPos = stream->buffer; // don't forget to set ptr back to start
   }

   stream->lastop = 'r';

   // read full buffers, loop through until eof or over requested size
   while(readSize > 0 && !stream->eof 
      && bytesWritten + readSize < bytesRequested) 
   {
      memcpy(clientPos, readPos, readSize); // copy to client buffer

      // track position along client buffer with void* offset
      bytesWritten += readSize;
      charPtr = &charPtr[readSize];
      clientPos = (void *) charPtr; // where to write to next

      // refill buffer, advance file ptr, flip eof flag if needed
      readSize = fillBuffer(stream);
      readPos = stream->buffer; // set ptr back to start of buffer
   }

   // test for exact position and empty buffer, grab last bit of data if need
   if (bytesWritten != bytesRequested && !stream->eof) 
   {
      readSize = bytesRequested - bytesWritten; // determine size of final read

      memcpy(clientPos, readPos, readSize); // copy to client
      charPtr = &charPtr[readSize]; // still track location
      clientPos = (void *) charPtr; 
      stream->pos += readSize;
      bytesWritten += readSize;
   }

   // didn't write full buffer, indicate end of string
   if(bytesWritten < bytesRequested)
      *charPtr = '\0';

   return bytesWritten;
}

//-----------------------------------------------------------------------------
//   fputc(): Write character to stream. 
//-----------------------------------------------------------------------------
/* Desc: Writes a character to the stream and advances the position indicator.
      The character is written at the position indicated by the internal
      position indicator of the stream, which is then automatically advanced by
      one. If buffer fills, the function will write current buffer and empty.
   Pre: stream exists
   Post: On sucess, the character written is returned. If a writing error
      occurs, EOF is returned */
int fputc(int c, FILE *stream) 
{
   if(stream == NULL || stream->fd < 0 || stream->flag == 0 
      || stream->flag == 2)
      return EOF;

   // if last buffer used for reading, clear, if buffer is full, write to file
   if(stream->lastop == 'r' || stream->actual_size >= stream->size)
      fflush(stream);

   // store character, move position one
   stream->buffer[stream->actual_size++] = c; // store character in buffer
   stream->lastop = 'w';

	return c;
}



//-----------------------------------------------------------------------------
//   fputs(): Write string to stream. 
//-----------------------------------------------------------------------------
/* Desc: Writes the C string pointed by str to the stream.
      The function begins copying from the address specified (str) until it 
      reaches the terminating null character ('\0'). This terminating 
      null-character is not copied to the stream.
   Pre: non-nullptr str and stream exits
   Post: On success, a non-negative value is returned.
      On error, the function returns -1 .*/
int fputs(const char *str, FILE *stream) 
{
	// complete it
   if ( str == NULL || stream->fd < 0 
      || stream->buffer == NULL || stream->flag == 0 || stream->flag == 2) 
      return -1;

   int strPos = 0;
   int ch = str[strPos];

   while (ch != '\0')
   {
      if (fputc(ch, stream) == -1)
         break;
      
      ch = str[++strPos];
   }
   
	return 0;
}

//-----------------------------------------------------------------------------
//   fwrite(): write block of data to stream. 
//-----------------------------------------------------------------------------
/* Desc: Writes an array of count elements, each one with a size of size
      bytes, from the block of memory pointed by ptr to the current position in
      the stream.

      The position indicator of the stream is advanced by the total number of
      bytes written.

      Internally, the function interprets the block pointed by ptr as if it was
      an array of (size*count) elements of type unsigned char
   Pre: non-nullptr (*ptr), size and nmemb > 0, stream exits
   Post: The total number of elements successfully written is returned.
      If this number differs from the count parameter, a writing error
      prevented the function from completing. 

      If either size or count is zero, the function returns zero */
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
   // check for invalid args
   if ( ptr == NULL || size < 1 || nmemb < 1 
      || stream->fd < 0 || stream->buffer == NULL || stream->flag == 0 
      || stream->flag == 2)
      return 0;

   int bytesRequested = size * nmemb;
   int bytesWritten = 0;

   if(stream->lastop == 'r' || stream->actual_size == stream->size)
      fflush(stream); // flush any previously written data

   // cast void ptr to char pointer for incrememtning
   char *inBuff = (char*) ptr;
   // void pointers for memcpy
   void *clientPos = (void *) ptr;
   // actual size tracks the position of meaningful data in the write buffer
   void *processPos = (void *) &stream->buffer[stream->actual_size]; 
   int writeSize = bytesRequested;

   stream->lastop = 'w'; // indicate last use of buffer was for writing

   // copy over string data based on requested size, only flush when full
   for(int i = (bytesRequested / stream->size); i >= 0; i--)
   {
      // don't overflow buffer
      writeSize = (writeSize < stream->size) ? writeSize : stream->size; 
      memcpy(processPos, clientPos, writeSize);

      // track stream size for proper output (pos stays at 0)
      stream->actual_size += writeSize;
      bytesWritten += writeSize; // total bytes written
  
      inBuff = &inBuff[writeSize]; // increment char ptr
      clientPos = (void *) inBuff;  // cast to void ptr
      writeSize = bytesRequested - bytesWritten; // update new write size
      
      if(stream->actual_size >= stream->size)
         fflush(stream); // write data to file, advance file ptr
      else
         processPos = &stream->buffer[stream->actual_size]; 
      // update ptr to end of buffer
   }

   return bytesWritten;
}

//-----------------------------------------------------------------------------
//   feof(): Check end-of-file indicator. 
//-----------------------------------------------------------------------------
/* Desc: Checks whether the end-of-File indicator associated with stream is
      set, returning a value different from zero if it is.

      This indicator is generally set by a previous operation on the stream 
      that attempted to read at or past the end-of-file.

      Notice that stream's internal position indicator may point to the 
      end-of-file for the next operation, but still, the end-of-file indicator 
      may not be set until an operation attempts to read at that point.

      This indicator is cleared by a call to fseek 
      Although if the position indicator is not repositioned by 
      such a call, the next i/o operation is likely to set the indicator again.
   Pre: stream exits
   Post: A non-zero value is returned in the case that the end-of-file 
         indicator associated with the stream is set. Otherwise, zero is 
         returned.*/
int feof(FILE *stream) 
{
	if(stream == NULL)
      return 0;
   return stream->eof == true;
}

//-----------------------------------------------------------------------------
//   fseek(): Reposition stream position indicator. 
//-----------------------------------------------------------------------------
/* Desc: Sets the position indicator associated with the stream to a new 
      position.

      The new position is defined by adding 
      offset to a reference position specified by whence.

      If the function is called with other values for these arguments, support 
      depends on particular system and library implementation (non-portable).

      The end-of-file internal indicator of the stream is cleared after a 
      successful call to this function

      On streams open for update (read+write), a call to fseek allows to switch
      between reading and writing.
   Pre: stream exists
   Post: If successful, the function returns zero. Otherwise, it returns 
      non-zero value. */
int fseek(FILE *stream, long offset, int whence) 
{
   // flush any unwritten data in buffer
   fflush(stream); 

   if (lseek(stream->fd, offset, whence) == -1)
      return -1;

   stream->eof = false;

	return 0;
}

//-----------------------------------------------------------------------------
//   fclose(): close file. 
//-----------------------------------------------------------------------------
/* Desc:  Closes the file associated with the stream and disassociates it.
      all buffers associated with the stream are disassociated from it 
      and flushed: the content of any unwritten output buffer is written and 
      the content of any unread input buffer is discarded.

      Even if the call fails, the stream passed as parameter will no longer be 
      associated with the file nor its buffers.

   Pre: stream exists
   Post: If the stream is successfully closed, a zero value is returned.
      On failure, EOF is returned.*/
int fclose(FILE *stream) 
{
	// complete it
   if(stream == nullptr || stream->fd < 0)
      return EOF;

   if (stream->lastop == 'w')
      fflush(stream);

   close(stream->fd);

   // client handles otherwise
   if(stream->bufown && stream->buffer != nullptr)
      delete[] stream->buffer;

   delete stream;

	return 0;
}