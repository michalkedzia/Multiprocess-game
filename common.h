#if !defined(_COMMON_H_)
#define _COMMON_H_
#include <stdio.h>
#include <semaphore.h> //sem*
#include <sys/mman.h> // mmap, munmap, shm_open, shm_unlink
#include <fcntl.h> // O_*
#include <stdlib.h> // exit
#include <unistd.h> // close, ftruncate
#include <string.h> // strcasecmp
#include <time.h> // time
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>
#include <ncurses.h>
#include <math.h>
#include <pthread.h>

enum direction { UP = 0, DOWN = 1, RIGHT = 2, LEFT = 3 };
#define ROW 25
#define COLUMN 51
char cBlock = (char)0x2588;
enum player_type { HUMAN = 0, CPU = 1 };

struct drop
{
	int x, y, dropped_treasure, init;
};

struct data_t
{
	sem_t sem1;
	sem_t sem2;
	sem_t sem3;
	int up, down, right, left;
	int init, conect;
	int player_number, coins_found, coins_brought, deaths, x, y, round_number, x_capsite, y_capsite, A_x, A_y, gl_round;
	pid_t serwer_pid;
	char tab[5][5];
	char name[20];
};

struct mem_all
{
	sem_t sem1;
	sem_t sem2;
	sem_t sem3;
	pid_t pid;
	int server_is_full;
	enum player_type type;
	char name[20];
};

struct player_data
{
	char  player_name[20];
	int fd;
	int player_number, coins_found, coins_brought, deaths, x, y, round_number, x_capsite, y_capsite, init, connect, bush, A_x, A_y;
	pid_t serwer_pid, player_pid;
	enum player_type type;
	struct data_t* player_memory;
};

struct data_struct
{
	struct player_data  arr[4];
	struct mem_all* ptr_mem_all;
	int global_round, players_on_serwer;
	struct drop drop_arr[10];
	pthread_t thr_players[4];
    sem_t* psem;
};

struct player_proces_struct
{
	int index;
	sem_t sem;
	struct data_struct* dt;
};


static void err(int c, const char* msg) {
	if (!c)
		return;
	perror(msg);
	exit(1);
}

#endif // _COMMON_H_
