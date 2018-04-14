/*	Author	Pavol Grof��k
*	Name	AZA Z1
*	Date	7.4.2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NAME "AZA3.in"

#define X 151		
#define Y 151
#define DX 11
#define DY 11

typedef struct point {
	short x;
	short y;
	short dx;
	short dy;

	struct point* parent;

}POINT;


typedef struct front {
	POINT *point;
	struct front* next;

}FRONT;


//Glob�lne premenn�

char arr[150][150];
char hashmap[151][151][11][11] = { 0 };

FRONT* first;
FRONT* last;

//Funkcia zahashuje dan� stav
int hashing(short x, short y, short dx, short dy) {
	if (hashmap[x][y][dx + 5][dy + 5] == 0) {
		hashmap[x][y][dx + 5][dy + 5] = 1;
		return 1;
	}
	return 0;
}

//Funkcia obnov� hashmapu na default stav
void clear_hashmap() {
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int k = 0; k < DX; k++) {
				for (int z = 0; z < DY; z++) {
					hashmap[i][j][k][z] = 0;
				}
			}
		}
	}
}



//Funkcia inicializuje frontu
void initialize_front() {
	first = (FRONT*)malloc(sizeof(FRONT));
	first->next = first->point = NULL;
	last = first;
}
//Funkcia dealokuje frontu
void deallocate_front() {

	if (first != NULL) {
		while (first != NULL) {
			last = first->next;
			free(first);
			first = last;
		}
	}
	first = last = NULL;
}


//Vytvorenie nov�ho uzla do fronty
FRONT* new_node(POINT* p) {
	FRONT* tmp = (FRONT*)malloc(sizeof(FRONT));
	tmp->next = NULL;
	tmp->point = p;

	return tmp;
}

//Funkcia prid� bod do fronty
void add_to_front(POINT* new) {
	

	if (first == NULL) {
		first = new_node(new);
		last = first;
	}
	else
	{
		last->next = new_node(new);
		last = last->next;
	}
}

//Funkcia extrahuje z fronty
POINT* extract_from_front() {
	
	if (first == NULL) {
		return NULL;
	}
	else
	{
		POINT* tmp = NULL;
		tmp = first->point;
		first = first->next;
		return (POINT*)tmp;
	}
}

//Funkcia over� pohyb pan��ika o nov� dx a dy
void validate_move(POINT* p, int dx, int dy, int size_x, int size_y) {

	int new_dx = p->dx + dx;
	int new_dy = p->dy + dy;

	//Overenie dx a dy <= 25
	if (((new_dx*new_dx) + (new_dy*new_dy)) <= 25) {

		int new_x = p->x + new_dx;
		int new_y = p->y + new_dy;

		//Overenie pohybu pan��iak po mape
		if (new_x < 0 || new_x >= size_x || new_y < 0 || new_y >= size_y || (arr[new_x][new_y] == 'X')) {
			return;
		}

		//Hashovanie �i sa nach�dza dan� stav v hash_map-e
		int sum = hashing(new_x, new_y, new_dx, new_dy);
		if (sum == 1) {
			POINT* tmp = (POINT*)malloc(sizeof(POINT));
			tmp->dx = new_dx;
			tmp->dy = new_dy;
			tmp->x = new_x;
			tmp->y = new_y;
			tmp->parent = p;

			add_to_front(tmp);
		}
		return;
	}
}


POINT* breadth_first_search(POINT* p, int size_x, int size_y, int end_x, int end_y) {
	POINT* tmp = NULL;

	//initialize_front;
	first = last = NULL;
	add_to_front(p);

	while (first != NULL) {
		tmp = extract_from_front();

		if (tmp->x == end_x && tmp->y == end_y) {
			return tmp;
		}

		validate_move(tmp, -1, -1, size_x, size_y);
		validate_move(tmp, -1, 0, size_x, size_y);
		validate_move(tmp, -1, 1, size_x, size_y);

		validate_move(tmp, 0, -1, size_x, size_y);
		validate_move(tmp, 0, 0, size_x, size_y);
		validate_move(tmp, 0, 1, size_x, size_y);

		validate_move(tmp, 1, -1, size_x, size_y);
		validate_move(tmp, 1, 0, size_x, size_y);
		validate_move(tmp, 1, 1, size_x, size_y);
	}
	return NULL;
}



void vypis_labyrintu(int x, int y) {
	int i, j;

	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			printf("%c", arr[i][j]);
		}
		putchar('\n');
	}
}


int main(void) {

	int scenario, x, y, start_x, start_y, end_x, end_y, c, i, j;
	FILE *f = NULL;
	POINT *path = NULL, *tmp = NULL, *p = NULL;

	if ((f = fopen(NAME, "r")) == NULL) {				//Otvorenie s�boru pre na��tavanie
		printf("Nepodarilo sa otvorit subor\n");
		return -1;
	}


	while (!feof(f)) {								//Na��tavanie scen�rov zo s�boru
		fscanf(f, "%d\n\n", &scenario);

		while (scenario > 0) {
			fscanf(f, "%d", &x);					//Rozmer x po�a
			fscanf(f, "%d\n", &y);					//Rozmer y po�a

			fscanf(f, "%d", &start_x);				//Za�iatok x v poli
			fscanf(f, "%d\n", &start_y);			//Za�iatok y v poli

			fscanf(f, "%d", &end_x);				//Koniec x v poli
			fscanf(f, "%d\n", &end_y);				//Koniec y v poli

			for (i = 0; i < x; i++) {
				for (j = 0; j < y; j++) {
					c = getc(f);

					if (c == '\n') {
						j--;
					}
					else
					{
						arr[i][j] = c;
					}
				}
			}

			p = (POINT*)malloc(sizeof(POINT));
			p->dx = 0;
			p->dy = 0;
			p->x = start_x;
			p->y = start_y;
			p->parent = NULL;
			hashing(p->x, p->y, p->dx, p->dy);
			
			//vypis_labyrintu(x, y);
			path = breadth_first_search(p, x, y, end_x, end_y);

			if (path!=NULL) {
				while (path->parent != NULL) {
					arr[path->x][path->y] = 'o';
					tmp = path;
					path = path->parent;
					free(tmp);
				}
				arr[p->x][p->y] = 'o';
				
				//V�pis mapy
				for (int i = 0; i < x; i++) {
					for (int j = 0; j < y; j++) {
						if (arr[i][j] == 46) {
							putchar(' ');
						}
						else
						{
							printf("%c", arr[i][j]);
						}

					}
					putchar('\n');
				}
			}
			else
			{
				printf("No solution\n");
			}
			free(p);
			p = NULL;
			clear_hashmap();
			deallocate_front();

			printf("--------------------\n");
			scenario--;
		}
	}

	//Zavretie zo s�boru
	if (fclose(f) == EOF) {
		printf("Subor sa nepodarilo zatvorit\n");
	}

	return 0;
}