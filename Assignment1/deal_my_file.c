#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/wait.h>

# define length 100
int main()
{
    FILE* file;
    char line[length];
    pid_t pid;
    int i;

    file = fopen("my_file.txt", "r");

    int file_lock = fileno(file);
    flock(file_lock, LOCK_EX);

    for(i = 1; i <= 4; i++)
    {

        pid = fork();
        if(pid == 0)
        {
            fseek(file, 0, SEEK_SET);
            for(int j = 1; j < i; j++)
            {
                if(fgets(line, length, file) == NULL){}
            }

           if(fgets(line, length, file) != NULL)
           {
                printf("Child %d read: %s \n", i, line);
           }
        }
        flock(file_lock, LOCK_UN);
    }
    for(i = 0; i < 4; i++)
    {
        wait(NULL);
    }
    fclose(file);

    return 0;
}