/*	Author	Pavol Grofèík
*	Name	AZA Z1
*	Date	7.4.2018
*/

#include <stdio.h>
#include <stdlib.h>

#define NAME "AZA1.in"



//Štruktúra ktorá reprezentuje bod v mape //prerobi na long
typedef struct point {
	int x;
	int y;
	int dx;
	int dy;
	struct point *parent;

}POINT;


//Štruktúra, ktorá reprezentuje frontu pre BFS
typedef struct front {
	POINT *point;
	struct front *prev;
	struct front *next;

}FRONT;


//Globálne premenné programu
char arr[150][150];
char goal[150][150];

FRONT *head = NULL; 
FRONT *last = NULL;


//Funkcia testuje èi je fronta prázdna alebo nie
int is_empty_front() {
	if (head == NULL) {
		return 1;
	}
	else
	{
		return 0;
	}
}


//Funkcia vytvorí nový uzol do fronty
FRONT* novy_node(POINT *node) {
	FRONT *tmp = (FRONT*)malloc(sizeof(FRONT));
	tmp->next = NULL;
	tmp->point = node;

	return tmp;

}

//Funkcia vytvorí nový point
POINT* novy_point(int x, int y, int dx, int dy, POINT *parent) {
	POINT* tmp = (POINT*)malloc(sizeof(POINT));
	tmp->x = x;
	tmp->y = y;
	tmp->dx = dx;
	tmp->dy = dy;
	tmp->parent = parent;

	return (POINT*) tmp;
}
//Urobi pointer na koniec a na zaèiatok
void pridaj_do_fronty(POINT *node) {

	FRONT *tmp = NULL;

	//Pridanie node ak fronta je prázdna
	if (head == NULL) {
		head = last = novy_node(node);
		return;
	}
	else
	{
		tmp = last;
		tmp->next = novy_node(node);
		tmp->next->prev = tmp;

		//Aktualizovanie posledného
		last = tmp;
	}
}

POINT* vyber_z_fronty() {

	POINT *tmp, *sec;
	FRONT *iterator, *return_node;

	tmp = (POINT*)malloc(sizeof(POINT));

	//Prvý prvok  vzozname
	if (head == last) {
		if (head != NULL) {
			tmp->dx = head->point->dx;
			tmp->dy = head->point->dy;
			tmp->x = head->point->x;
			tmp->y = head->point->y;
			tmp->parent = head->point->parent;

			free(head);
			head = last = NULL;
			return (POINT*)tmp;
		}
		return NULL;
	}
	else
	{
		//Aktualizovanie zoznamu
		iterator = last->prev;
		return_node = last;
		iterator->next = NULL;

		//Prerobi na funkciu
		tmp->dx = return_node->point->dx;
		tmp->dy = return_node->point->dy;
		tmp->x = return_node->point->x;
		tmp->y = return_node->point->y;
		tmp->parent = return_node->point->parent;

		//Uvo¾níme posledný uzol;
		free(return_node);

		//Posun o jeden uzol spa
		last = iterator;
		
		return (POINT*)tmp;
	}
}


//Algoritmus nájdenia najlepšej cesty
POINT* breadt_first_search(POINT *p, int end_x, int end_y) {

	POINT* new = NULL;
	pridaj_do_fronty(p);

	while (!is_empty_front()) {

		new = vyber_z_fronty();

		//Cesta bola nájdená
		if (arr[new->x][new->y] == arr[end_x][end_y]) {
			printf("Koniec\n");
			return new;
		}


	}
	
	printf("No solution\n");
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
	putchar('\n');
}


int main(void) {

	int scenario, x, y, start_x, start_y, end_x, end_y, c,i,j;
	FILE *f = NULL;

	POINT* p = (POINT*)malloc(sizeof(POINT));
	p->x = 0;
	p->y = 0;
	p->dx = 0;
	p->dy = 0;
	p->parent = NULL;
	POINT *path = NULL;

	
	if ((f = fopen(NAME, "r")) == NULL) {				//Otvorenie súboru pre naèítavanie
		printf("Nepodarilo sa otvorit subor\n");
		return -1;
	}


	while (!feof(f)) {									//Naèítavanie scenárov zo súboru
		fscanf(f, "%d\n\n", &scenario);

		while (scenario > 0) {
			fscanf(f, "%d", &x);			//Rozmer x po¾a
			fscanf(f, "%d\n", &y);			//Rozmer y po¾a

			fscanf(f, "%d", &start_x);		//Zaèiatok x v poli
			fscanf(f, "%d\n", &start_y);	//Zaèiatok y v poli
		
			fscanf(f, "%d", &end_x);		//Koniec x v poli
			fscanf(f, "%d\n", &end_y);		//Koniec y v poli

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

			//Some code snippet of BFS
			vypis_labyrintu(x, y);
			//Volanie BFS
			path = breadt_first_search(p, end_x, end_y);
			if (path == NULL) {
				printf("No solution\n");
			}
			else
			{
				//Tu bude výpis cesty cez dva for-y
			}
			
			printf("--------------------\n");
			scenario--;
		}
	}

	//Zavretie zo súboru
	if (fclose(f) == EOF) {
		printf("Subor sa nepodarilo zatvorit\n");
	}

	return 0;
}