#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SAMPLE_NAME_LENGTH 50
#define MAX_STR_LENGTH 50
#define MAX_NUMOF_FILE 50
#define MAX_CONTENT_LENGTH 1000
#define TABLE_SIZE 11
#define R_NUMBER 31

char* concatDirectory(char*, char*);
void createHashTable(char hashTable[TABLE_SIZE][MAX_SAMPLE_NAME_LENGTH], char*, char*);
void insert2Table(char*, unsigned long int, char hashTable[TABLE_SIZE][MAX_SAMPLE_NAME_LENGTH]); 
void displayHashTable(char hashTable[TABLE_SIZE][MAX_SAMPLE_NAME_LENGTH]);
void addDocument(char*, char*, char hashTable[TABLE_SIZE][MAX_SAMPLE_NAME_LENGTH]);
unsigned long int power(int, int);

int main(int argc, char *argv[]) {
	
	char hashTable[TABLE_SIZE][MAX_SAMPLE_NAME_LENGTH]; // hash tablosu
	char run = 1; // mainin �al���r halde kalmas�n� sa�layan de�i�ken
	int i; // d�ng� de�i�keni
	char docName[MAX_SAMPLE_NAME_LENGTH]; // okunan d�k�man�n ismi
	char docDirectory[MAX_SAMPLE_NAME_LENGTH]; // okunan d�k�man�n lokasyonu
	
	for(i = 0; i < TABLE_SIZE; i++){
		hashTable[i][0] = '\0';
	}
	createHashTable(hashTable, "index.txt", "directory/");	

	while(run != '0'){
		printf("Display Hash-Table        : 1\n");
		printf("Add new doc to Hash-Table : 2\n");
		printf("Exit                      : 0\n");
		scanf(" %c", &run);
		switch(run){
			case '0': {
				printf("Program ended.");
				break;
			}
			case '1': {
				displayHashTable(hashTable);
				break;
			}
			case '2': {
				printf("Enter document's name : ");
				scanf("%s",docName);
				printf("Enter document's directory : ");
				scanf("%s", docDirectory);
				addDocument(docName, docDirectory, hashTable);
				break;
			}
			default: {
				printf("error : Entered undefined input.\n");
			}
		} 			     
	}
	
	return 0;
}

void createHashTable(char hashTable[TABLE_SIZE][MAX_SAMPLE_NAME_LENGTH], char* fileName, char* directory) {
	
	FILE* samples = NULL; // index dosyas�
	FILE* doc = NULL; // d�k�manlar
	char sampleNames[MAX_NUMOF_FILE][MAX_SAMPLE_NAME_LENGTH]; // sampleden okunan dosyalar�n isimlerinin tutulaca�� matris
	int numOfFiles = 0; // samples i�erisindeki dosya say�s�
	int i, j; // d�ng� de�i�kenleri
	unsigned long int key, sum; // key ve key olu�turmada kullan�lan yard�mc� sum de�eri
	char str[MAX_STR_LENGTH]; // key olu�turmada kullan�lan string dizisi
	char sampleFileName[MAX_SAMPLE_NAME_LENGTH]; // sampleden okunan string ile olu�turulan dosya uzant�s�n�n tutuldu�u string
		
	samples = fopen(fileName, "r");
	if(!samples){
		printf("File not found.\n");
		return;
	}
	
	while(!feof(samples)){
		fscanf(samples, "%s", &sampleNames[numOfFiles]);
		//printf("%s\n", sampleName);
		numOfFiles++;	
	}
	fclose(samples);
	
	j = 0;
	while(j < numOfFiles){
		
		strcpy(sampleFileName, concatDirectory(directory, sampleNames[j]));
		doc = fopen(sampleFileName, "r");
		if(doc == NULL){
			printf("File not found (%s)\n", sampleFileName);
			j++;
			continue;
		}
		key = 0;
		while(!feof(doc)){
			fscanf(doc, "%s", &str);
			sum = 0;
			i = 0;
			while(str[i] != '\0'){
				sum += str[i] * power(R_NUMBER, strlen(str) - i - 1);
				i++;
			}
			key += sum;
		}
		fclose(doc);
		// key de�erini hesaplad�k, tabloya ekleme i�lemi :
		insert2Table(sampleNames[j], key, hashTable);
		j++;
	}
}

void insert2Table(char* itemName, unsigned long int key, char hashTable[TABLE_SIZE][MAX_SAMPLE_NAME_LENGTH]){
	
	unsigned int MM = TABLE_SIZE - 1; // 2. key de�erinin hesaplanmas� i�in kullan�lan de�er
	unsigned long int key1 = key % TABLE_SIZE; // h1key 1 de�eri
	unsigned long int key2 = 1 + ( key % MM ); // h2key 2 de�eri
	int i = 0, j = 0; // d�ng� de�i�kenleri
	
	key = ( key1 + i * key2 ) % TABLE_SIZE;
	
	while( ( i < TABLE_SIZE ) && ( hashTable[ key ][0] != '\0' ) ){
		i++;
		key = ( key1 + i * key2 ) % TABLE_SIZE;
	}
	if( i < TABLE_SIZE ){
		strcpy(hashTable[key], itemName);
		return;
	}	
}

void addDocument(char* docName, char* docDirectory, char hashTable[TABLE_SIZE][MAX_SAMPLE_NAME_LENGTH]){
	
	FILE* doc = NULL; // eklenecek d�k�man
	FILE* samples = NULL; // index dosyas�
	int i = 0, j = 0; // d�ng� de�i�kenleri
	unsigned long int sum, key; // key de�erinin hesaplamnas�nda kullan�lan de�i�kenler
	char str[MAX_STR_LENGTH]; // d�k�mandan okumada kullan�lacak string
	char doc1Content[MAX_CONTENT_LENGTH] = ""; // eklenecek d�k�man�n i�eri�i
	char doc2FileName[MAX_SAMPLE_NAME_LENGTH];// veri taban�nda i�eri�i kontrol edilecek d�k�man�n ismi
	char doc1FileName[MAX_SAMPLE_NAME_LENGTH];// eklenecek d�k�man�n ismi
	char c;
	
	strcpy(doc1FileName, concatDirectory(docDirectory, docName));
	
	doc = fopen(doc1FileName, "r");
	if(doc == NULL){
		printf("error : Document not found (%s)\n", doc1FileName);
		return;
	}
	key = 0;
	while(!feof(doc)){
		fscanf(doc, "%s", &str);
		i = 0;
		sum = 0;
		while(str[i] != '\0'){
			sum += str[i] * power(R_NUMBER, strlen(str) - i - 1);
			i++;
		}
		key += sum;	
	}
	fseek(doc, 0, SEEK_SET);
	i = 0;
	
	while(!feof(doc)){	
		c = fgetc(doc);
		if(c != EOF)
			doc1Content[i] = c;
		i++;	
	}	
	fclose(doc);
	doc1Content[i] = '\0';
	
	unsigned int MM = TABLE_SIZE - 1;// 2. key de�erinin hesaplanmas� i�in kullan�lan de�er
	unsigned long int key1 = key % TABLE_SIZE;// h1key de�eri
	unsigned long int key2 = 1 + ( key % MM );// h2key de�eri
	i = 0;
	key = ( key1 + i * key2 ) % TABLE_SIZE;
	printf("Start Key %lu\n", key);
	while( ( i < TABLE_SIZE ) && ( hashTable[ key ][0] != '\0') ){
		
		strcpy(doc2FileName, concatDirectory("directory/", hashTable[key]));
		j = 0;
		doc = fopen(doc2FileName, "r");
		while(!feof(doc) && doc1Content[j] != '\0' && fgetc(doc) == doc1Content[j]){
			j++;	
		}
		if(!feof(doc))
			fgetc(doc);
		if(doc1Content[j] == '\0' && feof(doc)){
			printf("error : Document's content is same with (%s)\n", hashTable[key]);
			fclose(doc);
			return;
		}else{
			i++;
			key = ( key1 + i * key2 ) % TABLE_SIZE;
			printf("Collusion for document %s New Key : %lu\n", docName, key);
		}	
	}
	fclose(doc);
	if(hashTable[ key ][0] == '\0'){
		strcpy(hashTable[key], docName);
		strcpy(doc1FileName, concatDirectory("directory/", docName));
		doc = fopen(doc1FileName, "w");
		fputs(doc1Content, doc);
		fclose(doc);
		doc = fopen("index.txt", "a");
		fputs(docName, doc);
		fputc('\n', doc);
		fclose(doc);
		printf("Document successfully added to Hash-Map.\n");
		return;
	}
	
	
	printf("error : Hash-Table is full.\n");
	
}

char* concatDirectory(char* directory, char* sampleName){
	
	char* newString = malloc( strlen(directory) + strlen(sampleName) + 2 );
	strcpy(newString, "");
	strcat(newString, directory);
	strcat(newString, sampleName);
	strcat(newString, ".txt");
	return newString;
}

void displayHashTable(char hashTable[TABLE_SIZE][MAX_SAMPLE_NAME_LENGTH]){
	int i;
	printf("HASH TABLE\n");
	for(i = 0; i < TABLE_SIZE; i++){
		printf("Table %d => %s\n", i, hashTable[i]);
	}	
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
