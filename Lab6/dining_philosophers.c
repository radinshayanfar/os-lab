#include <stdio.h>
#include <pthread.h>

pthread_t philosopher[5];
//pthread_t is a data type for defining threads
pthread_mutex_t chopstick[5];
//pthread_mutex for declaring mutex data types

void *process(int n)
{
    printf("n philosopher %d is thinking !!\n", n);

    pthread_mutex_lock(&chopstick[n - 1]);
    pthread_mutex_lock(&chopstick[(n) % 5]);

    printf("n philosopher %d is eating using chopstick[%d] and chopstick[%d]!!\n", n, (n - 1), (n % 5));
    sleep(2);

    pthread_mutex_unlock(&chopstick[n - 1]);
    pthread_mutex_unlock(&chopstick[(n) % 5]);
    printf("n philosopher %d finished eating !!\n", n);
}

int main()
{
    int i;
    //pthread_mutex_init is used to create a mutex
    for (i = 0; i < 5; i++)
    {
        pthread_mutex_init(&chopstick[i], NULL);
    }

    for (i = 1; i <= 5; i++)
    {
        //pthread_create -- to create a thread
        pthread_create(&philosopher[i], NULL, (void *)process, (int *)i);
    }

    for (i = 1; i <= 5; i++)
    {
        pthread_join(philosopher[i], NULL);
    }

    return 0;
}