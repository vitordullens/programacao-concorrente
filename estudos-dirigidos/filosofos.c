/*
    Vitor Fernandes Dullens
    16/0148260
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5
#define ESQ(id) (id+N-1)%N
#define DIR(id)	(id+1)%N
#define PENSANDO 0
#define FAMINTO 1
#define COMENDO 2

int estados[N];

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
sem_t s[N];

void *teste (int id) {
	if(estados[id] == FAMINTO && estados[ESQ(id)] != COMENDO && estados[DIR(id)] != COMENDO) {
		estados[id] = COMENDO;
		sem_post(&s[id]);
	}
}

void *pega_garfos (int id) {
	pthread_mutex_lock(&lock);
	estados[id] = FAMINTO;
	teste(id);
	pthread_mutex_unlock(&lock);
	sem_wait(&s[id]);
} 


void *libera_garfos (int id) {
	pthread_mutex_lock(&lock);
	estados[id] = PENSANDO;
	teste(ESQ(id));
	teste(DIR(id));
	pthread_mutex_unlock(&lock);
}

void *filosofos (void *data) {
	int id = (long) data;
	while(1) {
		printf("%d pensando ¯\\_( ͡° ͜ʖ ͡°)_/¯\n\n", id);
		pega_garfos(id);
        sleep(2);
		printf("----- %d comendo (ˆڡˆ)\n\n", id);
        printf("----- %d acabou de comer ( ͡° ͜ʖ ͡°)\n\n", id);
		libera_garfos(id);
        sleep(2);
    }
}

int main () {
	long i;

	for (i=0; i<N; i++) {
		sem_init(&(s[i]), 0, 0);
	}

	pthread_t r[N]; 

        for (i = 0; i < N ; i++) {
	     pthread_create(&r[i], NULL, filosofos, (void*) i);
	    }

         pthread_join(r[0],NULL);	
  
	return 0;
}