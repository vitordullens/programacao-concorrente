/*
  Aluno: Vitor Fernandes Dullens
  matricula: 16/0148260
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_DIREITA 10
#define N_ESQUERDA 10

pthread_mutex_t DIREITA;
pthread_mutex_t ESQUERDA;
pthread_mutex_t PONTE;

pthread_mutex_t TURNO;

int right = 0;
int left = 0;

void * macaco2direita(void * a){
  int i = *((int *) a);    
  while(1){
	    pthread_mutex_lock(&TURNO);
	    pthread_mutex_lock(&DIREITA);
	    right++;
	    if(right == 1){
    		pthread_mutex_lock(&PONTE);
	    }
	    pthread_mutex_unlock(&DIREITA);
	    pthread_mutex_unlock(&TURNO);
	    printf("Macaco %d passando: direita -> esquerda \n",i);
	    sleep(1);
	    pthread_mutex_lock(&DIREITA);
      right--;
      printf("Macaco %d passou\n" ,i);
	    if(right == 0){
    		pthread_mutex_unlock(&PONTE);
	    }
	    pthread_mutex_unlock(&DIREITA);
  }
}

void * macaco2esquerda(void * a){
    int i = *((int *) a);
    while(1){
	    pthread_mutex_lock(&TURNO);
	    pthread_mutex_lock(&ESQUERDA);
	    left++;
	    if(left == 1){
    		pthread_mutex_lock(&PONTE);
	    }
	    pthread_mutex_unlock(&ESQUERDA);
	    pthread_mutex_unlock(&TURNO);
	    printf("Macaco %d passando: esquerda -> direita \n",i);
	    sleep(1);
	    pthread_mutex_lock(&ESQUERDA);
	    left--;
      printf("Macaco %d passou\n" ,i);
	    if(left == 0){
    		pthread_mutex_unlock(&PONTE);
	    }
	    pthread_mutex_unlock(&ESQUERDA);
    }
}

int main(int argc, char * argv[]){
    pthread_t m_o[N_ESQUERDA+N_DIREITA];


    pthread_mutex_init(&DIREITA, NULL);
    pthread_mutex_init(&ESQUERDA, NULL);
    pthread_mutex_init(&PONTE, NULL);
    pthread_mutex_init(&TURNO, NULL);


   int *id;
   int i = 0;
   for(i = 0; i < N_ESQUERDA+N_DIREITA; ++i){
        id = (int *) malloc(sizeof(int));
        *id = i;
        if(i%2 == 0)
          pthread_create(&m_o[i], NULL, &macaco2esquerda, (void*)id);
        
        else
          pthread_create(&m_o[i], NULL, &macaco2direita, (void*)id);
        
	      id++;
    }

    for(i = 0; i < N_ESQUERDA+N_DIREITA; ++i){
      pthread_join(m_o[i], NULL);
    }

    return 0;
}