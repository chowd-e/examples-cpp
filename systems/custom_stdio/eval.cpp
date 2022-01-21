#include "stdio.h"  
#include <fcntl.h> 
#include <sys/time.h> 
#include <sys/types.h> 
#include <sys/uio.h> 
#include <unistd.h> 
#include <sys/stat.h> 
#include <stdlib.h> 
#include <string.h>   
using namespace std;

#define BUFSIZE 4096
#define DATASIZE 131072

struct timeval start, end;

void startTimer() 
{
	gettimeofday(&start, NULL);
}

void stopTimer(char rw, char iotype, char testcase, char* filename) 
{
	gettimeofday(&end, NULL);
	const char *str_rw = (rw == 'r') ? "Reads : " : (rw == 'w') ? "Writes: " : "Unknown";
	const char *str_iotype = (iotype == 'u') ? "Unix   I/O" : (iotype == 'f') ? "C File I/O" : "Unknown";
	const char *str_testcase =
    (testcase == 'a') ? "Read   once     ":
    (testcase == 'b') ? "Block  transfers" :
    (testcase == 'c') ? "Char   transfers"  :
    (testcase == 'r') ? "Random transfers" : "Unknown";
	
	printf(str_rw);
	printf( str_iotype );
	printf( " [" );
	printf( str_testcase );
	printf( "] = %d\n",(end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec ));
}

void reads(char iotype, char testcase, char *filename) 
{
	int fd = open(filename, O_RDONLY);
	if ( fd == -1 ) 
	{
		printf( "filename(" );
		printf( filename );
		printf( ") not found\n" );
		return;
	}
	FILE *file = (iotype == 'f') ? fopen(filename, "r") : NULL;
	char buffer[BUFSIZE];
	
	startTimer();
	if (testcase == 'a') 
	{
		struct stat fileStat;
		if (fstat(fd, &fileStat) >= 0 ) 
		{
			char wholeData[ fileStat.st_size ];
			if (iotype == 'u')
			{
				read(fd, wholeData, fileStat.st_size);
			}
			if (iotype == 'f')
			{
				fread(wholeData, sizeof(char), fileStat.st_size, file);
			}		
		}
	}
	else if (testcase == 'b') 
	{
		if (iotype == 'u')
		{
			while (read(fd, buffer, BUFSIZE) > 0);
		}
		if (iotype == 'f')
		{
			while (fread(buffer, sizeof(char), BUFSIZE, file) > 0);
		}
	}
	else if (testcase == 'c') 
	{
		if (iotype == 'u')
		{
			while (read(fd, buffer, 1) > 0);
		}
		if (iotype == 'f')
		{
			while (fgetc(file) != EOF);
		}
	}
	else if (testcase ==  'r') 
	{
		while (true) 
		{
			int retval = -1;
			char *retstr = (char *) -1;
			switch(rand() % 3) 
			{
			case 0: // char
				if (iotype == 'u')
				{
					retval = read(fd, buffer, 1);
				}
				if (iotype == 'f')
				{
					retval = fgetc(file);
				}
				break;
			case 1: // line
				if (iotype == 'u')
				{
					retval = read(fd, buffer, 80);
				}
				if (iotype == 'f')
				{
					retstr = fgets(buffer, 80, file);
				}
				break;
			case 2: // block
				if (iotype == 'u')
				{
					retval = read(fd, buffer, BUFSIZE);
				}	
				if (iotype == 'f')
				{
					retval = fread(buffer, sizeof(char), BUFSIZE, file);
				}
				break;
			}
			if (iotype == 'u' && retval <= 0)
			{
				break;
			}
			if (iotype == 'f' && (retval == 0 || retstr == NULL))
			{
				break;
			}
		}
	}
	else 
	{
		printf( "testcase not supported" );
	}
	
	stopTimer( 'r', iotype, testcase, filename );
	if (iotype == 'u')
	{
		close(fd);
	}
	if (iotype == 'f')
	{
		fclose(file);
	}
}

const char *contents = "abcdefghijklmnopqrstuvwxyz1234567890~!@#$%^&*()-+={}|:;',./?ABC\n\0";

char *init_data() 
{
	char *data = new char[DATASIZE];
	for (int i = 0; i < DATASIZE; i += 64)  // 2048 repetitions
	{
		strncpy(data + i, contents, 64);
	}
	return data;
}

void writes(char iotype, char testcase, char *filename) 
{
	int fd = (iotype == 'u') ? open( filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ) : -1;
	FILE *file = (iotype == 'f') ? fopen(filename, "w") : NULL;
	char *buffer = init_data();
	if (iotype == 'u' && fd == -1 || iotype == 'f' && file == NULL) 
	{
		printf( "filename(" );
		printf( filename );
		printf( "): write protected\n" );
		return;
	}

	startTimer();
	switch( testcase ) 
	{
	case 'a': // write once
		if (iotype == 'u')
		{
			write(fd, buffer, DATASIZE);
		}
		if (iotype == 'f')
		{
			fwrite(buffer, sizeof(char), DATASIZE, file);
		}
		break;
	case 'b': // block writes
		for ( int i = 0; i < DATASIZE; i += BUFSIZE ) // 32 repetitions
		{ 
			if (iotype == 'u')
			{
				write(fd, buffer + i, BUFSIZE);
			}
			if (iotype == 'f')
			{
				fwrite(buffer + i, sizeof(char), BUFSIZE, file);
			}
		}
		break;
	case 'c': // char writes
		for ( int i = 0; i < DATASIZE; i ++ ) 
		{ 
			if (iotype == 'u')
			{
				write(fd, buffer + i, 1);
			}
			if (iotype == 'f')
			{
				fputc(buffer[i], file);
			}
		}
		break;
	case 'r':
		int j;
		j = 0;
		int size;
		while (j < DATASIZE) 
		{
			switch(rand() % 3) 
			{
			case 0: // char
				for ( int k = 0; k < 64; k++ ) 
				{
					if (iotype == 'u')
					{
						write(fd, buffer + j + k, 1);
					}
					if (iotype == 'f')
					{
						fputc(buffer[j + k], file);
					}
				}
				j += 64;
				break;
			case 1: // line
				size = (DATASIZE - j >= 64) ? 64 : DATASIZE - j;
				if (iotype == 'u')
				{
					write(fd, buffer + j, size);
				}
				if (iotype == 'f')
				{
					fputs(contents, file);
				}
				j += size;
				break;
			case 2: // block
				size = (DATASIZE - j >= BUFSIZE) ? BUFSIZE : DATASIZE - j;
				if (iotype == 'u')
				{
					write(fd, buffer + j, size);
				}
				if (iotype == 'f')
				{
					fwrite(buffer + j, sizeof(char), size, file);
				}
				j += size;
				break;
			default:
				break;
			}
		}
		break;
	default:
		printf( "testcase not supported\n" );
		break;
	}
	if (iotype == 'f')
	{
		fflush(file);
	}
	stopTimer( 'w', iotype, testcase, filename );

	if (iotype == 'u')
	{
		close(fd);
	}
	if (iotype == 'f')
	{
		fclose(file);
	}
}

int main(int argc, char *argv[]) 
{
	// argument verification
	if ( argc != 5 ) 
	{
		printf("usage: eval r/w u|f a|b|c|r filename, where:\n");
		printf("r = read,     w = write\n");
		printf("u = unix i/o, f = c file i/o\n");
		printf("a = at once,  b = 4096B block,  c = 1B char,  r = random\n");
		return -1;
	}
	char rw = argv[1][0];
	char iotype = argv[2][0];
	char testcase = argv[3][0];
	char *filename = argv[4];

	if (iotype != 'u' && iotype != 'f') 
	{
		printf( "iotype(" );
		printf( argv[2] );
		printf( "): not supported\n" );
		return -1;
	}
	
	if (rw == 'r')
	{
		reads(iotype, testcase, filename);
	}
	else if (rw == 'w')
	{
		writes(iotype, testcase, filename);
	}
	else 
	{
		printf("rw(");
		printf(argv[1]);
		printf("): not supported\n");
	}
	return 0;
}
