/*	Author	Pavol Grofèík
*	Name	AZA Z1
*	Date	7.4.2018
*/

#include <stdio.h>
#include <stdlib.h>

#define NAME "AZA1.in"

//Konštanty pre hash_map
#define D1  150			//* y - súradnica
#define D2  22500			//* dx - súradnica
#define D3  247500			//* dy - súradnica
#define SIZE  2722500		//Úplná ve¾kos hash map-y



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

FRONT *head = NULL; 
FRONT *last = NULL;

int *hash_map = NULL;


//Funkcia inicializuje hash_map - u
void initialize_hash_map() {
	
	hash_map = (FRONT*)calloc(SIZE,sizeof(int));			//Inicializuje hodnoty na 0
	if (hash_map == NULL) {
		printf("Nepodarilo sa pridelit pamat\n");
		return NULL;
	}
}

//Funkcia dealokuje hash_map -u
void deallocate_hash_map() {
	free(hash_map);
	hash_map = NULL;
}

//Funkcia zahashuje danı stav do hash_mapy a urèí èi sa má prida do fronty
int hashing(int x, int y, int dx, int dy) {
	int sum;
	sum = x + D1 * y + D2 * dx + D3 * dy;

	if (hash_map[sum] == 0) {								//Danı stav ešte nebol vygenerovanı
		hash_map[sum] = 1;
		return 1;
	}
	else
	{
		return 0;		//Danı stav u existuje, nepridám ho do fronty
	}
}


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


//Funkcia vytvorí novı uzol do fronty
FRONT* new_node(POINT *node) {
	FRONT *tmp = (FRONT*)malloc(sizeof(FRONT));
	tmp->next = NULL;
	tmp->point = node;

	return tmp;

}

//Funkcia vytvorí novı point
POINT* new_point(int x, int y, int dx, int dy, POINT *parent) {
	POINT* tmp = (POINT*)malloc(sizeof(POINT));
	tmp->x = x;
	tmp->y = y;
	tmp->dx = dx;
	tmp->dy = dy;
	tmp->parent = parent;

	return (POINT*) tmp;
}
//Urobi pointer na koniec a na zaèiatok
void add_to_front(POINT *node) {

	FRONT *tmp = NULL;

	//Pridanie node ak fronta je prázdna
	if (head == NULL) {
		head = last = new_node(node);
		return;
	}
	else
	{
		tmp = last;
		tmp->next = new_node(node);
		tmp->next->prev = tmp;

		//Aktualizovanie posledného
		last = tmp;
	}
}

POINT* pop_from_front() {

	POINT *tmp, *sec;
	FRONT *iterator, *return_node;

	tmp = (POINT*)malloc(sizeof(POINT));

	//Prvı prvok  vzozname
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

		//Uvo¾níme poslednı uzol;
		free(return_node);

		//Posun o jeden uzol spa
		last = iterator;
		
		return (POINT*)tmp;
	}
}

//Funkcia zvaliduje èi panáèik nevyboèil z mapy
int is_valid_move(int x, int y, int size_x, int size_y) {
	if (x < 0 || x >= size_x || y < 0 || y >= size_y || arr[x][y] == 'X') {
		return 0;
	}
	else
	{
		return 1;
	}
}

//Funkcia umocní x na druhú x^2
int power_of_two(int x) {
	int number = x;
	return x * x;
}

//Funkcia overí èi pre danı pohyb je platnı aj dx2 <= 25
int is_free(int dx, int dy) {

	int sum = 0;

	if ((power_of_two(dx) + power_of_two(dy)) <= 25) {
		return 1;
	}
	else
	{
		return 0;
	}
}


//Algoritmus nájdenia najlepšej cesty
POINT* breadt_first_search(POINT *p, int end_x, int end_y, int size_x, int size_y) {

	POINT* new = NULL;
	add_to_front(p);

	while (!is_empty_front()) {

		new = pop_from_front();

		//Cesta bola nájdená
		if (arr[new->x][new->y] == arr[end_x][end_y]) {
			printf("Koniec\n");
			return new;
		}

		//V tejto èasti ošetri hashovaním
		if (is_valid_move(new->dx - 1 , new->dy, size_x, size_y)) {
			//Pridám novovygenerované stavy do fronty a hash mapy

		}

		//Ošetri duplikát #
		if (is_valid_move(new->dx, new->dy, size_x, size_y));


		if (is_valid_move(new->dx + 1, new->y, size_x, size_y)) {

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

	POINT* p = NULL;
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
			//Úprava labyrintu cie¾ového bodu na písmeno Z
			arr[end_x][end_y] = 'Z';

			//Volanie BFS
			//Vytvorenie inicializaèného bodu
			p = new_point(start_x, start_y, 0, 0, NULL);
			path = breadt_first_search(p, end_x, end_y, x, y);

			if (path == NULL) {
				printf("No solution\n");
			}
			else
			{
				//Tu bude vıpis cesty cez dva for-y (funkcia vypis_riesenia())
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