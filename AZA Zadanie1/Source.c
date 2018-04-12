/*	Author	Pavol Grof��k
*	Name	AZA Z1
*	Date	7.4.2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NAME "AZA1.in"

//Kon�tanty pre hash_map
#define D1  150				//* y - s�radnica
#define D2  22500			//* dx - s�radnica
#define D3  247500			//* dy - s�radnica
#define SIZE  2722500		//�pln� ve�kos� hash map-y



//�trukt�ra ktor� reprezentuje bod v mape //prerobi� na long
typedef struct point {
	int x;
	int y;
	int dx;
	int dy;
	struct point *parent;

}POINT;


//�trukt�ra, ktor� reprezentuje frontu pre BFS
typedef struct front {
	POINT *point;
	struct front *prev;
	struct front *next;

}FRONT;


//Glob�lne premenn� programu
char arr[150][150];

FRONT *head = NULL;
FRONT *last = NULL;

int *hash_map = NULL;


//Funkcia inicializuje hash_map - u
void initialize_hash_map() {

	hash_map = (FRONT*)calloc(SIZE, sizeof(int));			//Inicializuje hodnoty na 0
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

//Funkcia zahashuje dan� stav do hash_mapy a ur�� �i sa m� prida� do fronty
int hashing(int x, int y, int dx, int dy) {
	int sum;
	sum = x + D1 * y + abs(D2 * dx) + abs(D3 * dy);

	if (hash_map[sum] == 0) {								//Dan� stav e�te nebol vygenerovan�
		hash_map[sum] = 1;
		return 1;
	}
	else
	{
		return 0;		//Dan� stav u� existuje, neprid�m ho do fronty
	}
}

//Funkcia dealokuje frontu
void deallocate_front() {
	FRONT* tmp = head, *next;

	if (tmp == NULL) {
		printf("Fronta pr�zdna\n");
		return;
	}
	else
	{
		while (tmp->next != NULL) {
			next = tmp->next;
			free(tmp);
			tmp = next;
		}
		free(tmp);
		head = last = NULL;
	}
}


//Funkcia testuje �i je fronta pr�zdna alebo nie
int is_empty_front() {
	if (head == NULL) {
		return 1;
	}
	else
	{
		return 0;
	}
}


//Funkcia vytvor� nov� uzol do fronty
FRONT* new_node(POINT *node) {
	FRONT *tmp = (FRONT*)malloc(sizeof(FRONT));
	tmp->next = NULL;
	tmp->prev = NULL;
	tmp->point = node;

	return tmp;

}

//Funkcia vytvor� nov� point
POINT* new_point(int x, int y, int dx, int dy, POINT *parent) {

	int new_x = parent->x + parent->dx + dx;
	int new_y = parent->y + parent->dy + dy;

	int new_dx = parent->dx + dx;
	int new_dy = parent->dy + dy;

	POINT* tmp = (POINT*)malloc(sizeof(POINT));

	tmp->x = new_x;
	tmp->y = new_y;
	tmp->dx = new_dx;
	tmp->dy = new_dy;
	tmp->parent = parent;

	return (POINT*)tmp;
}

//Funkcia sl��i na inicializ�ciu po�iato�n�ho pointu
POINT* start_point(int x, int y, int dx, int dy, POINT* parent) {

	POINT* tmp = (POINT*)malloc(sizeof(POINT));
	tmp->x = x;
	tmp->y = y;
	tmp->dx = dx;
	tmp->dy = dy;
	tmp->parent = parent;

	return (POINT*)tmp;

}

//Urobi� pointer na koniec a na za�iatok
void add_to_front(POINT *node) {

	FRONT *tmp = NULL;

	//Pridanie node ak fronta je pr�zdna
	if (head == NULL) {
		head = last = new_node(node);
		return;
	}
	else
	{
		tmp = last;
		tmp->next = new_node(node);
		tmp->next->prev = tmp;

		//Aktualizovanie posledn�ho
		last = tmp->next;
	}
}

POINT* pop_from_front() {

	POINT *tmp, *sec;
	FRONT *iterator, *return_node;

	tmp = (POINT*)malloc(sizeof(POINT));

	//Prv� prvok  vzozname
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

		//Zefekt�vnenie
		iterator = head;
		head = head->next;
		head->prev = NULL;
		return_node = iterator;

		//Prerobi� na funkciu
		tmp->dx = return_node->point->dx;
		tmp->dy = return_node->point->dy;
		tmp->x = return_node->point->x;
		tmp->y = return_node->point->y;
		tmp->parent = return_node->point->parent;

		//Uvo�n�me posledn� uzol;
		free(return_node);

		//Posun o jeden uzol spa�
		return (POINT*)tmp;
	}
}

//Funkcia umocn� x na druh� x^2
int power_of_two(int x) {
	int number = x * x;
	if (number < 0) {
		return number * (-1);
	}
	else
	{
		return number;
	}
}

//Funkcia over� �i pre dan� pohyb je platn� aj dx2 <= 25
int is_free(int dx, int dy) {

	if ((power_of_two(dx) + power_of_two(dy)) <= 25) {
		return 1;
	}
	else
	{
		return 0;
	}
}

//Funkcia zvaliduje �i pan��ik nevybo�il z mapy
int is_valid_move(POINT* p, int x, int y, int size_x, int size_y) {
	int p_x = p->x;	//1
	int p_y = p->y;	//18

	int p_dx = p->dx + x;//-1
	int p_dy = p->dy + y;//6

	printf("p_dx: %d p_dy: %d\n", p_dx, p_dy);
	//Aby sa ne�lo do z�porn�ch indexov
	if ((p_dx < -5) || (p_dx > 5) || (p_dy > 5) || (p_dy < -5)) {
		return 0;
	}

	if (is_free(p_dx, p_dy)) {		//pohyb je <=25
		p_x += p_dx;
		p_y += p_dy;

		//Over� �i sa d� prejs� cez mapu a cez zak�zan� pol��ka
		if (p_x < 0 || p_x >= size_x || p_y < 0 || p_y >= size_y || arr[p_x][p_y] == 'X') {
			return 0;
		}
		else
		{
			return 1;
		}
	}

	return 0;
}



//Algoritmus n�jdenia najlep�ej cesty
POINT* breadt_first_search(POINT *p, int end_x, int end_y, int size_x, int size_y) {

	POINT* new = NULL;
	POINT* tmp = NULL;

	POINT* valid_point = (POINT*)malloc(sizeof(POINT));		//Valid point

	add_to_front(p);

	while (!is_empty_front()) {

		new = pop_from_front();

		//Cesta bola n�jden�
		if (arr[new->x][new->y] == arr[end_x][end_y]) {
			printf("Koniec\n");
			return new;
		}

		//Prid�m novovygenerovan� stavy do fronty a hash mapy

		//Case dx = -1
		//-1,-1
		printf("\nPred pohybom\tx: %d y:%d dx:%d dy:%d\n", new->x, new->y, new->dx, new->dy);
		if (is_valid_move(new, -1, -1, size_x, size_y)) {
			if (hashing(new->x + new->dx - 1, new->y + new->dy - 1, new->dx - 1, new->dy - 1)) {
				tmp = new_point(new->x , new->y,-1,  1, new);
				add_to_front(tmp);
			}
		}//-1,0
		if (is_valid_move(new, -1,0, size_x, size_y)) {
			if (hashing(new->x + new->dx - 1, new->y + new->dy, new->dx - 1, new->dy)) {
				tmp = new_point(new->x , new->y , -1, 0, new);
				add_to_front(tmp);
			}
		}//-1,+1
		if (is_valid_move(new,-1,  1, size_x, size_y)) {
			if (hashing(new->x + new->dx - 1, new->y + new->dy + 1, new->dx - 1, new->dy + 1)) {
				tmp = new_point(new->x, new->y, - 1, 1, new);
				add_to_front(tmp);
			}
		}
		printf("Po prvych 3\tx: %d y:%d dx:%d dy:%d\n", new->x, new->y, new->dx, new->dy);
		//Case: dx = 0
		//0,-1
		if (is_valid_move(new, 0, - 1, size_x, size_y)) {
			if (hashing(new->x + new->dx, new->y + new->dy - 1, new->dx, new->dy - 1)) {
				tmp = new_point(new->x , new->y, 0, -1, new);
				add_to_front(tmp);
			}
		}//0,0
		if (is_valid_move(new,0,0, size_x, size_y)) {
			if (hashing(new->x + new->dx, new->y + new->dy, new->dx, new->dy)) {
				tmp = new_point(new->x, new->y, 0, 0, new);
				add_to_front(tmp);
			}
		}
		//0,1
		if (is_valid_move(new, 0, 1, size_x, size_y)) {
			if (hashing(new->x + new->dx, new->y + new->dy + 1, new->dx, new->dy + 1)) {
				printf("Som tuuuu\n");
				tmp = new_point(new->x, new->y, 0, 1, new);
				add_to_front(tmp);
			}
		}

		printf("Po druhych 3\tx: %d y:%d dx:%d dy:%d\n", new->x, new->y, new->dx, new->dy);
		//Case: dx = +1
		//1,-1
		if (is_valid_move(new,1,-1, size_x, size_y)) {
			if (hashing(new->x + new->dx + 1, new->y + new->dy - 1, new->dx + 1, new->dy - 1)) {
				tmp = new_point(new->x, new->y,1,-1, new);
				add_to_front(tmp);
			}
		}//1,0
		if (is_valid_move(new,1, 0, size_x, size_y)) {
			if (hashing(new->x + new->dx + 1, new->y + new->dy, new->dx + 1, new->dy)) {
				tmp = new_point(new->x, new->y,1,0, new);
				add_to_front(tmp);
			}
		}//1,1

		if (is_valid_move(new, 1, 1, size_x, size_y)) {
			if (hashing(new->x + new->dx + 1, new->y + new->dy + 1, new->dx + 1, new->dy + 1)) {
				tmp = new_point(new->x, new->y,1,1, new);
				add_to_front(tmp);
			}
		}
		printf("Po iter�cii\tx: %d y:%d dx:%d dy:%d\n", new->x, new->y, new->dx, new->dy);

	}

	//Ak sa rie�enie nena�lo
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

	int scenario, x, y, start_x, start_y, end_x, end_y, c, i, j;
	FILE *f = NULL;

	POINT* p = NULL, *tmp, *next;
	POINT *path = NULL;


	if ((f = fopen(NAME, "r")) == NULL) {				//Otvorenie s�boru pre na��tavanie
		printf("Nepodarilo sa otvorit subor\n");
		return -1;
	}


	while (!feof(f)) {									//Na��tavanie scen�rov zo s�boru
		fscanf(f, "%d\n\n", &scenario);

		while (scenario > 0) {
			fscanf(f, "%d", &x);			//Rozmer x po�a
			fscanf(f, "%d\n", &y);			//Rozmer y po�a

			fscanf(f, "%d", &start_x);		//Za�iatok x v poli
			fscanf(f, "%d\n", &start_y);	//Za�iatok y v poli

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
			//�prava labyrintu cie�ov�ho bodu na p�smeno Z
			arr[end_x][end_y] = 'Z';


			//Inicializ�cia hashmapy
			initialize_hash_map();


			//Volanie BFS
			//Vytvorenie inicializa�n�ho bodu
			p = start_point(start_x, start_y, 0, 0, NULL);
			hashing(start_x, start_y, 0, 0);

			path = breadt_first_search(p, end_x, end_y, x, y);

			if (path == NULL) {
				printf("No solution\n");
			}
			else
			{
				//Tu bude v�pis cesty cez dva for-y (funkcia vypis_riesenia())
				printf("Solution was found!\n");
				while (path->parent != NULL) {
					printf("x:%d y:%d dx:%d, dy:%d\n", path->x, path->y, path->dx, path->dy);
					path = path->parent;
				}
				printf("x:%d y:%d dx:%d, dy:%d\n", p->x, p->y, p->dx, p->dy);
			}

			printf("--------------------\n");
			deallocate_front();
			deallocate_hash_map();
			scenario--;
		}
	}

	//Zavretie zo s�boru
	if (fclose(f) == EOF) {
		printf("Subor sa nepodarilo zatvorit\n");
	}

	return 0;
}