#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACTOR_NAME 60
#define MAX_MOVIE_NAME 100
#define R_NUMBER 31
#define MAX_ACTOR_NUM 300000
#define MAX_MOVIE_NUM 15000
#define QUEUE_SIZE 500000

typedef struct actor{
	char name[MAX_ACTOR_NAME]; // akt�r�n ad�
	struct movie** movies; // akt�r�n oynad��� filmerin adreslerini tutan dizi
	unsigned char num_movies; // bir akt�r en fazla 255 filmde oynar kabul edildi --
	unsigned int visited:1; // bu akt�r�n daha �nceden gezilmi� oldu�unu g�steren visited de�i�keni
	struct movie* parent_movie; // graf �zerinde gezilen yolu yazd�rmaya yard�mc� olacak parent_movie de�i�keni
}ACTOR;

typedef struct movie{ // ayn� struct yap�s�n�n film i�in olan�
	char name[MAX_MOVIE_NAME];
	struct actor** actors;
	unsigned char num_actors; // bir filmde en fazla 255 akt�r oynar kabul edildi --
	unsigned int visited:1;
	struct actor* parent_actor;
}MOVIE;

typedef struct que_actor{
	ACTOR* array[QUEUE_SIZE]; // kuyruk dizisi
	int start; // kuyru�un ilk eleman�n� tutan indis.
	int end; // kuyru�a eklenecek eleman�n eklenecek indisini tutan de�i�ken.
}Q_ACTOR;
typedef struct que_movie{ // ayn� kuyru�un film i�in olan�
	MOVIE* array[QUEUE_SIZE]; 
	int start;
	int end;
}Q_MOVIE;	


unsigned long int insertActor(char*, ACTOR*); // akt�r ekleyip ekledikten sonra eklenen indisin ( key ) de�erini d�nd�ren fonksiyon
ACTOR* searchActor(char*, ACTOR*); // akt�r� aray�p, bulunursa struct actor olarak akt�r�n adresini, bulunamaz ise NULL d�nd�ren fonksiyon
unsigned long int power(int base, int exponent); // �st alma fonksiyonu
void bfs(ACTOR*, MOVIE*, char*, char*); // breadth first search yaparak ka� ad�mda bulundu�unu ve bulunurken izlenen yolu ekrana yazd�ran fonksiyon
void enqueActor(ACTOR*, Q_ACTOR*); // akt�r kuyru�una akt�r ekleyen fonksiyon
void enqueMovie(MOVIE*, Q_MOVIE*); // movie kuyru�una movie ekleyen fonksiyon
ACTOR* dequeActor(Q_ACTOR*); // akt�r kuyru�undan akt�r�n adresini d�nd�ren fonksiyon
MOVIE* dequeMovie(Q_MOVIE*); // movie kuyru�undan movienin adresini d�nd�ren fonksiyon
void displayPath(ACTOR*, int); // bulunan yolu ( tersten s�ral� ) ba�tan sona yazd�rmak i�in kulland���m, yolu ekrana yazan fonksiyon



int main(){
	
	char c; // dosyadan karakter-karakter okuma i�leminde kullan�lan buffer
	char tmp_movie[ MAX_MOVIE_NAME ]; // dosyadan anl�k olarak okunan film
	char tmp_actor[ MAX_ACTOR_NAME ]; // dosyadan anl�k olarak okunan akt�r
	int i; // d�ng�lerde kullan�lan indis de�i�keni
	int movie_index = 0; // hangi sat�rdaki filmin okundu�unu g�steren de�i�ken ( �rn: movie_index = 50 --> 50. 49. sat�rdaki filmin okuma i�lemi )
	unsigned long int key; // akt�r�n hashmapteki indisini tutan de�i�ken
	
	// movies ve actors dizilerinin tan�mlanmas� ve ayarlanmas� --
	MOVIE* movies = (MOVIE*) malloc (sizeof(MOVIE) * MAX_MOVIE_NUM); // movie dizisii
	ACTOR* actors = (ACTOR*) malloc (sizeof(ACTOR) * MAX_ACTOR_NUM); // actor dizisi
	
	
	// dosyadan okuma i�lemi --
	FILE* fp = fopen("data.txt", "r");
	c = getc(fp);
	while(c != EOF){
		i = 0;
		while(c != '/'){
			tmp_movie[i] = c;
			i++;
			c = getc(fp);
		}
		tmp_movie[i] = '\0';	
		strcpy(movies[ movie_index ].name, tmp_movie);
		movies[ movie_index ].num_actors = 0;
		movies[ movie_index ].visited = 0;
		
		movies[ movie_index ].actors = (ACTOR**) malloc(sizeof(ACTOR*));
		while(c != '\n' && c != EOF){
			i = 0;
			c = getc(fp);
			while(c != '/' && c != '\n' && c != EOF){
				tmp_actor[i] = c;
				i++;
				c = getc(fp);
			}
			tmp_actor[i] = '\0';
			
			key = insertActor(tmp_actor, actors);
			
			movies[ movie_index ].actors = (ACTOR**) realloc(movies[ movie_index ].actors, sizeof(ACTOR*) * ( 1 + movies[ movie_index ].num_actors));
			if(actors[ key ].num_movies == 0){
				actors[ key ].movies = (MOVIE**) malloc(sizeof(MOVIE*));
			}
			else{
				actors[key].movies = (MOVIE**) realloc( actors[key].movies, ( 1 + actors[key].num_movies ) * sizeof(MOVIE*) );	
			}
			
			movies[ movie_index ].actors[ movies[ movie_index ].num_actors ] = &actors[key];
			actors[ key ].movies[ actors[ key ].num_movies ] = &movies[ movie_index ];
			
			actors[key].num_movies++;
			movies[ movie_index ].num_actors++;
		}
		
		//printf("Film Adi : %s\n", movies[movie_index].name);
		movie_index++;
		c = getc(fp);
	}
	fclose(fp);
	
	char source_actor[MAX_ACTOR_NAME]; // kullan�c�dan al�nacak ba�lang�� akt�r�
	char dest_actor[MAX_MOVIE_NAME]; // kullan�c�dan al�nacak hedef akt�r�
	
	
	printf("Enter first actor: ");
	gets(source_actor);
	printf("Enter destination actor: ");
	gets(dest_actor);
	
	bfs(actors, movies, source_actor, dest_actor);
	
	free(movies);
	free(actors);
	
	return 0;
}

unsigned long int insertActor(char* actor_name, ACTOR* actor_list){
	
	unsigned int i = 0; // linear probing i�in kullan�lan de�i�ken
	unsigned long int key = 0; // strinden olu�turulan key say�s�
	
	while(actor_name[i] != '\0'){
		key += actor_name[i] * power(R_NUMBER, i);
		i++;
	}
	key = key % MAX_ACTOR_NUM;
	
	i = 0;
	while( i < MAX_ACTOR_NUM && actor_list[ (i + key) % MAX_ACTOR_NUM ].name[0] != '\0' && 
			strcmp( actor_list[ (i + key) % MAX_ACTOR_NUM ].name, actor_name ) != 0 )		
		i++;
	
	// bulunma durumu --
	if( strcmp( actor_list[ (key + i) % MAX_ACTOR_NUM ].name, actor_name ) == 0 )
		return ( (key + i) % MAX_ACTOR_NUM); // bulundu�u indis de�eri olan key'i d�nd�r�yoruz --
	
	if( i < MAX_ACTOR_NUM ){
		strcpy( actor_list[ (key + i) % MAX_ACTOR_NUM ].name, actor_name );
		actor_list[ (key + i) % MAX_ACTOR_NUM ].visited = 0;
		actor_list[ (key + i) % MAX_ACTOR_NUM ].num_movies = 0;
		return ( (key + i) % MAX_ACTOR_NUM); // ekledi�imiz indis de�eri olan key'i d�nd�r�yoruz --
	}
	else{
		printf("Error: Map-Size insufficient.\n");
	}
}

ACTOR* searchActor(char* actor_name, ACTOR* actor_list){
	
	unsigned int i = 0; // linear probing i�in kullan�lan de�i�ken
	unsigned long int key = 0; // strinden olu�turulan key say�s�
	
	while(actor_name[i] != '\0'){
		key += actor_name[i] * power(R_NUMBER, i);
		i++;
	}
	key = key % MAX_ACTOR_NUM;
	
	i = 0;
	while( i < MAX_ACTOR_NUM && actor_list[ (i + key) % MAX_ACTOR_NUM ].name[0] != '\0' && 
			strcmp( actor_list[ (i + key) % MAX_ACTOR_NUM ].name, actor_name ) != 0 )		
		i++;
	
	// bulunma durumu --
	if( strcmp( actor_list[ (key + i) % MAX_ACTOR_NUM ].name, actor_name ) == 0 )
		return &actor_list[ ( (key + i) % MAX_ACTOR_NUM) ]; // akt�r�n adresini d�nd�r�yoruz.
	
	printf("Error: Actor not found.\n");
	return NULL;
}

unsigned long int power(int base, int exponent){ // bizim �ss�m�z 0 dan k���k olamayaca�� i�in yeterli �s alma fonksiyonu.
	unsigned int result = 1;
	if(exponent > 0){
		int i;
		for(i = 0; i < exponent; i++)
			result *= base;
	}
	return result;
}

void enqueActor(ACTOR* actor, Q_ACTOR* que){
	que->array[ que->end ] = actor;
	que->end++;
	actor->visited = 1;
}

void enqueMovie(MOVIE* movie, Q_MOVIE* que){
	que->array[ que->end ] = movie;
	que->end++;
	movie->visited = 1;
}

ACTOR* dequeActor(Q_ACTOR *que){
	if(que->start == que->end){
		printf("Error: Actor queue is empty.");
		return NULL; // que bo�tur.
	}
	que->start++;
	return que->array[ que->start - 1 ];
}

MOVIE* dequeMovie(Q_MOVIE *que){
	if(que->start == que->end){
		printf("Error: Movie queue is empty.");
		return NULL; // que bo�tur.
	}
	que->start++;
	return que->array[ que->start - 1 ];
}


void bfs(ACTOR* actors, MOVIE* movies, char* source, char* dest){
	
	Q_ACTOR* q_actor = (Q_ACTOR*) malloc(sizeof(Q_ACTOR)); // akt�r kuyru�u
	Q_MOVIE* q_movie = (Q_MOVIE*) malloc(sizeof(Q_MOVIE)); // film kuyru�u
	q_actor->start = 0; // akt�r kuyru�unun start indisi 0 lan�yor
	q_actor->end = 0; // akt�r kuyru�unun end indisi 0 lan�yor
	q_movie->start = 0; // movie kuyru�unun ba�lang�� de�eri 0 lan�yor
	q_movie->end = 0; // movie kuyru�unun end de�eri 0 lan�yor
	int distance = 0; // 2 akt�r�n birbirine olan uzakl���
	int found = 0; // bulundu de�i�keni
	int i; // d�ng� de�i�keni
	

	
	ACTOR* tmp_actor = searchActor(source, actors); // Armstrond Darrell
	ACTOR* dest_actor = searchActor(dest, actors); // Beatty Warren
	if(tmp_actor == NULL || dest_actor == NULL){
		printf("Error: Destination or Source actors missing.");
		return;
	}
	MOVIE* tmp_movie = (MOVIE*) malloc(sizeof(MOVIE));
	enqueActor(tmp_actor, q_actor);
	while( distance <= 6 && (!found) ){

		while( q_actor->start < q_actor->end && (!found) ){
			tmp_actor = dequeActor(q_actor);
			//printf("Aktor: %s Oynadigi Film Sayisi: %d\n", tmp_actor->name, tmp_actor->num_movies);
			if( dest_actor == tmp_actor ){
				found = 1;
			}
			else{
				for(i = 0; i < tmp_actor->num_movies; i++){
					if( tmp_actor->movies[i]->visited == 0 ){
						tmp_actor->movies[i]->parent_actor = tmp_actor;
						enqueMovie(tmp_actor->movies[i], q_movie);
					}
				}
			}
		}
		if(found == 0){
			distance++;
			while( q_movie->start < q_movie->end ){
				tmp_movie = dequeMovie(q_movie);
				for(i = 0; i < tmp_movie->num_actors; i++){
					if( tmp_movie->actors[i]->visited == 0 ){
						tmp_movie->actors[i]->parent_movie = tmp_movie;
						enqueActor(tmp_movie->actors[i], q_actor);
					}
				}
			}
		}
	}
	
	printf("Source Actor     : %s\n", source);
	printf("Destination Actor: %s\n", dest);
	
	
	if(distance > 6){	
		printf("There is no connection.\n");
	}else{
		printf("There is connection with distance: %d\n", distance);
	}
	
	printf("Path:\n");
	displayPath(dest_actor, distance);
	
	
	free(q_actor);
	free(q_movie);
}

void displayPath(ACTOR* dest_actor, int distance){
	
	if(distance > 1)
		displayPath(dest_actor->parent_movie->parent_actor, distance - 1);
	
	if(distance > 0)
		printf("%s - %s : \"%s\"  \n", dest_actor->name, dest_actor->parent_movie->parent_actor->name, dest_actor->parent_movie->name);
}
