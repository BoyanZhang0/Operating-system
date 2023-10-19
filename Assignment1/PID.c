#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> /* contains prototype for wait */
int main(void)
{
	int pid;
	int status;
	printf("Hello World!\n");
	pid = fork( );
	if (pid == -1) /* check for error in fork */
	{
		perror("bad fork");
		return 1;
	}
		if (pid == 0)
			printf("I am the child process.\n");
		else
		{
			pid_t terminatedd_pid = wait(&status); /* parent waits for child to finish */
			printf("pid is %d \n", terminatedd_pid);
            printf("I am the parent process.\n");
		}
	return 0;
}