/*
    Vitor Fernandes Dullens
    16/148260
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#define EATERS 10
#define COOKER_RATE 10

#define COOKING_TIME 1
#define EATING_TIME 1

pthread_mutex_t LOCK = PTHREAD_MUTEX_INITIALIZER;;
pthread_cond_t SLEEP_C = PTHREAD_COND_INITIALIZER;
pthread_cond_t SLEEP_E = PTHREAD_COND_INITIALIZER;

struct {
    bool table[100];
    int plates;
} table;

void add_meals(int x){ 
    pthread_mutex_lock(&LOCK);
    int i;
    for(i = 0; i < x; i++){
        if(table.plates+i == 100){
            break;
        }
        table.table[table.plates+i] = true;
    }
    table.plates += i;
    printf("Adicionado %d pratos na mesa. Total: %d\n", i, table.plates);
    pthread_mutex_unlock(&LOCK);    
}

void eat_meal(){
    pthread_mutex_lock(&LOCK);
    printf("Pratos antes de comer: %d\n", table.plates);
    table.plates -= 1;
    table.table[table.plates] = false;
    printf("Prato removido. Total: %d\n", table.plates);
    pthread_mutex_unlock(&LOCK);        
}

void* cooker(void* data){
    while(true){
        printf("Cozinheiro cozinhando...\n");
        sleep(COOKING_TIME);
        pthread_mutex_lock(&LOCK);
        while(table.plates == 100){
            printf("Cozinheiro foi dormir zZzZ\n");
            pthread_cond_wait(&SLEEP_C, &LOCK);
        }
        pthread_mutex_unlock(&LOCK);
        add_meals(COOKER_RATE);
        pthread_mutex_lock(&LOCK);
        if(table.plates == COOKER_RATE){
            pthread_cond_broadcast(&SLEEP_E);
        }
        pthread_mutex_unlock(&LOCK);        
    }
}

void* eater(void* data){
    long id = (long) data;
    printf("Canibal %ld com fome!! :9\n", id);
    while(true) {
        pthread_mutex_lock(&LOCK);
        if(table.plates > 0){
            pthread_mutex_unlock(&LOCK);      
            printf("Canibal %ld comendo :D\n", id);      
            eat_meal();
            pthread_mutex_lock(&LOCK);
            if(table.plates == 0){
                pthread_cond_signal(&SLEEP_C);
            }
            pthread_mutex_unlock(&LOCK);    
        }
        pthread_mutex_unlock(&LOCK);    
        while(table.plates == 0){
            printf("Canibal %ld foi dormir zZzZ\n", id);
            pthread_cond_wait(&SLEEP_E, &LOCK);
        }
        pthread_mutex_unlock(&LOCK);
        sleep(EATING_TIME);        
    }
}

int main(){
    for(int i = 0; i < 100; i++){
        table.table[i] = false;
    }
    table.plates = 0;

    pthread_t cook;
    pthread_t eaters[EATERS];

    pthread_create(&cook, NULL, cooker, NULL);

    for(long i = 0; i < EATERS; i++){
        pthread_create(&eaters[i], NULL, eater, (void*) i);
    }

    pthread_join(cook, NULL);
}