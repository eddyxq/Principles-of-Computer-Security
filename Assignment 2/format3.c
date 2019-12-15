#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s MESSAGE\n", argv[0]);
        return 1;
    }
    const char * message = argv[1];

    char * allowed_chars = (char *)malloc(62);
        allowed_chars[0]  = 'a'; allowed_chars[1]  = 'A';
        allowed_chars[2]  = 'b'; allowed_chars[3]  = 'B';
        allowed_chars[4]  = 'c'; allowed_chars[5]  = 'C';
        allowed_chars[6]  = 'd'; allowed_chars[7]  = 'D';
        allowed_chars[8]  = 'e'; allowed_chars[9]  = 'E';
        allowed_chars[10] = 'f'; allowed_chars[11] = 'F';
        allowed_chars[12] = 'g'; allowed_chars[13] = 'G';
        allowed_chars[14] = 'h'; allowed_chars[15] = 'H';
        allowed_chars[16] = 'i'; allowed_chars[17] = 'I';
        allowed_chars[18] = 'j'; allowed_chars[19] = 'J';
        allowed_chars[20] = 'k'; allowed_chars[21] = 'K';
        allowed_chars[22] = 'l'; allowed_chars[23] = 'L';
        allowed_chars[24] = 'm'; allowed_chars[25] = 'M';
        allowed_chars[26] = 'n'; allowed_chars[27] = 'N';
        allowed_chars[28] = 'o'; allowed_chars[29] = 'O';
        allowed_chars[30] = 'p'; allowed_chars[31] = 'P';
        allowed_chars[32] = 'q'; allowed_chars[33] = 'Q';
        allowed_chars[34] = 'r'; allowed_chars[35] = 'R';
        allowed_chars[36] = 's'; allowed_chars[37] = 'S';
        allowed_chars[38] = 't'; allowed_chars[39] = 'T';
        allowed_chars[40] = 'u'; allowed_chars[41] = 'U';
        allowed_chars[42] = 'v'; allowed_chars[43] = 'V';
        allowed_chars[44] = 'w'; allowed_chars[45] = 'W';
        allowed_chars[46] = 'x'; allowed_chars[47] = 'X';
        allowed_chars[48] = 'y'; allowed_chars[49] = 'Y';
        allowed_chars[50] = 'z'; allowed_chars[51] = 'Z';
        allowed_chars[52] = '0'; allowed_chars[53] = '1';
        allowed_chars[54] = '2'; allowed_chars[55] = '3';
        allowed_chars[56] = '4'; allowed_chars[57] = '5';
        allowed_chars[58] = '6'; allowed_chars[59] = '7';
        allowed_chars[60] = '8'; allowed_chars[61] = '9';

    char buf[64];
    snprintf(buf, sizeof(buf), "Checking \"%.31s\" against whitelist...", message);
    printf(buf);

    const char *c = message;
    while (*c)
    {
        if (!strchr(allowed_chars, *c))
        {
            printf("\033[1m\033[31mFOUND '%c'!\033[0m\n", *c);
            return 1;
        }
        c++;
    }
    printf("\033[1m\033[33mDONE!\033[0m\n");

    snprintf(buf, sizeof(buf), "/bin/echo %s", message);
    printf("ECHO: "); fflush(stdout);
    system(buf);

    free(allowed_chars);

    return 0;
}
