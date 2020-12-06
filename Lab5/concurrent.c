#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define THREADS 5

typedef struct
{
    int data[25];
} Hist;

int samples = 500000;

void printHistogram(int hist[]);
int id;
int base_pid;
int pid;
Hist *hist;

int main(int argc, char const *argv[])
{
    if (argc == 2)
    {
        samples = atoi(argv[1]);
    }

    srand(time(NULL));

    if ((id = shmget(IPC_PRIVATE, sizeof(Hist), IPC_CREAT | 0666)) < 0)
    {
        printf("couldn't create the shared memory");
        exit(-1);
    }

    base_pid = getpid();
    for (int i = 0; i < THREADS; i++)
    {
        if (getpid() == base_pid)
            pid = fork();
        else
            break;
    }
    if ((hist = (Hist *)shmat(id, NULL, 0)) == (Hist *)-1)
    {
        printf("couldn't attach to the shared memory");
        exit(-1);
    }
    // else
    //     printf("Process %d attached the segment %d\n", getpid(), id);

    if (getpid() == base_pid)
    {
        for (int i = 0; i < THREADS; i++)
        {
            wait(NULL);
        }
    }
    else
    {

        int counter, random;
        for (int i = 0; i < samples / THREADS; i++)
        {
            counter = 0;
            for (int j = 0; j < 12; j++)
            {
                random = rand() % 101;
                if (random >= 49)
                {
                    counter++;
                }
                else
                {
                    counter--;
                }
            }
            hist->data[counter + 12]++;
        }

        exit(0);
    }

    // printHistogram(hist->data);

    return 0;
}

void printHistogram(int hist[])
{
    for (int i = 0; i < 25; i++)
    {
        // printf("%d: %d\n", i - 12, hist[i]);
        for(int j = 0; j < hist[i]; j++) {
          printf("*");
        }
        printf("\n");
    }
}
