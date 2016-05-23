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

#define GST_Items 8
#define NGST_Items 6

struct Item {
	char code[6];
	char name[20];
	double price;
	int initialQuantity;
	int itemsSold;
};

//Functions not listed here: doesItemExist(), and whereIsItem()
//Functions not listed here are only to be used by other functions, not the main loop
void clearScreen(){ system("clear");} //Clears screen(Only works on Linux/Unix)
void printOptions();
void purchaseItems(struct Item *gst, struct Item *ngst);
void editItem();
void addItem();
void deleteItem();
void showInventory(struct Item *gst, struct Item *ngst);
void dailyTransactions(struct Item *gst, struct Item *ngst);

int main(){
	//File pointers here
	FILE *gst_file, *ngst_file;
	//Fixed arrays to store the items
	//TODO: Change to dynamic arrays (Hard in C)
	struct Item gst[GST_Items], ngst[NGST_Items];
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
		for(i = 0; i < GST_Items; i++){
			//Enter each item into array
			fscanf(gst_file, "%5c;%[a-zA-Z ];%lf;%d\n", 
				gst[i].code, gst[i].name, 
				&gst[i].price, &gst[i].initialQuantity
				);
			//Null character terminator needed for string printing
			gst[i].code[5] = '\0'; 
			//No Item have been sold yet, so set each ItemSold to 0
			gst[i].itemsSold = 0;
		}
		//Read from ngst_file
		for(i = 0; i < NGST_Items; i++){
			//Enter each item into array
			fscanf(ngst_file, "%5c;%[a-zA-Z ];%lf;%d\n", 
				ngst[i].code, ngst[i].name, 
				&ngst[i].price, &ngst[i].initialQuantity
				);
			//Null character terminator needed for string printing
			ngst[i].code[5] = '\0';
			//No Item have been sold yet, so set each ItemSold to 0
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
				case '2':	editItem();					break;
				case '3':	addItem();						break;
				case '4':	deleteItem();					break;
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

//This function prints the list of options available
void printOptions(){
	puts("------------------------------------");
	puts("Grocery Retail"); 
	puts("------------------------------------");
	puts("1. Purchase Item");
	puts("2. Edit Item");
	puts("3. Add Item");
	puts("4. Delete Item");
	puts("5. Show inventory");
	puts("6. Show daily transaction");
	puts("7. Exit");
	puts("------------------------------------");
	puts("");
}

//This function checks whether the item is in gst, ngst, or neither
//Returns 1 if it has gst, 0 if it does not
//Returns -1 if neither
int doesItemExist(struct Item *gst, struct Item *ngst, char *itemCode){
	//All gst Item have 'G' as 2nd character
	//So, check if itemCode has it
	if(itemCode[1] == 'G'){
		int i;
		for(i = 0; i < GST_Items; i++)
			if(strcmp(itemCode, gst[i].code) == 0)
				return 1;
	}
	//All ngst Item have 'N' as 2nd character
	//So, check if itemCode has it instead
	else if(itemCode[1] == 'N'){
		int i;
		for(i = 0; i < NGST_Items; i++)
			if(strcmp(itemCode, ngst[i].code) == 0)
				return 0;
	}
	//If it doesn't have 'G' or 'N', or exhausts both attempts
	return -1;
}

//This function finds where the item is listed
//MUST be used after doesItemExist(). NO EXCEPTIONS.
//Returns the item's position in the array if found
//If failed, return -1 (SHOULD NEVER HAPPEN)
//Used to further refine the search for the item
int whereIsItem(struct Item *list, int listLength, char *itemCode){
	int i;
	for(i = 0; i < listLength; i++){
		//Return position of list(currently stored in i), if found
		if(strcmp(itemCode, list[i].code) == 0)
			return i;
	}
	//This should NEVER HAPPEN if implemented correctly
	return -1;
}

//This function is used to print the receipt
void printReceipt(struct Item *list, int listLength){
	int i, totalSold;
	double gstTotal, ngstTotal, gst, total;
	//Set all to 0
	totalSold = 0;
	gstTotal = 0; ngstTotal = 0; 
	gst = 0; total = 0;
	for(i = 0; i < listLength; i++){
		//Item code can only be 'G' or 'N'
		int isItemGst = (list[i].code[1] == 'G');
		if(isItemGst){
			gstTotal += (list[i].price * list[i].itemsSold); 
			gst += (list[i].price * list[i].itemsSold) * 6 / 100;
		}
		else{
			ngstTotal += 
				(list[i].price * list[i].itemsSold);
		}
		totalSold += list[i].itemsSold;
		printf("%2dx %-10s %5.2f %6.2f%s\n", 
			list[i].itemsSold,	list[i].code,
			list[i].price,	list[i].price * list[i].itemsSold,
			(isItemGst)? "SR" : "ZR"
		);
		printf("   %-20s\n", list[i].name);
	}
	total = gstTotal + ngstTotal;
	printf("\n");
	printf("Total sales including GST: RM %6.2f\n", total);
	printf("Number of items sold: %d\n", totalSold);
	printf("%-20s\t%6s\t%6s\n", "GST Summary", "Amount", "Tax");
	printf("%-20s\t%6.2f\t%6.2f\n", "SR @6%", gstTotal, gst);
	printf("%-20s\t%6.2f\t%6.2f\n", "ZR @0%", ngstTotal, 0.00);
	printf("%-20s\t%6.2f\t%6.2f\n", "Total", total, gst);
}

void purchaseItems(struct Item *gst, struct Item *ngst){
	struct Item *list;
	int listSize;
	double subTotal;
	subTotal = 0;
	//Start from beginning of array
	listSize = 0;
	//Print instructions
	puts("To purchase an item, enter it in this format:");
	puts("(item code, quantity), no brackets");
	printf("\nEnter -1 in quantity to exit.\n");
	puts("------------------------------------");
	//Main purchasing loop
	int quantity;
	do{
		char codeBuffer[6];
		printf("Purchase: ");
		scanf(" %5c, %d", codeBuffer, &quantity);
		codeBuffer[5] = '\0'; //assign last element as empty
		if(quantity > 0){
			int itemCategory = doesItemExist(gst, ngst, codeBuffer);
			if(itemCategory == -1){
				printf("Invalid item. Please try again.\n");
			}
			else{
				//Now we search for the item
				int isItemGst = (itemCategory == 1);
				int position = whereIsItem(
					(isItemGst)? gst : ngst, 
					(isItemGst)? GST_Items : NGST_Items,
					codeBuffer
					);
				struct Item *selected = (isItemGst)? gst : ngst;
				if(selected[position].initialQuantity <= selected[position].itemsSold + quantity){
					printf("Not enough stock for purchase.Please try again\n");
				}
				else{
					//Main problem found: Line below
					struct Item *temp = realloc(list, (listSize + 1) * sizeof(struct Item));
					//DO NOT continue operation if temp is null
					//Means that there's not enough memory in system
					//Prompt user to close other programs before continuing
					if(temp == NULL){
						puts("Error: Not enough memory");
						puts("Please close other programs before trying again.");
					}
					//Proceed as normal if otherwise
					else{
						//Dump old list
						free(list);
						//Repoint list to new allocation
						list = temp;
						//Item has been found. Now load list with item found
						list[listSize] = selected[position];
						//Perform necessary assignments and math
						list[listSize].itemsSold = quantity;
						selected[position].itemsSold += quantity;
						subTotal += 
							(list[listSize].price * quantity) * 
							((isItemGst)? 1.06 : 1.00);
						printf("This item is %s\n", list[listSize - 1].name);
						printf("Subtotal: RM %.2f", subTotal);
						//Finished with item
						//Increment listSize by 1 to match with total count
						listSize += 1;
					}
				}
			}
		}
	}while(quantity > 0);
	//Print th receipt if *list is not NULL and there are elements in list 
	if(list != NULL && listSize != 0){
		//Print the receipt
		printReceipt(list, listSize);
		//Transaction is done, we don't need list anymore
		free(list);
	}
	else{
		printf("No items received. No receipt printed.\n");
	}
}

//This function edits an item
void editItem(){
	puts("This option allows user to edit Item");
}

//This function adds an item 
void addItem(){
	puts("This option allows user to add Item");
}

//This function deletes an item
void deleteItem(){
	puts("This option allows user to delete Item");
}

//This function shows the current inventory
void showInventory(struct Item *gst, struct Item *ngst){
	char excess;
	int i;
	//Print Taxable Item
	printf("Taxable Item:\n\n");
	printf("%s\t%-20s\t%-6s\t%-6s\n", 
		"Code", "Name", "Price", "Quantity");
	puts("-------------------------------------------------");
	for(i = 0; i < GST_Items; i++){
		int remaining = gst[i].initialQuantity - gst[i].itemsSold;
		printf("%s\t%-20s\t%5.2f\t%d\n", 
			gst[i].code, gst[i].name, gst[i].price, remaining
			);
	}
	printf("\n\n");
	//Print Non-taxable Item
	printf("Non-taxable Item:\n\n");
	printf("%s\t%-20s\t%-6s\t%-6s\n", 
		"Code", "Name", "Price", "Quantity");
	puts("-------------------------------------------------");
	for(i = 0; i < NGST_Items; i++){
		int remaining = gst[i].initialQuantity - gst[i].itemsSold;
		printf("%s\t%-20s\t%5.2f\t%d\n", 
			ngst[i].code, ngst[i].name, ngst[i].price, remaining
			);
	}
	printf("\nEnter any character to continue...  ");
	scanf(" %c", &excess);
}

//This function shows the transactions made today
void dailyTransactions(struct Item *gst, struct Item *ngst){
	int i; char excess;
	int totalItemSold;
	double gstTotal, ngstTotal, gstCollected;
	//Initalise all to 0
	totalItemSold = 0;
	gstTotal = 0; ngstTotal = 0;
	gstCollected = 0;
	//Get total from gst
	for(i = 0; i < GST_Items; i++){
		totalItemSold += gst[i].itemsSold;
		gstTotal += gst[i].price * gst[i].itemsSold;
	}
	//Get total from ngst
	for(i = 0; i < NGST_Items; i++){
		totalItemSold += ngst[i].itemsSold;
		ngstTotal += ngst[i].price * ngst[i].itemsSold;
	}
	printf("Total transaction\t: %d\n", totalItemSold);
	printf("Sales with GST\t\t: RM %.2f\n", gstTotal);
	printf("Sales without GST\t: RM %.2f\n", ngstTotal);
	printf("GST collected\t\t: RM %.2f\n", gstCollected);
	printf("\n\nEnter any character to continue...  ");
	scanf(" %c", &excess);
}
