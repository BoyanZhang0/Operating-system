#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <unistd.h>

double calculate(double x, int n)
{
    double result = 1.0;
    double term = 1.0;

    for(int i = 1; i <= n; i++)
    {
        term *= x / i;
        result +=term;
    }

    return result;
}

int main(int argc, char *argv[])
{
    double x = atof(argv[1]);
    int n = atoi(argv[2]);

    double ex = calculate(x, n);
    printf("Child(PID=%d): for x=%.2f, first %d term come out %.7f\n", (int)getpid(), x, n, ex);
    return 0;
}