/*
    Vitor Fernandes Dullens
    16/0148260
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define PRODUTORES 3
#define CONSUMIDORES 3

#define ESPERA_CONSUMIDOR 1.5
#define ESPERA_PRODUTOR 1

pthread_mutex_t LOCK_BUFFER = PTHREAD_MUTEX_INITIALIZER;
sem_t cheio, vazio;

struct {
    int idx_consumidor;
    int idx_produtor;
    int valor[100];
} buffer;

int count = 0;
int size = 100;

void produzir(int val){
    pthread_mutex_lock(&LOCK_BUFFER);
    buffer.valor[buffer.idx_produtor] = val;
    buffer.idx_produtor = (buffer.idx_produtor + 1)%size;
    pthread_mutex_unlock(&LOCK_BUFFER);
}

void* produtor(void* id){
    int val;
    long id_produtor = (long) id;
    while(1){

        val = rand()%size;
        
        sem_wait(&vazio);
        printf("produtor %ld produziu %d\n", id_produtor, val);
        produzir(val);
        sem_post(&cheio);
        

        sleep(ESPERA_PRODUTOR);
    }
}

int consumir(){
    int val;
    pthread_mutex_lock(&LOCK_BUFFER);
    val = buffer.valor[buffer.idx_consumidor];
    buffer.idx_consumidor = (buffer.idx_consumidor + 1)%size;
    pthread_mutex_unlock(&LOCK_BUFFER);
    return val;
}

void* consumidor(void* id){
    int val;
    long id_consumidor = (long) id;
    while(1){
        
        sem_wait(&cheio);
        val = consumir();
        printf("consumidor %ld consumiu %d\n", id_consumidor, val);
        sem_post(&vazio);

        sleep(ESPERA_CONSUMIDOR);
    }
}

int main(){

    buffer.idx_consumidor = buffer.idx_produtor = 0;

    pthread_t consumidores[CONSUMIDORES];
    pthread_t produtores[PRODUTORES];
    sem_init(&vazio, 0, 100);
    sem_init(&cheio, 0, 0);

    for(long i = 0; i < PRODUTORES; i++){
        pthread_create(&produtores[i], NULL, produtor, (void*) i);
    }
    for(long i = 0; i < CONSUMIDORES; i++){
        pthread_create(&consumidores[i], NULL, consumidor, (void*) i);
    }
    pthread_join(consumidores[0], NULL);
}