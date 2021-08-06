#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int a=0;

void encerra_a(int sinal){
    printf("Encerrando a execuçao\n");
    exit(15);
}

void reinicia_a(int sinal) {
    printf("Reiniciando o valor de a...\n");
    a=0;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main(void){
    signal(15,encerra_a);
    signal(2, reinicia_a);

    printf("PID=%d\n", getpid());

    while(1){
        printf("a=%d\n",a);
        a++;
        sleep(3);
    }
}
#pragma clang diagnostic pop

