/*
    Vitor Fernandes Dullens
    16/0148260
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define PRODUTORES 3
#define CONSUMIDORES 3

#define ESPERA_CONSUMIDOR 1
#define ESPERA_PRODUTOR 2

pthread_mutex_t LOCK_BUFFER = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t LOCK_COUNT = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t DORMIR_CONSUMIDOR = PTHREAD_COND_INITIALIZER;
pthread_cond_t DORMIR_PRODUTOR = PTHREAD_COND_INITIALIZER;

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
        pthread_mutex_lock(&LOCK_COUNT);

        while (count == size){
            printf("produtor %ld dormiu\n", id_produtor);
            pthread_cond_wait(&DORMIR_PRODUTOR, &LOCK_COUNT);
        }
        count++;
        pthread_mutex_unlock(&LOCK_COUNT);
        printf("produtor %ld produziu %d\n", id_produtor, val);
        produzir(val);
        pthread_mutex_lock(&LOCK_COUNT);

        if(count <= CONSUMIDORES){
            pthread_cond_signal(&DORMIR_CONSUMIDOR);
        }
        pthread_mutex_unlock(&LOCK_COUNT);

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
        pthread_mutex_lock(&LOCK_COUNT);

        while (count == 0){
            printf("consumidor %ld dormiu\n", id_consumidor);
            pthread_cond_wait(&DORMIR_CONSUMIDOR, &LOCK_COUNT);
        }
        count--;
        pthread_mutex_unlock(&LOCK_COUNT);
        val = consumir();
        printf("consumidor %ld consumiu %d\n", id_consumidor, val);
        pthread_mutex_lock(&LOCK_COUNT);

        if(count >= size - PRODUTORES){
            pthread_cond_signal(&DORMIR_PRODUTOR);
        }
        pthread_mutex_unlock(&LOCK_COUNT);

        sleep(ESPERA_CONSUMIDOR);
    }
}

int main(){

    buffer.idx_consumidor = buffer.idx_produtor = 0;

    pthread_t consumidores[CONSUMIDORES];
    pthread_t produtores[PRODUTORES];

    for(long i = 0; i < PRODUTORES; i++){
        pthread_create(&produtores[i], NULL, produtor, (void*) i);
    }
    for(long i = 0; i < CONSUMIDORES; i++){
        pthread_create(&consumidores[i], NULL, consumidor, (void*) i);
    }
    pthread_join(consumidores[0], NULL);
}