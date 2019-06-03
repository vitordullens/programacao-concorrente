/*
    Vitor Fernandes Dullens
    16/0148260
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define SEATS 10
#define CLIENTS 15

sem_t barber, cut, turn, chair;

void* barbery(void* data){
    printf("OPEN\n");
    while(1){
        sem_wait(&barber);
        printf("CUTING...\t");
        sleep(1);
        printf("DONE\n");
        sem_post(&cut);
    }
}

void* client(void* data){
    long id = (long) data;
    while(1){
        int catch = sem_trywait(&turn);
        if(catch){
            printf("eu, %ld, to na fila...\n", id);
            sem_wait(&chair);
            printf("eu, %ld, indo cortar o cabelo\n", id);
            sem_post(&turn);
            sem_post(&barber);
            sem_wait(&cut);
            printf("eu, %ld, corteeeeei\n", id);
            sem_post(&chair);
            sleep(1);
        }
        else{
            printf("Vish ta lotado, eu, %ld, volto mais tarde\n", id);
            sleep(2);
        }

    }
}

int main(){
    sem_init(&barber, 0, 0);
    sem_init(&chair, 0, 1);
    sem_init(&cut, 0, 0);
    sem_init(&turn, 0, SEATS);

    pthread_t barbero, customer[CLIENTS];

    pthread_create(&barbero, NULL, barbery, NULL);

    for(long i=0; i < CLIENTS; i++){
        pthread_create(&customer[i], NULL, client, (void*) i);
    }

    pthread_join(barbero, NULL);
}