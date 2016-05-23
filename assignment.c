/*-------------------------------------------------------------------*/
/* ITS60304– Assignment #1 */
/* C Programming */
/* Student Name: <Chong Jia Sheng> <Kwan Juen Wen> */
/* Student ID: <0326800> <0322448> */
/*-------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define GST_ITEMS 8
#define NGST_ITEMS 6

struct Items {
	char code[6];
	char name[20];
	double price;
	int initialQuantity;
	int itemsSold;
};

void clearScreen(){ system("clear");} //Clears screen(Only works on Linux/Unix)
void printOptions();
void purchaseItems(struct Items *gst, struct Items *ngst);
void editItems();
void addItems();
void deleteItems();
void showInventory(struct Items *gst, struct Items *ngst);
void dailyTransactions(struct Items *gst, struct Items *ngst);

int main(){
	//File pointers here
	FILE *gst_file, *ngst_file;
	//Fixed arrays to store the items
	//TODO: Change to dynamic arrays (Hard in C)
	struct Items gst[GST_ITEMS], ngst[NGST_ITEMS];
	//A variable to store the choices made
	char choice;
	//--Execution starts from here--
	//Clear screen for program
	clearScreen();
	//Open both files
	gst_file = fopen("gst.txt", "r");
	ngst_file = fopen("ngst.txt", "r");
	//If opening files is successfull, proceed as normal
	if((gst_file != NULL) && (ngst_file != NULL)){
		int i;
		//Read from gst_file
		for(i = 0; i < GST_ITEMS; i++){
			//Enter each item into array
			fscanf(gst_file, "%5c;%[a-zA-Z ];%lf;%d\n", 
				gst[i].code, gst[i].name, 
				&gst[i].price, &gst[i].initialQuantity
				);
			//Null character terminator needed for string printing
			gst[i].code[5] = '\0'; 
			//No items have been sold yet, so set each itemsSold to 0
			gst[i].itemsSold = 0;
		}
		//Read from ngst_file
		for(i = 0; i < NGST_ITEMS; i++){
			//Enter each item into array
			fscanf(ngst_file, "%5c;%[a-zA-Z ];%lf;%d\n", 
				ngst[i].code, ngst[i].name, 
				&ngst[i].price, &ngst[i].initialQuantity
				);
			//Null character terminator needed for string printing
			ngst[i].code[5] = '\0';
			//No items have been sold yet, so set each itemsSold to 0
			ngst[i].itemsSold = 0;
		}
		//Main operation loop
		do{
			printOptions();
			printf("Choice: ");
			scanf(" %c", &choice);
			//sleep(1); //Pause for 1 second
			//Do not clear screen if choice is '2', '3', '4', or '7'
			//Used strchr check as a shortcut
			if(strchr("2347", choice) == NULL)
				clearScreen(); 
			switch(choice){
				case '1':	purchaseItems(gst, ngst);		break;
				case '2':	editItems();					break;
				case '3':	addItems();						break;
				case '4':	deleteItems();					break;
				case '5':	showInventory(gst, ngst);		break;
				case '6':	dailyTransactions(gst, ngst);	break;
				case '7':	puts("Goodbye.");	sleep(1);	break;
				default: 
					puts("Invalid option entered. Please try again.");
					break;
			}
			//sleep(2);
			clearScreen();
		}while(choice != '7');
	}
	//If opening files has an error,
	//immediately print generic error message and exit.
	else{
		puts("Cannot open either gst.txt, or ngst.txt.");
		puts("Please close all programs using either two files try again later.");
	}
	//Close files after reading
	fclose(gst_file);
	fclose(ngst_file);
	return 0;
}

void printOptions(){
	puts("------------------------------------");
	puts("Grocery Retail"); 
	puts("------------------------------------");
	puts("1. Purchase items");
	puts("2. Edit items");
	puts("3. Add items");
	puts("4. Delete items");
	puts("5. Show inventory");
	puts("6. Show daily transaction");
	puts("7. Exit");
	puts("------------------------------------");
	puts("");
}

//This function checks whether the item is in gst, ngst, or neither
//Returns 1 if it has gst, 0 if it does not
//Returns -1 if neither
int doesItemExist(struct Items *gst, struct Items *ngst, char *itemCode){
	//All gst items have 'G' as 2nd character
	//So, check if itemCode has it
	if(itemCode[1] == 'G'){
		int i;
		for(i = 0; i < GST_ITEMS; i++)
			if(strcmp(itemCode, gst[i].code) == 0)
				return 1;
	}
	//All ngst items have 'N' as 2nd character
	//So, check if itemCode has it instead
	else if(itemCode[1] == 'N'){
		int i;
		for(i = 0; i < NGST_ITEMS; i++)
			if(strcmp(itemCode, ngst[i].code) == 0)
				return 0;
	}
	//If it doesn't have 'G' or 'N', or exhausts both attempts
	return -1;
}

void purchaseItems(struct Items *gst, struct Items *ngst){
	char itemCode[6];
	int quantity;
	printf("Enter item code: ");
	scanf(" %s", itemCode);
	printf("Enter quantity:  ");
	scanf(" %d", &quantity);
	//Only use next line in debug
	printf("You've entered %s\n", itemCode);
	int itemCategory = doesItemExist(gst, ngst, itemCode);
	if(itemCategory == -1){
		printf("Invalid item. Please try again.\n");
	}
	else{
		printf("This item is %d\n", itemCategory);
	}
	//Next line is for debugging purposes
	//printf("You entered: %s, %d\n", itemCode, quantity);
	
}

void editItems(){
	puts("This option allows user to edit items");
}

void addItems(){
	puts("This option allows user to add items");
}

void deleteItems(){
	puts("This option allows user to delete items");
}

void showInventory(struct Items *gst, struct Items *ngst){
	char excess;
	int i;
	//Print Taxable items
	printf("Taxable items:\n\n");
	printf("%s\t%-20s\t%-6s\t%-6s\n", 
		"Code", "Name", "Price", "Quantity");
	puts("-------------------------------------------------");
	for(i = 0; i < GST_ITEMS; i++){
		int remaining = gst[i].initialQuantity - gst[i].itemsSold;
		printf("%s\t%-20s\t%5.2f\t%d\n", 
			gst[i].code, gst[i].name, gst[i].price, remaining
			);
	}
	printf("\n\n");
	//Print Non-taxable items
	printf("Non-taxable items:\n\n");
	printf("%s\t%-20s\t%-6s\t%-6s\n", 
		"Code", "Name", "Price", "Quantity");
	puts("-------------------------------------------------");
	for(i = 0; i < NGST_ITEMS; i++){
		int remaining = gst[i].initialQuantity - gst[i].itemsSold;
		printf("%s\t%-20s\t%5.2f\t%d\n", 
			ngst[i].code, ngst[i].name, ngst[i].price, remaining
			);
	}
	printf("\nEnter any character to continue...  ");
	scanf(" %c", &excess);
}

void dailyTransactions(struct Items *gst, struct Items *ngst){
	int i; char excess;
	int totalItemsSold;
	double gstTotal, ngstTotal, gstCollected;
	//Initalise all to 0
	totalItemsSold = 0;
	gstTotal = 0; ngstTotal = 0;
	gstCollected = 0;
	//Get total from gst
	for(i = 0; i < GST_ITEMS; i++){
		totalItemsSold += gst[i].itemsSold;
		gstTotal += gst[i].price * gst[i].itemsSold;
	}
	//Get total from ngst
	for(i = 0; i < NGST_ITEMS; i++){
		totalItemsSold += ngst[i].itemsSold;
		ngstTotal += ngst[i].price * ngst[i].itemsSold;
	}
	printf("Total transaction\t: %d\n", totalItemsSold);
	printf("Sales with GST\t\t: RM %.2f\n", gstTotal);
	printf("Sales without GST\t: RM %.2f\n", ngstTotal);
	printf("GST collected\t\t: RM %.2f\n", gstCollected);
	printf("\n\nEnter any character to continue...  ");
	scanf(" %c", &excess);
}
