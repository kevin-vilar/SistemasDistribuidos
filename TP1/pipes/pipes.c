//bibliotecas utilizadas...
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define READ 0
#define WRITE 1

int fd[2];

int ultimo = 1;
void produtor(int fd[2], int conjunto){
    int qtd;
    int novo_numero;

    close(fd[READ]);

    for(qtd=0; qtd < conjunto; qtd++){
        novo_numero = numero_produtor(qtd);
        write(fd[WRITE], &novo_numero, sizeof(novo_numero));
    }
    write(fd[WRITE],0,sizeof(0));
    close (fd[WRITE]);
}

int numero_produtor(){
    int novo_numero;
    int aleatorio;
    srand (time(NULL) + ultimo );

    aleatorio = rand() % 100;
    novo_numero = aleatorio + ultimo;
    ultimo = novo_numero;

    printf(">> O número %d está em espera\n", novo_numero);

    return novo_numero;
}

void novo_produtor(int fd[2], int conjunto){
    int novo_processo;

    novo_processo = fork();
    if(novo_processo == -1){
        printf("Erro na criacao do processo produtor\n");
    }

    else if(novo_processo == 0){
        produtor(fd, conjunto);
        exit(0);
    }

    return;
}

void consumer(int fd[2]){
    int qtdade;
    int resp;
    int consomenumero;
    close (fd[WRITE]); /* Fecha o lado de leitura que nao eh utilizado*/

    while( resp != 0 ){

        resp = read(fd[READ], &consomenumero, sizeof(consomenumero));
        if(resp==-1){
            printf("Erro na leitura do pipe\n");
        }

        else if(resp==0){
            //printf("Pipe esta vazio... \n");
        }
        else{
            consumir_numero(&consomenumero);
        }
    }
    close (fd[READ]); /* Fecha o lado utilizado*/
}

void novo_consumidor(int fd[2]){
    int novo_processo;
    novo_processo = fork();

    if(novo_processo == -1){
        printf("Erro na criacao do processo consumidor\n");
    }
    else{
        consumer(fd);
        exit(0);
    }
}

void consumir_numero(int * t){
    int is_primo = is_prime(*t);
    if(is_primo == 0) {
        printf("O número %d não é primo\n", *t);
    } else {
        printf("O número %d é primo\n", *t);
    }
}

int is_prime(int numero){
    int i;
    for (i=2; i < numero; i++) {
        if (numero % i == 0 && i != numero) return 0;
    }
    return 1;
}

int main(){
    int conjunto ;
    printf("Quantos números quer gerar?\n");
    scanf("%i", &conjunto);
    pipe(fd);
    novo_produtor(fd, conjunto);
    novo_consumidor(fd);
    exit(0);
}