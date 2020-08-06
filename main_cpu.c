

#include "common.h"

sem_t sem_draw_maze;

void* draw_maze(void* arg)
{
	sem_post(&sem_draw_maze);
	struct str_data* strct = (struct str_data*)arg;
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_RED);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);
	init_pair(4, COLOR_RED, COLOR_WHITE);
	init_pair(5, COLOR_BLACK, COLOR_YELLOW);
	init_pair(6, COLOR_YELLOW, COLOR_GREEN);
	init_pair(7, COLOR_GREEN, COLOR_YELLOW);

	while (1)
	{
		sem_wait(&strct->memory->sem1);
		int temp_x = strct->memory->x, temp_y = strct->memory->y;
		temp_x = temp_x - 2;
		temp_y = temp_y - 2;
		if (temp_x <= 0) temp_x = 0;
		if (temp_y <= 0) temp_y = 0;

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (strct->arr[i][j] == 'W')
				{
					attron(COLOR_PAIR(1));
					move(temp_x + i, temp_y + j);
					addch(cBlock);
					attroff(COLOR_PAIR(1));
				}
				else if (strct->arr[i][j] == ' ')
				{
					attron(COLOR_PAIR(1));
					move(temp_x + i, temp_y + j);
					addch(strct->arr[i][j]);
					attroff(COLOR_PAIR(1));
				}
				else if (strct->arr[i][j] == '1' || (strct->arr[i][j] == '2') || (strct->arr[i][j] == '3') || (strct->arr[i][j] == '4'))
				{
					attron(COLOR_PAIR(2));
					move(temp_x + i, temp_y + j);
					addch(strct->arr[i][j]);
					attroff(COLOR_PAIR(2));
				}
				else if (strct->arr[i][j] == '#')
				{
					attron(COLOR_PAIR(3));
					move(temp_x + i, temp_y + j);
					addch(strct->arr[i][j]);
					attroff(COLOR_PAIR(3));
				}
				else if (strct->arr[i][j] == '*')
				{
					attron(COLOR_PAIR(4));
					move(temp_x + i, temp_y + j);
					addch(strct->arr[i][j]);
					attroff(COLOR_PAIR(4));
				}
				else if (strct->arr[i][j] == 'c' || strct->arr[i][j] == 't' || strct->arr[i][j] == 'T')
				{
					attron(COLOR_PAIR(5));
					move(temp_x + i, temp_y + j);
					addch(strct->arr[i][j]);
					attroff(COLOR_PAIR(5));
				}
				else if (strct->arr[i][j] == 'A')
				{
					attron(COLOR_PAIR(6));
					move(temp_x + i, temp_y + j);
					addch(strct->arr[i][j]);
					attroff(COLOR_PAIR(6));
				}
				else if (strct->arr[i][j] == 'D')
				{
					attron(COLOR_PAIR(7));
					move(temp_x + i, temp_y + j);
					addch(strct->arr[i][j]);
					attroff(COLOR_PAIR(7));
				}

			}
		}

		mvprintw(1, 53, "Server's PID : %d", strct->memory->serwer_pid);

		if (strct->memory->x_capsite != 0 && strct->memory->y_capsite != 0)
		{
			mvprintw(2, 55, "Campiste X/Y :        ", strct->memory->x_capsite, strct->memory->y_capsite);
			mvprintw(2, 55, "Campiste X/Y : %d/%d", strct->memory->x_capsite, strct->memory->y_capsite);
		}
		else mvprintw(2, 55, "Campiste X/Y : unknown");

		mvprintw(3, 55, "Round number : %d", strct->memory->gl_round);

		mvprintw(5, 53, "Player :");
		mvprintw(6, 55, "Type : CPU");
		mvprintw(7, 55, "Curr X/Y : %d/%d", strct->memory->x, strct->memory->y);
		mvprintw(8, 55, "Deaths : %d", strct->memory->deaths);
		mvprintw(11, 55, "Coins found : %d", strct->memory->coins_found);
		mvprintw(12, 55, "Coins brought : %d", strct->memory->coins_brought);

		mvprintw(15, 53, "Legend:");
		mvprintw(16, 55, "1234 - players");
		mvprintw(17, 55, "%c - wall", cBlock);
		mvprintw(18, 55, "# - bushes (slow down)");
		mvprintw(19, 55, "* - enemy");
		mvprintw(20, 55, "c - one coin");
		mvprintw(21, 55, "t - treasure (10 coins)");
		mvprintw(22, 55, "T - large treasure (50 coins)");
		mvprintw(23, 55, "A - campsite");

		sem_post(&strct->memory->sem1);
		refresh();
	}
	return NULL;
}

int dir(int arg)
{
	int res = rand() % 4;
	if (res == arg) dir(arg);
	return res;
}


int main(int argc, char** argv)
{
	initscr();
	clear();
	start_color();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);

	struct str_data strct;
    
    sem_t* psem = sem_open ("sem_server" ,0);
    if(psem==SEM_FAILED)
    {
        printf("BRAK SERWERA !!!\n");
        psem = sem_open("sem_server", O_CREAT , 0600 , 0);
    }
    
    sem_wait(psem);
 
    int fd = shm_open("/memory", O_RDWR, 0600);

	struct mem_all* memory = (struct mem_all*)mmap(NULL, sizeof(struct mem_all), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	err(memory == NULL, "mmap");

	sem_wait(&memory->sem1);
	memory->type = CPU;
	memory->pid = getpid();
	sem_post(&memory->sem2);
	sem_wait(&memory->sem3);
	if (memory->server_is_full == 1)
	{
		printf("Serwer pelny\n");
		return 0;
	}

	int fd2 = shm_open(memory->name, O_RDWR, 0600);
	struct data_t* mem = (struct data_t*)mmap(NULL, sizeof(struct data_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
	for (int i = 0; memory->name[i] != '\0'; i++) memory->name[i] = '#';
	strct.memory = mem;
	strct.arr = mem->tab;
	sem_post(&memory->sem1);

	pthread_t thr_draw_maze;
	sem_init(&sem_draw_maze, 0, 0);
	pthread_create(&thr_draw_maze, NULL, draw_maze, &strct);
	sem_wait(&sem_draw_maze);

	int last = RIGHT, current = 0;

	while (1)
	{

		sem_wait(&mem->sem1);

		if (last == UP)
		{
			if (mem->tab[2 - 1][2] == 'W' || mem->tab[2 - 1][2] == '*' || mem->tab[2 - 1][2] == '1' || mem->tab[2 - 1][2] == '2' || mem->tab[2 - 1][2] == '3' || mem->tab[2 - 1][2] == '4')
			{
				current = dir(UP);
				last = current;
			}
			else
			{
				current = last;
			}


		}
		else if (last == DOWN)
		{
			if (mem->tab[2 + 1][2] == 'W' || mem->tab[2 + 1][2] == '*' || mem->tab[2 + 1][2] == '1' || mem->tab[2 + 1][2] == '2' || mem->tab[2 + 1][2] == '3' || mem->tab[2 + 1][2] == '4')
			{
				current = dir(DOWN);
				last = current;
			}
			else
			{
				current = last;
			}
		}
		else if (last == LEFT)
		{
			if (mem->tab[2][2 - 1] == 'W' || mem->tab[2][2 - 1] == '*' || mem->tab[2][2 - 1] == '1' || mem->tab[2][2 - 1] == '2' || mem->tab[2][2 - 1] == '3' || mem->tab[2][2 - 1] == '4')
			{
				current = dir(LEFT);
				last = current;
			}
			else
			{
				current = last;
			}
		}
		else if (last == RIGHT)
		{
			if (mem->tab[2][2 + 1] == 'W' || mem->tab[2][2 + 1] == '*' || mem->tab[2][2 + 1] == '1' || mem->tab[2][2 + 1] == '2' || mem->tab[2][2 + 1] == '3' || mem->tab[2][2 + 1] == '4')
			{
				current = dir(RIGHT);
				last = current;
			}
			else
			{
				current = last;
			}
		}

		mem->left = 0;
		mem->right = 0;
		mem->up = 0;
		mem->down = 0;

		if (current == RIGHT) mem->right = 1;
		else if (current == LEFT) mem->left = 1;
		else if (current == UP) mem->up = 1;
		else if (current == DOWN) mem->down = 1;

		sem_post(&mem->sem1);
		sem_wait(&mem->sem2);
	}

	clrtoeol();
	endwin();
	return 0;
}