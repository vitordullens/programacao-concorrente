/*
    Vitor Fernandes Dullens
    16/0148260
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 20

int matrix[SIZE][SIZE];
int soma[SIZE];

pthread_t worker[SIZE];
pthread_barrier_t barreira;

void matrix_init(){
    int i, j;
    for(i=0; i<SIZE; i++)
        for(j=0;j<SIZE;j++)
            matrix[i][j] = 1;
}

void * somador(void * data){
    int id = (long) data;

    printf("trabalhador %d comecou\n", id);
    int i, j;
    int sum_linha = 0, total = 0;
    for(i=0; i<SIZE; i++)
            sum_linha += matrix[id][i];
    soma[id] = sum_linha;
    printf("-------- trabalhador %d soma[%d] = %d\n", id, id, soma[id]);
    pthread_barrier_wait(&barreira);
    if(id == 0){
        sum_linha = 0;
        for(i=0; i<SIZE; i++)
            total += soma[i];

        printf("O total eh %d\n", total);
    }
    pthread_exit(0);
}

int main(){
    
    pthread_barrier_init(&barreira, NULL, SIZE);
    
    matrix_init();
    
    long i;

    for(i = 0; i<SIZE; i++){
        pthread_create(&worker[i], NULL, somador, (void *) i);
    }
    for(i =0; i< SIZE; i++){
        pthread_join(worker[i], NULL);
    }
    printf("acabou\n");
}