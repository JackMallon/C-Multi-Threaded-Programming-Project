#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define for_x for (int x = 0; x < w; x++)
#define for_y for (int y = 0; y < h; y++)
#define for_xy for_x for_y

//Generates probability of becoming infected
int infect(int n){
	int random_number = rand() % 10 + 1;
	if(random_number < n || n == 9){
		return 1;
	}
	return 0;
}

//Generates probabilty of becoming removed
int removed(){
	int random_number = rand() % 100 + 1;
	if(random_number > 99){
		return 1;
	}
	return 0;
}

void show(void *u, int w, int h)
{
	int (*univ)[w] = u;
	printf("\033[H");
	for_y {
		for_x {
			if(univ[y][x] == 0) {
				printf("h ");
			}
			if(univ[y][x] == 1) {
				printf("\033[07m  \033[m");
			}
			if(univ[y][x] == 2) {
				printf("  ");
			}
		}
		printf("\033[E");
	}
	fflush(stdout);
}

int evolve(void *u, int w, int h)
{
	unsigned (*univ)[w] = u;
	unsigned new[h][w];

	for_y for_x {
		new[y][x] = univ[y][x];
	}

	for_y for_x {
		//printf("---\n");
		//printf("y = %d, ", y);
		//printf("x = %d\n", x);
		int n = 0;
		for (int y1 = y - 1; y1 <= y + 1; y1++){
			//printf("-");
			for (int x1 = x - 1; x1 <= x + 1; x1++){
				//printf("/");
				//printf("y1 = %d, ", y1);
				//printf("x1 = %d, ", x1);
				if (univ[y1][x1] == 1 && y1 >= 0 && x1 >= 0){

					n++;
				}
			}
		}
		//printf("N: %x\n", n);
		if(infect(n) == 1){
			new[y][x] = 1;
		}

		if(removed() == 1 && univ[y][x] == 0){
			new[y][x] = 2;
		}
	}

	for_y for_x {
		if(new[y][x] == 1){
				univ[y][x] = 1;
		}
		if(new[y][x] == 2){
				univ[y][x] = 2;
		}
	}

	int inf = 0;

	for_y for_x {
		if(univ[y][x] == 1){
			inf++;
		}
	}

	printf("infected count: %d\n", inf);

	if(inf == 400){
		for_y for_x {
			return 1;
		}
	}

	return 0;
}

void beginInfection(int w, int h)
{
	unsigned univ[h][w];
	for_xy univ[y][x] = rand() < RAND_MAX / 10 ? 1 : 0;
	int n = 0;
	show(univ, w, h);
	usleep(200000);
	while (n != 1) {
		int breakloop = evolve(univ, w, h);
		show(univ, w, h);
		usleep(200000);
		if (breakloop == 1) n++;
	}
}

int main()
{
	//Begin timer
	clock_t t;
  t = clock();

	int w = 20, h = 20;
	beginInfection(w, h);
	printf("Game Over! \n");

	//Finish and print time
	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
	printf("EVERYONE HAS BEEN INFECTED\n");
	printf("Program was computing for %f seconds\n", time_taken);
}
