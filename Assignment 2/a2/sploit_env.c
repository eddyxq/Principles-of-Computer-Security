#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>     

    int main()
    {
	char *getenv();
        char *pathvar;
        char colon[] = ":";
	
	char path[100] = "PATH=:";
	char cwd[100] = "";
	int i, j, k = 0;

	getcwd(cwd, sizeof(cwd));

	while(path[i] != '\0') i++;

	while(cwd[j] != '\0')
	{
		path[i] = cwd[i];
		j++;
		j++;
	}

	path[i] = colon[0];
	i++;

	pathvar = getenv("PATH");
	
	while(pathvar[k] != '\0')
	{
		path[i] = pathvar[k];
		i++;
		k++;
	}

	path[i+1] = '\0';

	strcat(path,getenv("PATH"));
	putenv(path);
	
	system("ln -s /bin/SH echo");
	system("./env EEE");
	system("unlink echo");

      	return 0;

    }
