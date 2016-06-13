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
};

int countLines(FILE *file){
	//Starts from the first line, so start from 1
	int result = 1; 
	while(!feof(file)){
		char c = fgetc(file);
		if(c == '\n'){
			result++;
		}
	}
	//Returns file pointer to beginning
	rewind(file);
	return result;
}

struct ItemArray readFile(FILE *file){
	struct ItemArray result;
	result.size = countLines(file);
	result.array = malloc(sizeof(struct Item) * result.size);
	int i;
	for(i = 0; i < result.size; i++){
		fscanf(file, "%5c;%[^;];%lf;%d\n",
			result.array[i].code, result.array[i].name,
			&result.array[i].price, &result.array[i].initialQuantity
			);
		result.array[i].code[5] = '\0'; //Null character terminator needed for string printing
		result.array[i].itemsSold = 0; //No Item have been sold yet, so set each ItemSold to 0
	}
	return result;
}

void printArray(struct ItemArray target){
	int i;
	for(i = 0; i < target.size; i++){
		printf("%s %s %.2f %d\n",
			target.array[i].code, target.array[i].name,
			target.array[i].price, target.array[i].initialQuantity
			);
	}
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
	
	//Test for countLines and readFile
	FILE *gst_file = fopen("../gst.txt", "r");
	FILE *ngst_file = fopen("../gst.txt", "r+");
	struct ItemArray test1 = readFile(gst_file);
	fseek(ngst_file, 0, SEEK_END);
	fprintf(ngst_file,"\nDG003;Example One;2.30;25");
	rewind(ngst_file);
	struct ItemArray test2 = readFile(ngst_file);
	printArray(test1);
	printArray(test2);
	free(test1.array);
	free(test2.array);
	
	return 0;
}