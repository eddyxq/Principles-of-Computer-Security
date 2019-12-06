#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <signal.h>
#include <error.h>
#include "constants.h"
#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>
#include <vector>

unsigned char query_oracle(unsigned char ctbuf[], size_t ctlen, int ifd[2], int ofd[2])
{
    int status;
    pid_t pid = fork();
    if (pid == 0)
    { 
        // this is the child; it will simply exec into our desired
        // oracle program, but first it replaces stdin and stdout with
        // the above-created pipes so that the parent can both write to
        // and read from it
        dup2(ofd[0], STDIN_FILENO);
        dup2(ifd[1], STDOUT_FILENO);

        // ask kernel to deliver SIGTERM in case the parent dies
        prctl(PR_SET_PDEATHSIG, SIGTERM);

        // run the oracle
        execl(ORACLE, ORACLE, (char *)NULL);
        // we'll never get here, unless execl fails for some reason
        exit(1);
    }
    else
    {
        ssize_t bytes_written = write(ofd[1], &ctlen, sizeof(ctlen));
        bytes_written = write(ofd[1], ctbuf, ctlen);
		//printf("Wrote %lu bytes\n", bytes_written);
        unsigned char result = 0;
        ssize_t bytes_read = read(ifd[0], &result, 1);
		//printf("Oracle responded: %c\n", result);
        waitpid(pid, &status, 0);
        return result;
    }
}

//returns the last padding byte
unsigned char getLastByte(unsigned char ctbuf[], size_t ctlen, int ifd[2], int ofd[2])
{
	//check if padding byte is already 0x01
	ctbuf[ctlen-2] ^= 0x01;
	unsigned char response = query_oracle(ctbuf, ctlen, ifd, ofd);
	ctbuf[ctlen-2] ^= 0x01;
	if (response == 'M')
	{
		return 0x01;
	}
	
	unsigned char padding;
	
	//getting last byte
	for(int i = 1; i <= 16; i++)
	{	
		//xor with 1 to 16 until we get bad mac
		ctbuf[ctlen-1] ^= i; 
		//if it returns a MAC error then we know last byte in plaintext was flipped to 1
		if(query_oracle(ctbuf, ctlen, ifd, ofd) == 'M')
		{
			padding = i;
		}
		//xor again to restore to original value for next loop iteration
		ctbuf[ctlen-1] ^= i;
	}
	return padding ^ 1;
}

//returns the next last plaintext character
unsigned char nextLastByte(unsigned char ctbuf[], size_t ctlen, int ifd[2], int ofd[2], unsigned char padding, int arr[])
{
	//increment the current padding bytes
	for (int i = 0; i < padding; i++)
	{
		ctbuf[ctlen-1-i] = ctbuf[ctlen-1-i] ^ padding  ^ padding + 1;
	}
	
	//xor the last byte after padding with the padding
	ctbuf[ctlen-1-padding] ^= padding + 1;
	//brute-force until that last byte after padding is equal to padding
	for (int i = 0; i < 255; i++)
	{
		ctbuf[ctlen-1-padding] ^= arr[i];
		
		if (query_oracle(ctbuf, ctlen, ifd, ofd) == 'M')
		{
			//lastByte xor padding xor i = padding
			//xor padding on both sides
			//lastByte xor i = padding xor padding    (anything xor itself is 0)
			//lastByte xor i = 0
			//xor i on both sides
			//lastByte = 0 xor i                 (anything xor with 0 is itself)
			//lastByte = i
			return arr[i]; //therefore return i
		}
		
		//xor again to restore to original value for next loop iteration
		ctbuf[ctlen-1-padding] ^= arr[i];
	}
}

//decrypts a 16 byte block of plaintext
void solveNextBlock(unsigned char ctbuf[], size_t ctlen, int ifd[2], int ofd[2], int arr[], int i, int numBlocks, unsigned char ptbuf[])
{
	int blockNumber = numBlocks - (i / 16);
	
	int start = (16 * blockNumber) - 1;
	
	for(int j = 0; j < 16; j++)
	{
		unsigned char next = nextLastByte(ctbuf, ctlen - i, ifd, ofd, j, arr);
		ptbuf[start--] = next;
		printf("%c\n", next);
	}	
}

void solveLastBlock(unsigned char ctbuf[], size_t ctlen, int ifd[2], int ofd[2], int arr[], unsigned char ptbuf[], int BLOCK_SIZE, unsigned char padding, int index)
{
		//get the bytes in the block before the padding
		int remainingBytes = BLOCK_SIZE - padding;
		unsigned char next;
		for(int i = 0; i < remainingBytes; i++)
		{
			next = nextLastByte(ctbuf, ctlen, ifd, ofd, padding + i, arr);
			ptbuf[--index] = next;
			printf("%c\n", next);
		}
}

int main(int argc, char * argv[])
{
	//re-arranged numbers from 1 to 255 from most frequently used
	//characters to least used to optimize loop running times
	int arr[255] = {32, 101, 116, 97, 111, 105, 110, 115, 114, 104, 108, 100, 99, 117, 109, 102, 112, 103, 119, 121, 98, 118, 107, 120, 106, 113, 122, 44, 39, 46, 69, 84, 65, 79, 73, 78, 83, 82, 72, 76, 68, 67, 85, 77, 70, 80, 71, 87, 89, 66, 86, 75, 88, 74, 81, 90, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 96, 95, 94, 93, 92, 91, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 45, 43, 42, 41, 40, 38, 37, 36, 35, 34, 33};
	int BLOCK_SIZE = 16;
	ssize_t bytes_read;
	
	// read the ciphertext from a file
	unsigned char ctbuf[IVLEN + MACLEN + CTLEN] = { '\0' };
	//unsigned char ptbuf[IVLEN + MACLEN + CTLEN] = { '\0' };
	int ctfd =  open(CTFILE, O_RDONLY);
	if (ctfd == -1) error(1, errno, "opening %s", CTFILE);
	bytes_read = read(ctfd, ctbuf, IVLEN + MACLEN + CTLEN);
	if (bytes_read <= IVLEN + MACLEN) error(1, errno, "ciphertext too short");
	close(ctfd);

	// create some pipes for directional communication with the child process
	int ifd[2], ofd[2];
	if (pipe(ifd) != 0) error(1, errno, "creating input pipe");
	if (pipe(ofd) != 0) error(1, errno, "creating output pipe");

	// loop till we're done...
	size_t ctlen = bytes_read;
	int ptlen = bytes_read - MACLEN - IVLEN;
	unsigned char ptbuf[ptlen];
	
	//index used to assign plaintext characters to ptbuf
	int index = ptlen;

	//open file for writing
	FILE * fp = fopen ("plaintext.txt","w");
	
	int numBlocks = (bytes_read - MACLEN - IVLEN) / BLOCK_SIZE;
	
	bool done = false;
	while (!done)
	{
		//decrypt the padding byte
		unsigned char padding = getLastByte(ctbuf, ctlen, ifd, ofd);
		//converts char to int for calculations
		int padBytes = padding;
		
		//now we know the padding number of bytes is all padding
		//remove it because it is not required to be written
		index -= padBytes;
		
		//decrypt the last block with the padding
		solveLastBlock(ctbuf, ctlen, ifd, ofd, arr, ptbuf, BLOCK_SIZE, padding, index);
	
		//continue to decrypt remaining blocks in reverse order
		for(int i = BLOCK_SIZE; i < ptlen; i += BLOCK_SIZE)
		{
			solveNextBlock(ctbuf, ctlen, ifd, ofd, arr, i, numBlocks, ptbuf);
		}
		
		//write plaintext to file
		for(int i = 0; i < ptlen - padBytes; i++)
		{
			fprintf (fp, "%c", ptbuf[i]);
		}
		
		done = true;
	}
	
	//close resources
	close(ofd[0]);
	close(ofd[1]);
	close(ifd[0]);
	close(ifd[1]);
	fclose(fp);

	return 0;
}
