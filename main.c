#include "common.h"

pthread_mutex_t mutex_map = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_strct = PTHREAD_MUTEX_INITIALIZER;


sem_t sem_draw_maze, sem_global_round, sem_add_del_players;

char map[25][53] = {
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
		"W   W       W               W         W       W   W",
		"W W WWW WWW WWWWWW WWWW WWW W WWWWWWW WWW WWWWW   W",
		"W W   W W W           W W W         W     W   W   W",
		"W WWW W W WWW   WWWWW W W WWWWW WWWWWWWW WWWW WWW W",
		"W W W   W           W W W     W       W       W W W",
		"W W WWWWW WWW WWWWWWW W W W WWW WWW WWW WWW W W W W",
		"W W         W W       W W W     W   W   W W W   W W",
		"W W WWW WWW WWW WWWW WW WWW W WWW WWW WWW W WWW W W",
		"W W W     W   W   W     W   W   W           W W W W",
		"W WWW WWW WWW WWW WWW WWW   WWW WWWWWWWW WW W W   W",
		"W W   W       W W   W     W W   W W       W W   W W",
		"W W WWWWWW WW W WWW WWW WWW W W W W WWWWW W W WWW W",
		"W W     W   W W   W       W   W   W W     W   W   W",
		"W W W   W WWW WWW WWW WWWWWWW WWW W WWW WWW W W WWW",
		"W W W   W       W   W W       W   W   W     W W W W",
		"W   W   WWWWWWW W W W W WW WWWW WWWWW WWWWWWW W W W",
		"W W W       W   W W W   W     W   W W         W   W",
		"W WWWWWWWWW W WWW WWWWWWW WWWWWWW W WWWWW W   WWW W",
		"W W       W W     W     W       W   W   W W     W W",
		"W W WWWWW W WW WWWW W WWW WW WW WWW W W WWW WWWWW W",
		"W   W     W         W     W   W       W   W       W",
		"W WWW WWWWWWWWWW WWWWWWWWWW W WWWWWWW WWW W       W",
		"W   W                       W           W         W",
		"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
};

char map_obs[25][53] = {
	   "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
	   "W   W       W  ####         W         W       W   W",
	   "W W WWW WWW WWWWWWWWWWW WWW W WWWWWWW WWW WWWWW   W",
	   "W W   W W W           W W W   W     W     W   W   W",
	   "W WWW W W WWW   WWWWW W W WWWWW WWWWWWWWWWWWW WWW W",
	   "W W W   W           W W W     W       W       W W W",
	   "W W WWWWW WWW WWWWWWW W W W WWW WWW WWW WWW W W W W",
	   "W W         W W       W W W     W   W   W W W   W W",
	   "W W WWWWWWW WWW WWWWWWW WWWWW WWW WWW WWW W WWW W W",
	   "W W W     W   W   W     W   W   W         W W W W W",
	   "W WWW WWW WWW WWW WWW WWW W WWW WWWWWWWWWWW W W W W",
	   "W W   W       W W   W  A  W W   W W       W W   W W",
	   "W W WWWWWW WW W WWW WWW WWW WWW W W WWWWW W W WWW W",
	   "W W    #W   W W   W   W   W   W   W W     W W W   W",
	   "W W W ##W WWW WWW WWW WWWWWWW WWW W WWW WWW W W WWW",
	   "W W W   W    #  W   W W       W   W   W     W W W W",
	   "W W W#  WWWWWWW W W W W WW WWWW WWWWW WWWWWWW W W W",
	   "W W W#      W   W W W   W     W   W W         W   W",
	   "W WWWWWWWWW W WWW WWWWWWW WWWWWWW W WWWWW W  #WWW W",
	   "W W       W W     W     W       W   W   W W     W W",
	   "W W WWWWW W WWWWWWW W WWW WWWWW WWW W W WWW WWWWW W",
	   "W   W     W         W     W   W     W W   W ###   W",
	   "W WWW WWWWWWWWWWWWWWWWWWWWW W WWWWWWW WWW W     # W",
	   "W   W                       W           W    ##   W",
	   "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
};

int dir(int arg)
{
	int res = rand() % 4;
	if (res == arg) dir(arg);
	return res;
}

void add_beast(int* xx, int* yy)
{
	int x = 0, y = 0;
	x = rand() % ROW;
	y = rand() % COLUMN;

	if (map[x][y] != '1' && map[x][y] != '2' && map[x][y] != '3' && map[x][y] != '4' && map[x][y] != 'c' && map[x][y] != 't' && map[x][y] != 'T' && map[x][y] != 'W'
		&& map[x][y] != '*' && map[x][y] != '#' && map[x][y] != 'A' && map[x][y] != 'D')
	{
		map[x][y] = '*';
		*xx = x;
		*yy = y;
	}
	else
	{
		add_beast(xx, yy);
	}
}

int BresenhamLine(const int x1, const int y1, const int x2, const int y2)
{
	int d, dx, dy, ai, bi, xi, yi;
	int x = x1, y = y1;
	if (x1 < x2)
	{
		xi = 1;
		dx = x2 - x1;
	}
	else
	{
		xi = -1;
		dx = x1 - x2;
	}
	if (y1 < y2)
	{
		yi = 1;
		dy = y2 - y1;
	}
	else
	{
		yi = -1;
		dy = y1 - y2;
	}
	if (map[x][y] == 'W' || map_obs[x][y] == '#') return 0;
	if (dx > dy)
	{
		ai = (dy - dx) * 2;
		bi = dy * 2;
		d = bi - dx;
		while (x != x2)
		{
			if (d >= 0)
			{
				x += xi;
				y += yi;
				d += ai;
			}
			else
			{
				d += bi;
				x += xi;
			}
			if (map[x][y] == 'W' || map_obs[x][y] == '#') return 0;
		}
	}
	else
	{
		ai = (dx - dy) * 2;
		bi = dx * 2;
		d = bi - dy;
		while (y != y2)
		{
			if (d >= 0)
			{
				x += xi;
				y += yi;
				d += ai;
			}
			else
			{
				d += bi;
				y += yi;
			}
			if (map[x][y] == 'W' || map_obs[x][y] == '#') return 0;
		}
	}
	return 1;
}

void* beast(void* arg)
{
	struct data_struct* strct = (struct data_struct*)arg;
	int x = 0, y = 0, beast_round = 0;
	pthread_mutex_lock(&mutex_map);
	add_beast(&x, &y);
	pthread_mutex_unlock(&mutex_map);

	char obs = 0;
	int bush = 0;
	int last = DOWN, current = DOWN;
	int right = 0, left = 0, up = 0, down = 0;
	int dir_x = 0, dir_y = 0;

	while (1)
	{
		pthread_mutex_lock(&mutex_map);
		pthread_mutex_lock(&mutex_strct);

		for (int i = 0; i < 4; i++)
		{
			if (strct->arr[i].init == 1 && strct->arr[i].connect == 1)
			{
				if (BresenhamLine(x, y, strct->arr[i].x, strct->arr[i].y) == 1)
				{
					dir_x = strct->arr[i].x - x;
					dir_y = strct->arr[i].y - y;

					if (dir_x < 0 && dir_y < 0) last = UP;
					else if (dir_x < 0 && dir_y >0) last = UP;
					else if (dir_x > 0 && dir_y > 0) last = DOWN;
					else if (dir_x > 0 && dir_y < 0) last = DOWN;
					else if (dir_x == 0 && dir_y < 0) last = LEFT;
					else if (dir_x > 0 && dir_y == 0) last = DOWN;
					else if (dir_x < 0 && dir_y == 0) last = UP;
					else if (dir_x == 0 && dir_y > 0) last = RIGHT;
					break;
				}
			}
		}

		if (last == UP)
		{
			if (map[x - 1][y] == 'W' || map[x - 1][y] == '*')
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
			if (map[x + 1][y] == 'W' || map[x + 1][y] == '*')
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
			if (map[x][y - 1] == 'W' || map[x][y - 1] == '*')
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
			if (map[x][y + 1] == 'W' || map[x][y + 1] == '*')
			{
				current = dir(RIGHT);
				last = current;
			}
			else
			{
				current = last;
			}
		}

		if (beast_round == strct->global_round)
		{
			pthread_mutex_unlock(&mutex_map);
			pthread_mutex_unlock(&mutex_strct);
			continue;
		}

		if (last == UP) up = -1;
		if (last == DOWN) down = 1;
		if (last == RIGHT) right = 1;
		if (last == LEFT) left = -1;

		if (((x + up + down) >= 0 && (x + up + down) < ROW) && ((y + left + right) >= 0 && (y + left + right) < COLUMN))
        {
            			if (map[x + up + down][y + left + right] != 'W' && map[x + up + down][y + left + right] != '*' && map[x + up + down][y + left + right] != '1' && map[x + up + down][y + left + right] != '2' && map[x + up + down][y + left + right] != '3' && map[x + up + down][y + left + right] != '4' &&
				map[x + up + down][y + left + right] != 'A' && map[x + up + down][y + left + right] != 'D' && map[x + up + down][y + left + right] != 'c' && map[x + up + down][y + left + right] != 't' && map[x + up + down][y + left + right] != 'T')
			{
				if (bush == 0)
				{
					if (obs == 0) map[x][y] = ' ';
					else map[x][y] = obs;
					obs = 0;
					map[x + up + down][y + right + left] = '*';
					x = x + up + down;
					y = y + right + left;
				}
				else bush = 0;
			}
			else if (map[x + up + down][y + left + right] == '1' || map[x + up + down][y + left + right] == '2' || map[x + up + down][y + left + right] == '3' || map[x + up + down][y + left + right] == '4')
			{
				if (obs == 0) map[x][y] = ' ';
				else map[x][y] = obs;
				obs = 0;
				int free_index = 0;

				for (int i = 0; i < 10; i++)
				{
					if (strct->drop_arr[i].init == 0)
					{
						free_index = i;
						break;
					}
				}

				int temp_pla = map[x + up + down][y + right + left] - '0' - 1;
				x = x + up + down;
				y = y + right + left;
				map[x][y] = '*';

				strct->drop_arr[free_index].init = 1;
				strct->drop_arr[free_index].dropped_treasure = strct->arr[temp_pla].coins_found;
				strct->arr[temp_pla].coins_found = 0;
				strct->drop_arr[free_index].x = strct->arr[temp_pla].x;
				strct->drop_arr[free_index].y = strct->arr[temp_pla].y;
                
				strct->arr[temp_pla].x = strct->arr[temp_pla].A_x;
				strct->arr[temp_pla].y = strct->arr[temp_pla].A_y;

				strct->arr[temp_pla].deaths += 1;
				map[strct->arr[temp_pla].x][strct->arr[temp_pla].y] = strct->arr[temp_pla].player_number + '0' + 1;
				map[strct->drop_arr[free_index].x][strct->drop_arr[free_index].y] = 'D';
				obs = 'D';
			}
			else if (map[x + up + down][y + left + right] == 'c' || map[x + up + down][y + left + right] == 'T' || map[x + up + down][y + left + right] == 't' || map[x + up + down][y + left + right] == 'D' || map[x + up + down][y + left + right] == 'A')
			{
				obs = map[x + up + down][y + left + right];
				map[x][y] = ' ';
				x = x + up + down;
				y = y + right + left;
			}
			else if (map_obs[x + up + down][y + left + right] == '#')
			{
				if (bush == 0)
				{
					if (obs == 0) map[x][y] = ' ';
					else map[x][y] = obs;
					obs = 0;
					map[x + up + down][y + right + left] = '*';
					x = x + up + down;
					y = y + right + left;
					bush = 1;
				}
				else  bush = 0;
			}
        }


		pthread_mutex_unlock(&mutex_map);
		pthread_mutex_unlock(&mutex_strct);
		right = 0, left = 0, up = 0, down = 0;
		beast_round++;

	}
	return NULL;
}

void* round_counter(void* arg)
{
	sem_post(&sem_global_round);
	struct data_struct* strct = (struct data_struct*)arg;
	while (1)
	{
		usleep(250000);

		pthread_mutex_lock(&mutex_strct);
		strct->global_round++;
		pthread_mutex_unlock(&mutex_strct);
	}
	return NULL;
}

void* draw_maze(void* arg)
{
	sem_post(&sem_draw_maze);
	struct data_struct* strct = (struct data_struct*)arg;

	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_RED);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);
	init_pair(4, COLOR_WHITE, COLOR_RED);
	init_pair(5, COLOR_BLACK, COLOR_YELLOW);
	init_pair(6, COLOR_YELLOW, COLOR_GREEN);
	init_pair(7, COLOR_GREEN, COLOR_YELLOW);

	while (1)
	{
		pthread_mutex_lock(&mutex_map);
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COLUMN; j++)
			{
				if (map[i][j] == 'W')
				{
					attron(COLOR_PAIR(1));
					mvwaddch(stdscr, i, j, cBlock);
					addch(cBlock);
					attroff(COLOR_PAIR(1));
				}
				else if (map[i][j] == ' ')
				{
					attron(COLOR_PAIR(1));
					mvwaddch(stdscr, i, j, map[i][j]);
					attroff(COLOR_PAIR(1));
				}
				else if (map[i][j] == '1' || (map[i][j] == '2') || (map[i][j] == '3') || (map[i][j] == '4'))
				{
					attron(COLOR_PAIR(2));
					mvwaddch(stdscr, i, j, map[i][j]);
					attroff(COLOR_PAIR(2));
				}
				else if (map[i][j] == '#')
				{
					attron(COLOR_PAIR(3));
					mvwaddch(stdscr, i, j, map[i][j]);
					attroff(COLOR_PAIR(3));
				}
				else if (map[i][j] == '*')
				{
					attron(COLOR_PAIR(4));
					mvwaddch(stdscr, i, j, map[i][j]);
					attroff(COLOR_PAIR(4));
				}
				else if (map[i][j] == 'c' || map[i][j] == 't' || map[i][j] == 'T')
				{
					attron(COLOR_PAIR(5));
					mvwaddch(stdscr, i, j, map[i][j]);
					attroff(COLOR_PAIR(5));
				}
				else if (map[i][j] == 'A')
				{
					attron(COLOR_PAIR(6));
					mvwaddch(stdscr, i, j, map[i][j]);
					attroff(COLOR_PAIR(6));
				}
				else if (map[i][j] == 'D')
				{
					attron(COLOR_PAIR(7));
					mvwaddch(stdscr, i, j, map[i][j]);
					attroff(COLOR_PAIR(7));
				}
			}
		}

		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COLUMN; j++)
			{
				if (map_obs[i][j] == '1' || (map_obs[i][j] == '2') || (map_obs[i][j] == '3') || (map_obs[i][j] == '4'))
				{
					attron(COLOR_PAIR(2));
					mvwaddch(stdscr, i, j, map_obs[i][j]);
					attroff(COLOR_PAIR(2));
				}
				else if (map_obs[i][j] == '#')
				{
					attron(COLOR_PAIR(3));
					mvwaddch(stdscr, i, j, map_obs[i][j]);
					attroff(COLOR_PAIR(3));
				}
				else if (map_obs[i][j] == 'c' || map_obs[i][j] == 't' || map_obs[i][j] == 'T')
				{
					attron(COLOR_PAIR(5));
					mvwaddch(stdscr, i, j, map_obs[i][j]);
					attroff(COLOR_PAIR(5));
				}
				else if (map_obs[i][j] == 'A')
				{
					attron(COLOR_PAIR(6));
					mvwaddch(stdscr, i, j, map_obs[i][j]);
					attroff(COLOR_PAIR(6));
				}
				else if (map_obs[i][j] == 'D')
				{
					attron(COLOR_PAIR(7));
					mvwaddch(stdscr, i, j, map_obs[i][j]);
					attroff(COLOR_PAIR(7));
				}
			}
		}

		pthread_mutex_unlock(&mutex_map);
		pthread_mutex_lock(&mutex_strct);

		mvprintw(0, 55, "Server's PID: %d", getpid());
		mvprintw(1, 56, "Campsite X/Y: 23/11");
		mvprintw(2, 56, "Round number: %d", strct->global_round);

		mvprintw(5, 55, "Parameter:"); mvprintw(5, 70, "Player1"); mvprintw(5, 80, "Player2"); mvprintw(5, 90, "Player3"); mvprintw(5, 100, "Player4");

		mvprintw(6, 55, "PID");
		for (int i = 0, j = 0; i < 4; i++, j += 10)
		{
			if (strct->arr[i].init == 1 && strct->arr[i].connect == 1)
			{
				mvprintw(6, 70 + j, "     ");
				mvprintw(6, 70 + j, "%d", strct->arr[i].player_pid);
			}
			else mvprintw(6, 70 + j, "--/--");
		}

		mvprintw(7, 55, "Type");
		for (int i = 0, j = 0; i < 4; i++, j += 10)
		{
			if (strct->arr[i].init == 1 && strct->arr[i].connect == 1)
			{
				mvprintw(7, 70 + j, "     ");
				if (strct->arr[i].type == HUMAN) mvprintw(7, 70 + j, "HUMAN");
				else mvprintw(7, 70 + j, "CPU");
			}
			else mvprintw(7, 70 + j, "--/--");
		}

		mvprintw(8, 55, "Curr X/Y");
		for (int i = 0, j = 0; i < 4; i++, j += 10)
		{
			if (strct->arr[i].init == 1 && strct->arr[i].connect == 1)
			{
				mvprintw(8, 70 + j, "     ");
				mvprintw(8, 70 + j, "%d/%d", strct->arr[i].x, strct->arr[i].y);
			}
			else mvprintw(8, 70 + j, "--/--");
		}

		mvprintw(9, 55, "Deaths");
		for (int i = 0, j = 0; i < 4; i++, j += 10)
		{
			if (strct->arr[i].init == 1 && strct->arr[i].connect == 1)
			{
				mvprintw(9, 70 + j, "     ");
				mvprintw(9, 70 + j, "%d", strct->arr[i].deaths);
			}
			else mvprintw(9, 70 + j, "--/--");
		}

		mvprintw(12, 55, "Coins");
		mvprintw(13, 56, "carried");
		mvprintw(9, 55, "Deaths");
		for (int i = 0, j = 0; i < 4; i++, j += 10)
		{
			if (strct->arr[i].init == 1 && strct->arr[i].connect == 1)
			{
				mvprintw(13, 70 + j, "     ");
				mvprintw(13, 70 + j, "%d", strct->arr[i].coins_found);
			}
			else mvprintw(13, 70 + j, "--/--");
		}

		mvprintw(14, 56, "brought");
		for (int i = 0, j = 0; i < 4; i++, j += 10)
		{
			if (strct->arr[i].init == 1 && strct->arr[i].connect == 1)
			{
				mvprintw(14, 70 + j, "     ");
				mvprintw(14, 70 + j, "%d", strct->arr[i].coins_brought);
			}
			else mvprintw(14, 70 + j, "--/--");
		}

		mvprintw(18, 55, "Legend:");
		attron(COLOR_PAIR(2));
		mvprintw(19, 56, "1234");
		attroff(COLOR_PAIR(2));
		mvprintw(19, 56 + 5, " - players");

		attron(COLOR_PAIR(1));
		mvprintw(20, 56, "");
		addch(cBlock);
		attroff(COLOR_PAIR(1));
		mvprintw(20, 56 + 5, " - wall");

		attron(COLOR_PAIR(3));
		mvprintw(21, 56, "#");
		attroff(COLOR_PAIR(3));
		mvprintw(21, 56 + 5, " - bushes (slow down)");

		attron(COLOR_PAIR(4));
		mvprintw(22, 56, "*");
		attroff(COLOR_PAIR(4));
		mvprintw(22, 56 + 5, " - wild beast");

		attron(COLOR_PAIR(5));
		mvprintw(23, 56, "c");
		attroff(COLOR_PAIR(5));
		mvprintw(23, 56 + 5, " - one coin");

		attron(COLOR_PAIR(5));
		mvprintw(24, 56, "t");
		attroff(COLOR_PAIR(5));
		mvprintw(24, 56 + 5, " - treasure (10 coins)");

		attron(COLOR_PAIR(5));
		mvprintw(25, 56, "T");
		attroff(COLOR_PAIR(5));
		mvprintw(25, 56 + 5, " - large treasure (50 coins)");

		attron(COLOR_PAIR(6));
		mvprintw(26, 56, "A");
		attroff(COLOR_PAIR(6));
		mvprintw(26, 56 + 5, " - campsite");

		attron(COLOR_PAIR(7));
		mvprintw(27, 56, "D");
		attroff(COLOR_PAIR(7));
		mvprintw(27, 56 + 5, " - dropped treasure");
		mvprintw(28, 56 + 5, "players on server %d", strct->players_on_serwer);
		pthread_mutex_unlock(&mutex_strct);

		refresh();
	}
	return NULL;
}

void add_coin(char type)
{
	int x = 0, y = 0;
	x = rand() % ROW;
	y = rand() % COLUMN;

	pthread_mutex_lock(&mutex_map);

	if (map[x][y] != '1' && map[x][y] != '2' && map[x][y] != '3' && map[x][y] != '4' && map[x][y] != 'c' && map[x][y] != 't' && map[x][y] != 'T' && map[x][y] != 'W'
		&& map[x][y] != '*' && map[x][y] != '#' && map[x][y] != 'A' && map[x][y] != 'D')
	{
		map[x][y] = type;
		pthread_mutex_unlock(&mutex_map);
	}
	else
	{
		pthread_mutex_unlock(&mutex_map);
		add_coin(type);
	}
}

void give_player_map(char(*arr)[5], int x, int y)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			arr[i][j] = map[i + x - 2][j + y - 2];
		}
	}
}

void position(int* xx, int* yy, int num)
{
	int x = 0, y = 0;
	x = rand() % ROW;
	y = rand() % COLUMN;
	*xx = x;
	*yy = y;

	if (map[x][y] != '1' && map[x][y] != '2' && map[x][y] != '3' && map[x][y] != '4' && map[x][y] != 'c' && map[x][y] != 't' && map[x][y] != 'T' && map[x][y] != 'W'
		&& map[x][y] != '*' && map[x][y] != '#' && map[x][y] != 'A' && map[x][y] != 'D')
	{
		map[x][y] = num + '0';
	}
	else
	{
		position(xx, yy, num);
	}
}

void* player_proces(void* arg)
{
	struct player_proces_struct* strct = (struct player_proces_struct*)arg;
	int* index = &strct->index;
	sem_post(&strct->sem);
	int r = 0, l = 0, u = 0, d = 0;

	while (1)
	{
		r = 0, l = 0, u = 0, d = 0;
		sem_wait(&strct->dt->arr[*index].player_memory->sem1);

		pthread_mutex_lock(&mutex_map);
		pthread_mutex_lock(&mutex_strct);

		if (strct->dt->arr[*index].init == 1 && strct->dt->arr[*index].connect == 1)
		{
			if ((strct->dt->arr[*index].player_memory->init == 0 && strct->dt->arr[*index].player_memory->conect == 0) || (abs(strct->dt->global_round - strct->dt->arr[*index].round_number)) >= 100)
			{
				map[strct->dt->arr[*index].x][strct->dt->arr[*index].y] = ' ';
				strct->dt->arr[*index].init = 0;
				strct->dt->arr[*index].connect = 0;
				strct->dt->players_on_serwer--;

				close(strct->dt->arr[*index].fd);
				shm_unlink(strct->dt->arr[*index].player_name);
                
                sem_post(strct->dt->psem); // dodane
                
				pthread_mutex_unlock(&mutex_map);
				pthread_mutex_unlock(&mutex_strct);
				return NULL;
			}
		}

		give_player_map(strct->dt->arr[*index].player_memory->tab, strct->dt->arr[*index].x, strct->dt->arr[*index].y);

		if (strct->dt->arr[*index].round_number == strct->dt->global_round)
		{
			sem_post(&strct->dt->arr[*index].player_memory->sem2);
			sem_post(&strct->dt->arr[*index].player_memory->sem1);
			pthread_mutex_unlock(&mutex_map);
			pthread_mutex_unlock(&mutex_strct);
			continue;
		}

		if (strct->dt->arr[*index].player_memory->up == 1) u = -1;
		else if (strct->dt->arr[*index].player_memory->down == 1) d = 1;
		else if (strct->dt->arr[*index].player_memory->right == 1) r = 1;
		else if (strct->dt->arr[*index].player_memory->left == 1) l = -1;

		if (strct->dt->arr[*index].player_memory->up == 1 || strct->dt->arr[*index].player_memory->left == 1 || strct->dt->arr[*index].player_memory->right == 1 || strct->dt->arr[*index].player_memory->down == 1)
		{
			if (((strct->dt->arr[*index].x + u + d) >= 0 && (strct->dt->arr[*index].x + u + d) < ROW) && ((strct->dt->arr[*index].y + l + r) >= 0 && (strct->dt->arr[*index].y + r + l) < COLUMN))
			{
				if (map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] != 'W' && map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] != '1' && map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] != '2' && map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] != '3'
					&& map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] != 'c' && map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] != 't'
					&& map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] != 'T'
					&& map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] != '4' && map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] != '*' && map_obs[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] != '#' && map_obs[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] != 'A' && map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] != 'D')
				{

					if (strct->dt->arr[*index].bush == 0)
					{
						map[strct->dt->arr[*index].x][strct->dt->arr[*index].y] = ' ';
						strct->dt->arr[*index].x = strct->dt->arr[*index].x + u + d;
						strct->dt->arr[*index].y = strct->dt->arr[*index].y + l + r;
						map[strct->dt->arr[*index].x][strct->dt->arr[*index].y] = strct->dt->arr[*index].player_number + '0' + 1;

					}
					else strct->dt->arr[*index].bush = 0;

					strct->dt->arr[*index].player_memory->up = 0;
					strct->dt->arr[*index].player_memory->down = 0;
					strct->dt->arr[*index].player_memory->right = 0;
					strct->dt->arr[*index].player_memory->left = 0;

					give_player_map(strct->dt->arr[*index].player_memory->tab, strct->dt->arr[*index].x, strct->dt->arr[*index].y);
					strct->dt->arr[*index].round_number = strct->dt->global_round;
					sem_post(&strct->dt->arr[*index].player_memory->sem2);

				}
				else if (map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] == 'c' || map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] == 't' || map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] == 'T')
				{
					if (strct->dt->arr[*index].bush == 0)
					{
						if (map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] == 'c')
						{
							strct->dt->arr[*index].coins_found += 1;

						}
						else if (map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] == 't')
						{
							strct->dt->arr[*index].coins_found += 10;
						}
						else if (map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] == 'T')
						{
							strct->dt->arr[*index].coins_found += 50;
						}
						map[strct->dt->arr[*index].x][strct->dt->arr[*index].y] = ' ';

						strct->dt->arr[*index].x = strct->dt->arr[*index].x + u + d;
						strct->dt->arr[*index].y = strct->dt->arr[*index].y + l + r;
						map[strct->dt->arr[*index].x][strct->dt->arr[*index].y] = strct->dt->arr[*index].player_number + '0' + 1;
					}
					else strct->dt->arr[*index].bush = 0;
                    
					strct->dt->arr[*index].player_memory->up = 0;
					strct->dt->arr[*index].player_memory->down = 0;
					strct->dt->arr[*index].player_memory->right = 0;
					strct->dt->arr[*index].player_memory->left = 0;
					strct->dt->arr[*index].round_number = strct->dt->global_round;
					strct->dt->arr[*index].player_memory->coins_found = strct->dt->arr[*index].coins_found;
					give_player_map(strct->dt->arr[*index].player_memory->tab, strct->dt->arr[*index].x, strct->dt->arr[*index].y);
					sem_post(&strct->dt->arr[*index].player_memory->sem2);

				}
				else if (map_obs[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] == 'A')
				{
					strct->dt->arr[*index].coins_brought = strct->dt->arr[*index].coins_found;
					strct->dt->arr[*index].coins_found = strct->dt->arr[*index].player_memory->coins_found = 0;
					strct->dt->arr[*index].player_memory->coins_brought = strct->dt->arr[*index].coins_brought;
					strct->dt->arr[*index].player_memory->round_number = strct->dt->global_round;

					map[strct->dt->arr[*index].x][strct->dt->arr[*index].y] = ' ';
					strct->dt->arr[*index].x_capsite = strct->dt->arr[*index].player_memory->x_capsite = 23;
					strct->dt->arr[*index].y_capsite = strct->dt->arr[*index].player_memory->y_capsite = 11;

					strct->dt->arr[*index].x = strct->dt->arr[*index].A_x;
					strct->dt->arr[*index].y = strct->dt->arr[*index].A_y;

					map[strct->dt->arr[*index].x][strct->dt->arr[*index].y] = strct->dt->arr[*index].player_number + '0' + 1;
					strct->dt->arr[*index].player_memory->up = 0;
					strct->dt->arr[*index].player_memory->down = 0;
					strct->dt->arr[*index].player_memory->right = 0;
					strct->dt->arr[*index].player_memory->left = 0;

					give_player_map(strct->dt->arr[*index].player_memory->tab, strct->dt->arr[*index].x, strct->dt->arr[*index].y);
					sem_post(&strct->dt->arr[*index].player_memory->sem2);
				}
				else if (map_obs[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] == '#')
				{
					if (strct->dt->arr[*index].bush == 0)
					{
						strct->dt->arr[*index].bush = 1;
						map[strct->dt->arr[*index].x][strct->dt->arr[*index].y] = ' ';
						strct->dt->arr[*index].x = strct->dt->arr[*index].x + u + d;
						strct->dt->arr[*index].y = strct->dt->arr[*index].y + l + r;
						map[strct->dt->arr[*index].x][strct->dt->arr[*index].y] = strct->dt->arr[*index].player_number + '0' + 1;
					}
					else  strct->dt->arr[*index].bush = 0;

					strct->dt->arr[*index].player_memory->round_number = strct->dt->global_round;
					strct->dt->arr[*index].player_memory->up = 0;
					strct->dt->arr[*index].player_memory->down = 0;
					strct->dt->arr[*index].player_memory->right = 0;
					strct->dt->arr[*index].player_memory->left = 0;
					give_player_map(strct->dt->arr[*index].player_memory->tab, strct->dt->arr[*index].x, strct->dt->arr[*index].y);
					sem_post(&strct->dt->arr[*index].player_memory->sem2);
				}
				else if (map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] == '1' || map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] == '2' || map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] == '3' || map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] == '4')
				{
					map[strct->dt->arr[*index].x][strct->dt->arr[*index].y] = ' ';
					strct->dt->arr[*index].x = strct->dt->arr[*index].x + u + d;
					strct->dt->arr[*index].y = strct->dt->arr[*index].y + l + r;

					int free_index = 0;

					for (int i = 0; i < 10; i++)
					{
						if (strct->dt->drop_arr[i].init == 0)
						{
							free_index = i;
							break;
						}
					}

					int temp_pla = map[strct->dt->arr[*index].x][strct->dt->arr[*index].y] - '0' - 1;
					strct->dt->drop_arr[free_index].init = 1;
					strct->dt->drop_arr[free_index].dropped_treasure = strct->dt->arr[*index].coins_found;
					strct->dt->arr[*index].coins_found = 0;
					strct->dt->drop_arr[free_index].x = strct->dt->arr[*index].x;
					strct->dt->drop_arr[free_index].y = strct->dt->arr[*index].y;

					strct->dt->arr[*index].x = strct->dt->arr[*index].A_x;
					strct->dt->arr[*index].y = strct->dt->arr[*index].A_y;

					map[strct->dt->arr[*index].x][strct->dt->arr[*index].y] = strct->dt->arr[*index].player_number + '0' + 1;

					strct->dt->drop_arr[free_index].dropped_treasure += strct->dt->arr[temp_pla].coins_found;
					strct->dt->arr[temp_pla].coins_found = 0;

					strct->dt->arr[temp_pla].x = strct->dt->arr[temp_pla].A_x;
					strct->dt->arr[temp_pla].y = strct->dt->arr[temp_pla].A_y;
                    
					map[strct->dt->arr[temp_pla].x][strct->dt->arr[temp_pla].y] = temp_pla + '0' + 1;

					map[strct->dt->drop_arr[free_index].x][strct->dt->drop_arr[free_index].y] = 'D';
					strct->dt->arr[*index].player_memory->up = 0;
					strct->dt->arr[*index].player_memory->down = 0;
					strct->dt->arr[*index].player_memory->right = 0;
					strct->dt->arr[*index].player_memory->left = 0;
					give_player_map(strct->dt->arr[*index].player_memory->tab, strct->dt->arr[*index].x, strct->dt->arr[*index].y);
					sem_post(&strct->dt->arr[*index].player_memory->sem2);
				}
				else if (map[strct->dt->arr[*index].x + u + d][strct->dt->arr[*index].y + l + r] == 'D')
				{
					map[strct->dt->arr[*index].x][strct->dt->arr[*index].y] = ' ';
					strct->dt->arr[*index].x = strct->dt->arr[*index].x + u + d;
					strct->dt->arr[*index].y = strct->dt->arr[*index].y + l + r;
					map[strct->dt->arr[*index].x][strct->dt->arr[*index].y] = strct->dt->arr[*index].player_number + '0' + 1;
					for (int i = 0; i < 10; i++)
					{
						if (strct->dt->drop_arr[i].init == 1)
						{
							if (strct->dt->drop_arr[i].x == strct->dt->arr[*index].x && strct->dt->drop_arr[i].y == strct->dt->arr[*index].y)
							{
								strct->dt->arr[*index].coins_found += strct->dt->drop_arr[i].dropped_treasure;
								strct->dt->drop_arr[i].init = 0;
							}
						}
					}
					strct->dt->arr[*index].player_memory->up = 0;
					strct->dt->arr[*index].player_memory->down = 0;
					strct->dt->arr[*index].player_memory->right = 0;
					strct->dt->arr[*index].player_memory->left = 0;
					give_player_map(strct->dt->arr[*index].player_memory->tab, strct->dt->arr[*index].x, strct->dt->arr[*index].y);
					strct->dt->arr[*index].round_number = strct->dt->global_round;
					sem_post(&strct->dt->arr[*index].player_memory->sem2);
				}
				else
				{
					strct->dt->arr[*index].player_memory->up = 0;
					strct->dt->arr[*index].player_memory->down = 0;
					strct->dt->arr[*index].player_memory->right = 0;
					strct->dt->arr[*index].player_memory->left = 0;
					sem_post(&strct->dt->arr[*index].player_memory->sem2);
				}
			}
			else
			{
				strct->dt->arr[*index].player_memory->up = 0;
				strct->dt->arr[*index].player_memory->down = 0;
				strct->dt->arr[*index].player_memory->right = 0;
				strct->dt->arr[*index].player_memory->left = 0;
				sem_post(&strct->dt->arr[*index].player_memory->sem2);
			}
		}
		strct->dt->arr[*index].player_memory->coins_brought = strct->dt->arr[*index].coins_brought;
		strct->dt->arr[*index].player_memory->coins_found = strct->dt->arr[*index].coins_found;
		strct->dt->arr[*index].player_memory->deaths = strct->dt->arr[*index].deaths;
		strct->dt->arr[*index].player_memory->x = strct->dt->arr[*index].x;
		strct->dt->arr[*index].player_memory->y = strct->dt->arr[*index].y;

		strct->dt->arr[*index].player_memory->gl_round = strct->dt->global_round;
		sem_post(&strct->dt->arr[*index].player_memory->sem1);

		pthread_mutex_unlock(&mutex_map);
		pthread_mutex_unlock(&mutex_strct);
	}
	return NULL;
}

void* add_players(void* arg)
{
	sem_post(&sem_add_del_players);
	struct data_struct* strct = (struct data_struct*)arg;
	struct player_proces_struct pps[4];

	while (1)
	{
		sem_wait(&strct->ptr_mem_all->sem2);

		pthread_mutex_lock(&mutex_map);
		pthread_mutex_lock(&mutex_strct);

		if (strct->players_on_serwer == 4)
		{
			strct->ptr_mem_all->server_is_full = 1;
			sem_post(&strct->ptr_mem_all->sem3);

			pthread_mutex_unlock(&mutex_map);
			pthread_mutex_unlock(&mutex_strct);

			continue;
		}

		strct->ptr_mem_all->server_is_full = 0;

		for (int i = 0; i < 4; i++)
		{
			if (strct->arr[i].init == 0 && strct->arr[i].connect == 0)
			{
				strct->arr[i].fd = shm_open(strct->arr[i].player_name, O_CREAT | O_RDWR, 0600);

				ftruncate(strct->arr[i].fd, sizeof(struct data_t));
				strct->arr[i].player_memory = (struct data_t*)mmap(NULL, sizeof(struct data_t), PROT_READ | PROT_WRITE, MAP_SHARED, strct->arr[i].fd, 0);

				strcpy(strct->ptr_mem_all->name, strct->arr[i].player_name);
				strct->arr[i].type = strct->ptr_mem_all->type;
				strct->arr[i].player_pid = strct->ptr_mem_all->pid;

				strct->arr[i].bush = 0;
				strct->arr[i].player_memory->init = strct->arr[i].init = 1;
				strct->arr[i].player_memory->conect = strct->arr[i].connect = 1;
				strct->players_on_serwer++;;
				strct->arr[i].player_memory->player_number = strct->arr[i].player_number = i;
				strct->arr[i].player_memory->coins_found = strct->arr[i].coins_found = 0;
				strct->arr[i].player_memory->coins_brought = strct->arr[i].coins_brought = 0;
				strct->arr[i].player_memory->deaths = strct->arr[i].deaths = 0;

				sem_init(&(strct->arr[i].player_memory->sem1), 1, 1);
				sem_init(&(strct->arr[i].player_memory->sem2), 1, 0);
				sem_init(&(strct->arr[i].player_memory->sem3), 1, 0);


				position(&strct->arr[i].x, &strct->arr[i].y, i + 1);

				strct->arr[i].player_memory->A_x = strct->arr[i].A_x = strct->arr[i].x;
				strct->arr[i].player_memory->A_y = strct->arr[i].A_y = strct->arr[i].y;

				give_player_map(strct->arr[i].player_memory->tab, strct->arr[i].x, strct->arr[i].y);

				strct->arr[i].player_memory->x = strct->arr[i].x;
				strct->arr[i].player_memory->y = strct->arr[i].y;

				strct->arr[i].player_memory->round_number = strct->arr[i].round_number = strct->global_round;
				strct->arr[i].player_memory->serwer_pid = strct->arr[i].serwer_pid = getpid();
				strct->arr[i].player_memory->up = 0;
				strct->arr[i].player_memory->down = 0;
				strct->arr[i].player_memory->left = 0;
				strct->arr[i].player_memory->right = 0;

				pps[i].index = i;
				pps[i].dt = strct;
				sem_init(&pps[i].sem, 0, 0);

				pthread_create(&strct->thr_players[i], NULL, player_proces, &pps[i]);

				sem_wait(&pps[i].sem);
				sem_destroy(&pps[i].sem);

				break;
			}
		}
		pthread_mutex_unlock(&mutex_map);
		pthread_mutex_unlock(&mutex_strct);
		sem_post(&strct->ptr_mem_all->sem3);
	}


	return NULL;
}

int main(int argc, char** argv)
{
	system("resize -s 55 110");
	srand(time(NULL));
	initscr();
	start_color();
	int c = 0;
	clear();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
    struct data_struct strct;
    
    
    sem_t* psem = sem_open("sem_server", O_CREAT | O_EXCL , 0600 , 5);
    if(psem==SEM_FAILED)
    {
        psem = sem_open ("sem_server" ,0);
        sem_post(psem);
        sem_post(psem);
        sem_post(psem);
        sem_post(psem);
        sem_post(psem);
    }
    
    
    strct.psem=psem;
    
    

	int fd = shm_open("/memory", O_CREAT | O_RDWR, 0600);
	err(fd == -1, "shm_open");

	ftruncate(fd, sizeof(struct mem_all));
	struct mem_all* memory = (struct mem_all*)mmap(NULL, sizeof(struct mem_all), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	err(memory == NULL, "mmap");

	sem_init(&memory->sem1, 1, 1);
	sem_init(&memory->sem2, 1, 0);
	sem_init(&memory->sem3, 1, 0);

	strct.global_round = 1;
	strct.ptr_mem_all = memory;
	strct.players_on_serwer = 0;

	char* players_array_name[4] = { "/player_1_dt","/player_2_dt","/player_3_dt","/player_4_dt" };

	for (int i = 0; i < 10; i++)
	{

		strct.drop_arr[i].x = 0;
		strct.drop_arr[i].y = 0;
		strct.drop_arr[i].dropped_treasure = 0;
		strct.drop_arr[i].init = 0;
	}

	for (int i = 0; i < 4; i++)
	{
		strct.arr[i].type = 0;
		strct.arr[i].player_pid = -1;
		strct.arr[i].init = 0;
		strct.arr[i].connect = 0;
		strct.arr[i].player_number = i;
		strct.arr[i].coins_found = 0;
		strct.arr[i].coins_brought = 0;
		strct.arr[i].deaths = 0;
		strct.arr[i].A_x = 0;
		strct.arr[i].A_y = 0;
		strct.arr[i].x_capsite = 0;
		strct.arr[i].y_capsite = 0;
		strct.arr[i].x = 0;
		strct.arr[i].y = 0;
		strct.arr[i].round_number = 0;
		strct.arr[i].serwer_pid = getpid();
		strct.arr[i].bush = 0;
		strcpy(strct.arr[i].player_name, players_array_name[i]);
	}

	pthread_t thr_add_players, thr_draw_maze, thr_round_counter;
	pthread_t thr_beast_arr[10];
	int beast_counter = 0;

	sem_init(&sem_draw_maze, 0, 0);
	sem_init(&sem_global_round, 0, 0);
	sem_init(&sem_add_del_players, 0, 0);

	pthread_create(&thr_round_counter, NULL, round_counter, &strct);
	sem_wait(&sem_global_round);

	pthread_create(&thr_draw_maze, NULL, draw_maze, &strct);
	sem_wait(&sem_draw_maze);

	pthread_create(&thr_add_players, NULL, add_players, &strct);
	sem_wait(&sem_add_del_players);

	int end = 0;

	while (1) {

		c = wgetch(stdscr);
		switch (c)
		{

		case 'Q':
			end = 1;
			break;
		case 'q':
			end = 1;
			break;

		case 'B':
			if (beast_counter < 10)
			{
				pthread_create(&thr_beast_arr[beast_counter], NULL, beast, &strct);
				beast_counter++;
			}

			break;

		case 'b':
			if (beast_counter < 10)
			{
				pthread_create(&thr_beast_arr[beast_counter], NULL, beast, &strct);
				beast_counter++;
			}

			break;
		case 'c':
			add_coin('c');
			break;
		case 't':
			add_coin('t');
			break;
		case 'T':
			add_coin('T');
			break;
		default:
			break;
		}

		if (end == 1)
        {
            sem_close(psem);
            sem_unlink("sem_server");
            close(fd);
            shm_unlink("/memory");
            break;
        }
	}

	clrtoeol();
	endwin();
	return 0;
}