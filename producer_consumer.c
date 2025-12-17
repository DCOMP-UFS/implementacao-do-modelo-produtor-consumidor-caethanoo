/* * Compilar: gcc -g -Wall -o prod_cons produtor_consumidor.c -lpthread
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <unistd.h>
#include <time.h>


// CENÁRIO 1 (Fila Cheia): Produção (1s) mais rápida que Consumo (2s)
#define PROD_DELAY 1
#define CONS_DELAY 2

// CENÁRIO 2 (Fila Vazia): Produção (2s) mais lenta que Consumo (1s)
// #define PROD_DELAY 2
// #define CONS_DELAY 1

#define NUM_PRODUTORES 3
#define NUM_CONSUMIDORES 3
#define BUFFER_SIZE 5   


typedef struct {
   int id_origem;
   int relogio[3];
} RelogioVetorial;

RelogioVetorial taskQueue[BUFFER_SIZE];
int taskCount = 0;

pthread_mutex_t mutex;
pthread_cond_t condFull, condEmpty;

RelogioVetorial gerarRelogio(int id) {
   RelogioVetorial r = { .id_origem = id };
   for (int i = 0; i < 3; i++) r.relogio[i] = rand() % 100;
   return r;
}

RelogioVetorial getTask(int id){
   pthread_mutex_lock(&mutex);
   
   while (taskCount == 0){
      printf("! Fila VAZIA. C%d espera.\n", id);
      pthread_cond_wait(&condEmpty, &mutex);
   }
   
   RelogioVetorial task = taskQueue[0];
   for (int i = 0; i < taskCount - 1; i++) taskQueue[i] = taskQueue[i+1];
   taskCount--;
   
   printf("   [C%d] Consumiu de P%d. Fila: %d/%d\n", id, task.id_origem, taskCount, BUFFER_SIZE);

   pthread_mutex_unlock(&mutex);
   pthread_cond_signal(&condFull);
   return task;
}

void submitTask(RelogioVetorial task, int id){
   pthread_mutex_lock(&mutex);

   while (taskCount == BUFFER_SIZE){
      printf("! Fila CHEIA. P%d espera.\n", id);
      pthread_cond_wait(&condFull, &mutex);
   }

   taskQueue[taskCount++] = task;
   
   printf("[P%d] Produziu [%d, %d, %d]. Fila: %d/%d\n", 
          id, task.relogio[0], task.relogio[1], task.relogio[2], taskCount, BUFFER_SIZE);

   pthread_mutex_unlock(&mutex);
   pthread_cond_signal(&condEmpty);
}

void *threadConsumidora(void* args) {
   long id = (long) args; 
   while (1){ 
      RelogioVetorial rv = getTask(id);
      printf("      -> Processado: [%d, %d, %d]\n", rv.relogio[0], rv.relogio[1], rv.relogio[2]);
      sleep(CONS_DELAY);
   }
   return NULL;
}

void *threadProdutora(void* args) {
    long id = (long) args;
    while(1) {
        submitTask(gerarRelogio(id), id);
        sleep(PROD_DELAY);
    }
    return NULL;
}

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   srand(time(NULL));

   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&condEmpty, NULL);
   pthread_cond_init(&condFull, NULL);

   pthread_t p[NUM_PRODUTORES], c[NUM_CONSUMIDORES];
   long i;

   for (i = 0; i < NUM_PRODUTORES; i++)
      pthread_create(&p[i], NULL, &threadProdutora, (void*) i);

   for (i = 0; i < NUM_CONSUMIDORES; i++)
      pthread_create(&c[i], NULL, &threadConsumidora, (void*) i);
   
   for (i = 0; i < NUM_PRODUTORES; i++) pthread_join(p[i], NULL);
   for (i = 0; i < NUM_CONSUMIDORES; i++) pthread_join(c[i], NULL);
   
   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&condEmpty);
   pthread_cond_destroy(&condFull);
   return 0;
}