// Init Thread
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NLOPS 10000
int counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Template function
void *thread_function(void *arg);

int main(){
    pthread_t thread1, thread2, thread3, thread4;
    
    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);
    pthread_create(&thread3, NULL, thread_function, NULL);
    pthread_create(&thread4, NULL, thread_function, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    return 0;
}

void *thread_function(void *arg){
    for (int i = 0; i < NLOPS; i++){
        pthread_mutex_lock(&lock);
        counter++;
        printf("Thread %ld: Counter = %d\n", pthread_self(), counter);
        pthread_mutex_unlock(&lock);
    }
}