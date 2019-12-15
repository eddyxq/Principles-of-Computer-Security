#include <stdio.h>

const char * filename = "./attacker-controlled-file.txt";

int main() {
    // a 64-byte buffer to store some user-provided input to be used later on...
    char buf[64];

    // this is the file that contains said user-provided input
    FILE * fin = fopen(filename, "r");

    if (!fin)
    {
    	perror(filename);
    	return 1;
    }

    // the first sizeof(int) bytes in fin specify how many additional bytes
    // there are in the file
    size_t len;
    fread((void *)&len, sizeof(size_t), 1, fin);

    printf("Reading %lu bytes into %lu-byte buffer...",
        len, sizeof(buf));
    size_t bytes_read = fread((void *)buf, 1, len, fin);
    if (bytes_read != len)
    {
    	printf("\033[1m\033[31mFOUND %lu BYTES\033[0m\n", bytes_read);
    }
    else
    {
    	printf("\033[1m\033[32mDONE!\033[0m\n");
    }

    //...do something with the output

//    fclose(fin);

    return 0;
}
