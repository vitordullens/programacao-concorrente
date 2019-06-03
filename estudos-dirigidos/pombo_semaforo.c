/*
    Vitor Fernandes Dullens
    16/0148260
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define USUARIOS 10

sem_t wait_pombo, writer;

int cartas = 0; 

void* pombo_correio(void* data){

    while(1){
        if(cartas < 20){
            sleep(1);
        }
        else{
            int catch = sem_trywait(&wait_pombo);
            if(catch){
                printf("Pombo: To indo de A pra B ...\n");
                sem_post(&wait_pombo);
                printf("Cheguei em A de novo hehe\n");
                cartas = cartas - 20;
                sleep(1);
            }
            else{
                printf("\tto no meio da viagem parceiro\n");
                sleep(2);
            }
        }
    }

}

void* user(void* data){
    long id = (long) data;

    while(1){
        sem_wait(&writer);
        cartas++;
        printf("%ld : estou escrevendo a cartinha %d\n", id, cartas);
        sleep(1);
        sem_post(&writer);
        sleep(5);
    }
}

int main(){
    sem_init(&wait_pombo, 0, 1);
    sem_init(&writer, 0, USUARIOS);

    pthread_t pombo, writers[USUARIOS];

    pthread_create(&pombo, NULL, pombo_correio, NULL);

    for(long i=0; i < USUARIOS; i++){
        pthread_create(&writers[i], NULL, user, (void*) i);
    }

    pthread_join(pombo, NULL);

}

