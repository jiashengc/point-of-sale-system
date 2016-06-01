/*
	This file is mainly used for experimentation of new functions to be added in Assignment 2.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//This function rounds doubles to the nearest multiple of 5
double roundToNearest5(const double input){
	return round(input * 20) / 20; 
}

struct Item {
	char code[6];
	char name[20];
	double price;
	int initialQuantity;
	int itemsSold;
};

//To be used in Assignment 2
struct ItemArray {
	int size; //Store the size of array here
	struct Item *array;
}

int main(){
	//Test for roundToNearest5
	/*
	int i;
	for(i = 0; i < 1000; i++){
		double value = (double)i / 100;
		printf("%.2f --> %.2f\n", value, roundToNearest5(value));
	}
	*/
	
	return 0;
}