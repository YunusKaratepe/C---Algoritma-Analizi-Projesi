#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

// STRUCT ----------------------->>>
typedef struct dot {
	int x;
	int y;
}DOT;
//---------------------------------
// FUNCTION DESCRIPTIONS ----------------------->>>
DOT* createSpace(int);
void sortBy_x(int, DOT*);
float distance(DOT, DOT);
float bruteForce(DOT*, int, DOT[]);
float closest_Pair(DOT*, int, DOT[]);
void displaySpace(int, DOT*);
//---------------------------------

// MAIN FUNCTION ----------------------->>>

int main() {

	int numOf_Dots; // uzaydaki nokta say�s�
	float min_Dist; // en yak�n uzakl�k
	printf("Enter the number of dots : ");
	scanf("%d", &numOf_Dots);
	DOT* space = createSpace(numOf_Dots); // noktalar uzay�
	DOT closestPair[2]; // en yak�n ikili
	
	printf("\nENTERED DOTS : \n");
	displaySpace(numOf_Dots, space);
	printf("\nSORTED BY X : \n");
	sortBy_x(numOf_Dots, space);
	displaySpace(numOf_Dots, space);
	printf("\nCLOSEST DOTS : \n");
	min_Dist = closest_Pair(space, numOf_Dots, closestPair);

	printf("\nFirst  dot  -> x : %d y : %d\n", closestPair[0].x, closestPair[0].y);
	printf("Second dot  -> x : %d y : %d\n", closestPair[1].x, closestPair[1].y);
	printf("Distance : %.4f\n", min_Dist);


	free(space);

	return 0;
}

//---------------------------------
// FUNCTIONS ----------------------->>>

DOT* createSpace(int numOf_Dots) {
	int i; // d�ng� de�i�keni
	DOT* space = (DOT*)malloc(sizeof(DOT) * numOf_Dots); // kullan�c�dan noktalar al�nd�ktan sonra
														 // d�nd�r�lecek noktalar uzay�
	for (i = 0; i < numOf_Dots; i++) {
		printf("Enter x-y coordinates for dot - %d : ", i + 1);
		scanf("%d%d", &space[i].x, &space[i].y);
	}
	return space;
}
void sortBy_x(int numOf_Dots, DOT* space) {
	int i, j; // d�ng� de�i�kenleri
	int keyX, keyY; // insertion sort keyleri
	for (i = 1; i < numOf_Dots; i++) {
		keyX = space[i].x;
		keyY = space[i].y;
		j = i - 1;
		while (j >= 0 && space[j].x > keyX) {
			space[j + 1].x = space[j].x;
			space[j + 1].y = space[j].y; 
			j--;
		}
		space[j + 1].x = keyX;
		space[j + 1].y = keyY;
	}
}
void displaySpace(int numOf_Dots, DOT* space) {
	int i; // d�ng� de�i�keni
	for (i = 0; i < numOf_Dots; i++) {
		printf("Coordinates -> x : %d - y : %d\n", space[i].x, space[i].y);
	}
}
float distance(DOT d1, DOT d2) {
	return sqrt((d1.x - d2.x) * (d1.x - d2.x) + (d1.y - d2.y) * (d1.y - d2.y));
}
float bruteForce(DOT* space, int numOf_Dots, DOT closestPair[]) {
	float min = distance(space[0], space[1]); // min de�erini ilk elemanlar�n mesafesi olarak belirledim.
	closestPair[0].x = space[0].x;
	closestPair[0].y = space[0].y;
	closestPair[1].x = space[1].x;
	closestPair[1].y = space[1].y;
	
	if (numOf_Dots >= 3) {
		int i, j; // d�ng� de�i�kenleri
		for (i = 0; i < numOf_Dots; i++)
			for (j = i + 1; j < numOf_Dots; j++)
				if (distance(space[i], space[j]) < min){
					min = distance(space[i], space[j]);
					closestPair[0].x = space[i].x;
					closestPair[0].y = space[i].y;
					closestPair[1].x = space[j].x;
					closestPair[1].y = space[j].y;
				}		
	}
	return min;
}
float closest_Pair(DOT* space, int numOf_Dots, DOT closestPair[]) {

	
	float min = 9999; // minimum de�i�keni
	
		if(numOf_Dots > 3){
			
			int median = numOf_Dots / 2;	 // medyan de�eri
			float min_left = 9999;			 // sol taraftaki minimumu tutan de�er 
			float min_right = 9999;			 // sol taraftaki minimumu tutan de�er 
			int i, j;						 // d�ng� de�i�kenleri
									 	
			DOT closestPair_Left[2]; 
			DOT closestPair_Right[2];
			min_left = closest_Pair(space, median, closestPair_Left);
			min_right = closest_Pair(&space[median], (numOf_Dots - median), closestPair_Right);
			
			if(min_left < min_right){
				closestPair[0].x = closestPair_Left[0].x;
				closestPair[0].y = closestPair_Left[0].y;
				closestPair[1].x = closestPair_Left[1].x;
				closestPair[1].y = closestPair_Left[1].y;
				min = min_left;
			}else{
				closestPair[0].x = closestPair_Right[0].x;
				closestPair[0].y = closestPair_Right[0].y;
				closestPair[1].x = closestPair_Right[1].x;
				closestPair[1].y = closestPair_Right[1].y;
				min = min_right;
			}

			DOT* leftDots = (DOT*)malloc(sizeof(DOT)); // 1 elemanl�k yer a�t�k daha sonras�nda realloc ile art�raca��z.
			DOT* rightDots = (DOT*)malloc(sizeof(DOT));// leftDots ve rightDots solda ve sa�da min kadar uzakl�ktan 
														// yak�n olan elemanlar olacaklar.
			int countLeft = 0;  // leftDots dizisinde ka� eleman oldu�unu tutan de�i�ken
			int countRight = 0; // rightDots dizisinde ka� eleman oldu�unu tutan de�i�ken
	
			for (i = median - 1; ( ( i >= 0 ) && ( space[i].x >= space[median].x - min ) ) ; i--) {
					countLeft++;
					leftDots = (DOT*)realloc(leftDots, sizeof(DOT) * countLeft);
					leftDots[countLeft - 1].x = space[i].x;
					leftDots[countLeft - 1].y = space[i].y;
			}
			for (i = median; ( ( i < numOf_Dots ) && ( space[i].x <= space[median].x + min ) ) ; i++) {
					countRight++;
					rightDots = (DOT*)realloc(rightDots, sizeof(DOT) * countRight);
					rightDots[countRight - 1].x = space[i].x;
					rightDots[countRight - 1].y = space[i].y;
			}
				
			float distL_R = 9999; // soldaki noktan�n sa�daki noktaya uzakl���n� tutacak de�iken
			for (i = 0; i < countLeft; i++) {
				for (j = 0; j < countRight; j++) {
					distL_R = distance(leftDots[i], rightDots[j]);
					if (distL_R < min) {
						min = distL_R;
						closestPair[0].x = leftDots[i].x;
						closestPair[0].y = leftDots[i].y;
						closestPair[1].x = rightDots[j].x;
						closestPair[1].y = rightDots[j].y;
					}
				}
			}
			
		}else if (numOf_Dots > 1){
			min = bruteForce(space, numOf_Dots, closestPair);
		}
	return min;
}
//---------------------------------
