/*

Aluno: Vitor Fernandes Dullens
Matricula: 16/0148260

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 6

pthread_mutex_t lock_escrever, lock_ler;
pthread_t threads[NUM_THREADS];
int bd[5] = {1, 1, 1, 1, 1};

void *escritor(void *t){

    int my_id = (int)(long)t;

    printf("--- escritor %d comecou\n", my_id);

    for(int i = 0; i < 10; i++){

        int id_bd = rand()%5;
        int valor_bd = rand()%10;
        
        printf("Escrevendo bd[%d] - valor %d\n", id_bd, valor_bd);

        pthread_mutex_lock(&lock_escrever);
    	pthread_mutex_lock(&lock_ler);
	    
	    pthread_mutex_unlock(&lock_ler);
	    bd[id_bd] = valor_bd;
	    pthread_mutex_unlock(&lock_escrever);
	        sleep(1.5);

	    }

    printf("--- escritor %d terminou\n", my_id);

    pthread_exit(NULL);
}

void *leitor(void *t){

    int my_id = (int)(long)t;

    printf("--- leitor %d comecou\n", my_id-3);

    for(int i = 0; i < 10; i++){
    
        int id_bd = rand()%5;

        pthread_mutex_lock(&lock_escrever);
	    pthread_mutex_lock(&lock_ler);

	    pthread_mutex_unlock(&lock_ler);
	    pthread_mutex_unlock(&lock_escrever);
	    int valor_bd = bd[id_bd];
	    pthread_mutex_lock(&lock_ler);

	  
	    pthread_mutex_unlock(&lock_ler);

        printf("Lendo bd[%d] - valor %d\n", id_bd, valor_bd);
        sleep(1.5);

    }

    printf("--- leitor %d terminou\n", my_id-3);

    pthread_exit(NULL);

}


int main(){ 

    printf("Inicial => ");
    for(int i = 0; i < 5; i++)
        printf(" bd[%d] = %d |", i, bd[i]);
    printf("\n");
    sleep(2);

    pthread_mutex_init(&lock_ler, NULL);
    pthread_mutex_init(&lock_escrever, NULL);

    for(long i = 0; i < NUM_THREADS; i++){
        if(i < 3)
            pthread_create(&threads[(int)i], NULL, escritor, (void *)i);
        else
            pthread_create(&threads[(int)i], NULL, leitor, (void *)i);
    }

    for (int i = 0; i < NUM_THREADS; i++) 
        pthread_join(threads[i], NULL);

    printf("Final => ");
    for(int i = 0; i < 5; i++)
        printf(" bd[%d] = %d |", i, bd[i]);
    printf("\n");

    return 0;
}