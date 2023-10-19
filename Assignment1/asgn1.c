//
//  code for assignment 1
//  
//
//  Created by Bibhas Ghoshal on 26/08/20.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define ITR 100000

int main()
{
    int i,j,pid;
    
    pid = fork();
    
    if( pid == 0 ){
        
        for ( i=0;i<20;i++){
           for (j=0;j<ITR;j++);
           printf("Child:%d\n",i);
        }        
    }
    else{
        wait(NULL);
        for ( i=0;i<20;i++){
            for (j=0;j<ITR;j++);
            printf("Parent:%d\n",i);
        }
    }
}
