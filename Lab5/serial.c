#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int samples = 500000;

void printHistogram(int hist[]);

int main(int argc, char const *argv[])
{
	if (argc == 2) {
		samples = atoi(argv[1]);
	}

	srand(time(NULL));

	int hist[25] = {0};
	int counter = 0;
	int random;

	for (int i = 0; i < samples; i++)
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
		hist[counter + 12]++;
	}

	// printHistogram(hist);

	return 0;
}

void printHistogram(int hist[])
{
	for (int i = 0; i < 25; i++)
	{
		// printf("%d: %d\n", i - 12, hist[i]);
		for(int j = 0; j < hist[i]; j++){
		  printf("*");
		}
		printf("\n");
	}
}
