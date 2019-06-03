/*
	Vitor Fernandes Dullens
	16/0148260
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define NUM 10

pthread_barrier_t barrier; 

void * p_AC (void * data){
	
	printf("A");
	pthread_barrier_wait(&barrier);
	pthread_barrier_wait(&barrier);
	printf("C");
	pthread_barrier_wait(&barrier);
	

}

void * p_BD (void * data){
	pthread_barrier_wait(&barrier);
	printf("B");
	pthread_barrier_wait(&barrier);
	pthread_barrier_wait(&barrier);
	printf("D");
	
}

int main(){
	pthread_t ac[NUM], bd[NUM];
	pthread_barrier_init(&barrier, NULL, 2*NUM);
	
	long i;
	for (i = 0; i < NUM; i++){
		pthread_create(&ac[i], NULL, p_AC, (void *) (i));
		pthread_create(&bd[i], NULL, p_BD, (void *) (i));
	}
	for (i = 0; i < NUM; i++){
		pthread_join(ac[i], NULL);
		pthread_join(bd[i], NULL);
	}
	
	
	return 0;
}