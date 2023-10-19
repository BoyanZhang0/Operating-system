#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    double x = atof(argv[1]);
    int n = atoi(argv[2]);

    pid_t pid = fork();
    
    if(pid == 0)
    {
        char n_str[10];
        sprintf(n_str, "%d", n);
        execl("./file2", "file2", argv[1], n_str, NULL);
        fprintf(stderr, "Exec failed.\n");
        return 1;
    }
    else
    {
        wait(NULL);
        printf("Parent(PID=%d): finished\n", getpid());
    }

    return 0;
}