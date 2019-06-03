/*
    Aluno: Vitor Fernandes Dullens
    Matricula: 16/1048260
    Compilação: gcc -pthread -o exec trabalho.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* cores - estetica */
#define GREEN "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"
#define CYAN "\x1b[36m"
#define RED "\x1b[31m"

/* quantos jogadores e quantas estrelas */
#define NUMERO_JOGADORES 8
#define NUMERO_ESTRELAS 2

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t juiz_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t estrela_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t jogador_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t comecar_jogo_cond = PTHREAD_COND_INITIALIZER;

int start = 1;
int no_banco = 0;
int em_quadra = 0;
int estrela_espera = 0;
int estrela_no_banco = 0;
int estrela_descanso = 0;
int jogador_descanso = 0;
int estrela_em_quadra = 0;

void print_dados(){
    printf("EM QUADRA: %d estrelas %d jogadores\n", estrela_em_quadra, em_quadra-estrela_em_quadra);
    printf("BANCO PARA JOGAR: %d estrelas %d jogadores\n", estrela_no_banco, no_banco);
    printf("BANCO DESCANSO: %d estrelas %d jogadores\n\n", estrela_descanso, jogador_descanso);
}

void * Juiz(void * data){
    while(1){
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&juiz_cond, &lock);
        
        if(start){
            print_dados();
            printf(GREEN "-> Juiz: Temos 5 jogadores em quadra - Apita o apito - Comeco!\n" RESET);
            sleep(5);
            start=0; // juiz ja autorizou o comeco da partida
        }
        else {
            printf(CYAN "-> Juiz: Susbtituicao - Apita o apito - Volta jogo\n" RESET);
            sleep(5);
            system("clear");

        }

        pthread_cond_broadcast(&comecar_jogo_cond); // libera todos os jogadores para o jogo

        pthread_mutex_unlock(&lock);
    }
    
}

void * Estrela (void * data){
    int id = (int)(long) data;
    while(1){
        pthread_mutex_lock(&lock);

        estrela_espera++;
        if(estrela_descanso) estrela_descanso--;

        while(em_quadra == 5) {
            estrela_no_banco++;
            no_banco++;

            print_dados();
            printf(YELLOW "**Estrela** %d: No banco - Quero entrar\n" RESET, id);
            sleep(2);
            system("clear");
            pthread_cond_wait(&estrela_cond, &lock); //espera ate ter espaco em quadra
            estrela_no_banco--;
            no_banco--;
        }
        estrela_espera--;
        
        em_quadra++;
        estrela_em_quadra++;
        print_dados();
        printf(GREEN "**Estrela** %d: Em quadra\n" RESET, id);
        sleep(2);
        system("clear");
        pthread_mutex_unlock(&lock);

        pthread_mutex_lock(&lock);
        if(em_quadra == 5 && start == 1) pthread_cond_signal(&juiz_cond);
        pthread_mutex_unlock(&lock);
        
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&comecar_jogo_cond, &lock); //espera o juiz apitar o comeco do jogo
        pthread_mutex_unlock(&lock);

        sleep(5 + rand()%5); // tempo da estrela jogar

        pthread_mutex_lock(&lock);
        em_quadra--;
        estrela_descanso++;
        estrela_em_quadra--;
        print_dados();
        printf(RED "**Estrela** %d: Cansei - Vou descansar\n" RESET, id);
        pthread_cond_signal(&juiz_cond);

        pthread_cond_signal(&estrela_cond);
        pthread_cond_signal(&jogador_cond);
        pthread_mutex_unlock(&lock);

        sleep(2 + rand()%5); // tempo de descanso
    }

}

void * Jogador (void * data){
    int id = (int)(long) data;
    while(1){
        pthread_mutex_lock(&lock);
        
        if(jogador_descanso) jogador_descanso--;
        
        while(em_quadra == 5 || estrela_espera > 0){
            no_banco++;

            print_dados();
            printf(CYAN "Jogador %d: No banco - Esperando\n" RESET, id);
            sleep(2);
            system("clear");
            pthread_cond_wait(&jogador_cond, &lock); //espera ate ter espaco em quadra
            no_banco--;
        }

        em_quadra++;
        print_dados();
        printf(GREEN"Jogador %d: Em quadra\n" RESET, id);
        sleep(2);
        system("clear");
        pthread_mutex_unlock(&lock);
        
        pthread_mutex_lock(&lock);
        if(em_quadra == 5 && start == 1) pthread_cond_signal(&juiz_cond);
        pthread_mutex_unlock(&lock);

        pthread_mutex_lock(&lock);
        pthread_cond_wait(&comecar_jogo_cond, &lock); //espera o juiz apitar o comeco do jogo
        pthread_mutex_unlock(&lock);

        sleep(2 + rand()%5); // tempo do jogador jogar

        pthread_mutex_lock(&lock);
        print_dados();
        em_quadra--;
        jogador_descanso++;
        printf(RED "Jogador %d: Cansei - Vou descansar\n" RESET, id);
        pthread_cond_signal(&juiz_cond); 

        pthread_cond_signal(&estrela_cond);
        pthread_cond_signal(&jogador_cond);
        pthread_mutex_unlock(&lock);

        sleep(5 + rand()%5); // tempo de descanso
    }

}

int main(){
    long i;
    pthread_t JUIZ;
    pthread_t ESTRELAS[NUMERO_ESTRELAS];
    pthread_t JOGADORES[NUMERO_JOGADORES];

    pthread_create(&JUIZ, NULL, Juiz, (void *) 0);
    
    for(i=0;i<NUMERO_ESTRELAS;i++) pthread_create(&ESTRELAS[i], NULL, Estrela, (void*) i);
    
    for(i=0;i<NUMERO_JOGADORES;i++) pthread_create(&JOGADORES[i], NULL, Jogador, (void*) i);

    pthread_join(ESTRELAS[0], NULL);
    
}