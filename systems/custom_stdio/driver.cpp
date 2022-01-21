#include "stdio.h"     // my stdio.h file
#include <sys/stat.h>  
#include <sys/types.h> 
#include <sys/uio.h>   
#include <unistd.h>
#include <string.h>

#define BUFSIZE  4096
#define DATASIZE 131072
using namespace std;

const char *contents =
	"abcdefghijklmnopqrstuvwxyz1234567890~!@#$%^&*()-+={}|:;',./?ABC\n\0";
const char *addition =
	"___append___append___append___append___append___append___append\n\0";

char *init_data() 
{
	char *data = new char[DATASIZE];
	for (int i = 0; i < DATASIZE; i += 64)  // 2048 repetitions
	{
		strncpy(data + i, contents, 64);
	}	
	return data;
}

int main(int argc, char *argv[]) 
{
	// validate the arguments
	if ( argc != 2 ) 
	{
		printf( "usage: driver filename\n" );
		return -1;
	}

	// fopen test
	printf("Testing fopen(r)...\n");
	FILE *file_r = fopen(argv[1], "r");
	struct stat file_state;
	int retval = fstat(3, &file_state);
	printf( "fstat = %d, file size = %d\n", retval, file_state.st_size );
	
	printf("Testing feof...\n");
	int eof = feof(file_r);
	printf("EOF = %d\n", eof); 

	// setvbuf test
	printf( "Testing setvbuf...\n" );
	char vbuf[40];
	setvbuf( file_r, vbuf, _IOFBF, 40 );

	char buf[100];
	int nRead;
	// fread test
	printf("Testing fread...\n");
	while ((nRead = fread(buf, 1, 100, file_r)) > 0)
	{
		write(1, buf, nRead);
	}
	
	// fseek test
	printf("Testing fseek...\n");
	fseek(file_r, 0, SEEK_SET);

	// fgets test
	printf("Testing fgets...\n");
	while (fgets(buf, 100, file_r) != NULL)
	{
		write(1, buf, strlen(buf));
	}
	
	// fgetc test
	printf( "Testing fgetc...\n" );
	fseek(file_r, -file_state.st_size, SEEK_END);
	char c;
	while ((c = fgetc(file_r)) != EOF)
	{
		write(1, &c, 1);
	}
	
	// feof test
	printf("Testing feof...\n");
	eof = feof(file_r);
	printf("EOF = %d\n", eof); 
	
	// fclose test
	printf("Testing fclose...\n");
	fclose(file_r);
	retval = fstat(3, &file_state);
	printf("fstat = %d\n", retval);

	// fopen test
	printf("Testing fopen(w)...\n");
	FILE *file_w1 = fopen("test1.txt", "w");
	FILE *file_w2 = fopen("test2.txt", "w");
	FILE *file_w3 = fopen("test3.txt", "w");
	
	for (int i = 3; i <=5; i++) 
	{
		retval = fstat(i, &file_state);
		printf( "fd = %d, fstat = %d, file size = %d\n", i, retval, file_state.st_size );
	}
	
	char *data = init_data();
	
	// fwrite test
	printf("Testing fwrite...\n");
	for (int i = 0; i < DATASIZE; i += BUFSIZE)
	{
		fwrite(data + i, sizeof(char), BUFSIZE, file_w1);
	}
	
	// fputs test
	printf( "Testing fputs...\n" );
	for (int i = 0; i < DATASIZE; i += 64) 
	{
		fputs(contents, file_w2);
	}
	
	// fputc test
	printf("Testing fputc...\n");
	for (int i = 0; i < DATASIZE; i++)
	{
		fputc(data[i], file_w3);
	}
	
	// fclose test
	printf("Testing fclose...\n");
	fclose(file_w1);
	fclose(file_w2);
	fclose(file_w3);
	for (int i = 3; i <=5; i++) 
	{
		retval = fstat(i, &file_state);
		printf("d = %d, fstat = %d\n", i, retval);
	}
	
	// fopen test
	printf("Testing fopen(a+)...\n");
	FILE *file_w4 = fopen("test3.txt", "a+");
	retval = fstat(3, &file_state);
	printf("fstat = %d, file size = %d\n", retval, file_state.st_size);
	
	fwrite(addition, sizeof(char), 64, file_w4);
	fseek(file_w4, -128, SEEK_END);
	
	while (fgets(buf, 100, file_w4) != NULL)
	{
		printf(buf);
	}
	
	// fclose test
	printf("Testing fclose...\n");
	fclose(file_w4);
	retval = fstat(3, &file_state);
	printf("d = %d, fstat = %d\n", 3, retval);
	
	return 0;
}
