#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main(void) {
    pid_t pid;
    int i;
    int j;

    printf("Qual o PID do processo?\n"); scanf("%d",&i);
    pid = (pid_t) i;

    printf("Qual sinal deseja enviar?\n"); scanf("%d",&j);

    while (j!=15) {
        if (j==2) {
            printf("Enviando sinal 2 ao processo %d\n",i);
            kill(pid,2);
            printf("Qual sinal deseja enviar?\n"); scanf("%d",&j);
        } else {
            printf("Sinal não compativel. Tente outro\n"); scanf("%d",&j);
        }
    }

    if (j==15) {
        printf("Enviando sinal 15 ao processo %d\n",i);
        kill(pid,15);
    }
}
