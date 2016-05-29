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

//Functions not listed here: doesItemExist(), whereIsItem(), and printReceipt()
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

	if((gst_file != NULL) && (ngst_file != NULL)){ //If opening files is successfull, proceed as normal
		int i;

		//Read from gst_file and append each item into the array
		for(i = 0; i < GST_Items; i++){
			fscanf(gst_file, "%5c;%[a-zA-Z ];%lf;%d\n",
				gst[i].code, gst[i].name,
				&gst[i].price, &gst[i].initialQuantity
				);

			gst[i].code[5] = '\0'; //Null character terminator needed for string printing
			gst[i].itemsSold = 0; //No Item have been sold yet, so set each ItemSold to 0

		}

		for(i = 0; i < NGST_Items; i++){

			//Read from ngst_file and append each item into array
			fscanf(ngst_file, "%5c;%[a-zA-Z ];%lf;%d\n",
				ngst[i].code, ngst[i].name,
				&ngst[i].price, &ngst[i].initialQuantity
				);

			ngst[i].code[5] = '\0'; //Null character terminator needed for string printing
			ngst[i].itemsSold = 0; //No items have been sold yet, so set each ItemSold to 0

		}

		//Main operation loop
		do{
			printOptions();
			printf("Choice: ");
			scanf(" %c", &choice);
			sleep(1); //Pause for 1 second

			if(strchr("156", choice) != NULL) //Clear screen only if choice is '1', '5', or '6', strchr check as a shortcut
				clearScreen();
			switch(choice){
				case '1':	purchaseItems(gst, ngst);		break;
				case '2':	editItem();						break;
				case '3':	addItem();						break;
				case '4':	deleteItem();					break;
				case '5':	showInventory(gst, ngst);		break;
				case '6':	dailyTransactions(gst, ngst);	break;
				case '7':	puts("Goodbye.");	sleep(1);	break;
				default:
					printf("\nInvalid option entered. Please try again.\n");
					break;
			}

			//Pause for 2 seconds if choice is not '1', '5', '6', or '7'
			if(strchr("1567", choice) == NULL)
				sleep(2);

			clearScreen();
		} while(choice != '7');

		//Close files
		fclose(gst_file);
		fclose(ngst_file);
	}

	//If opening files has an error,
	//immediately print generic error message and exit.
	else{
		puts("Cannot open either gst.txt, ngst.txt, or both.");
		puts("Solutions:");
		puts("1. Add those two files and try again.");
		puts("2. Close any programs using them and try again.");
	}
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

/* This function checks whether the item is in gst, ngst, or neither
Returns 1 if it has gst, 0 if it does not
Returns -1 if neither */
int doesItemExist(struct Item *gst, struct Item *ngst, char *itemCode){

	//All gst Item have 'G' as 2nd character
	if(itemCode[1] == 'G'){
		int i;
		for(i = 0; i < GST_Items; i++)
			if(strcmp(itemCode, gst[i].code) == 0)
				return 1;
	}

	//All ngst Item have 'N' as 2nd character
	else if(itemCode[1] == 'N'){
		int i;
		for(i = 0; i < NGST_Items; i++)
			if(strcmp(itemCode, ngst[i].code) == 0)
				return 0;
	}

	return -1; //Return -1 when G or N is not found as the 2nd character
}

/* This function finds where the item is listed
MUST be used after doesItemExist(). NO EXCEPTIONS.
Returns the item's position in the array if found
If failed, return -1 (SHOULD NEVER HAPPEN)
Used to further refine the search for the item */
int whereIsItem(struct Item *list, int listLength, char *itemCode){
	int i;
	for(i = 0; i < listLength; i++){
		//Return position of list(currently stored in i), if found
		if(strcmp(itemCode, list[i].code) == 0)
			return i;
	}

	return -1; //This should NEVER HAPPEN if implemented correctly
}

//This function is used to print the receipt
void printReceipt(struct Item *list, int listLength){
	int i, totalSold;
	double gstTotal, ngstTotal, gst, total;

	//Set all to 0
	totalSold = 0;
	gstTotal = 0; ngstTotal = 0;
	gst = 0; total = 0;

	//--Execution starts here--
	printf("Receipt:\n\n");
	for(i = 0; i < listLength; i++){

		//Item code can only be 'G' or 'N', and now stored as an int
		int isItemGst = (list[i].code[1] == 'G');

		//Add item to gstTotal or ngstTotal depending on if it has GST
		if(isItemGst){
			gstTotal += (list[i].price * list[i].itemsSold);
			gst += (list[i].price * list[i].itemsSold) * 6 / 100;
		}
		else{
			ngstTotal += (list[i].price * list[i].itemsSold);
		}

		//Add quantity of items sold to totalSold
		totalSold += list[i].itemsSold;

		//Print out item's quantity, code, price, and subtotal to console
		printf("%2dx %-10s %5.2f %6.2f%s\n",
			list[i].itemsSold,	list[i].code,
			list[i].price,
			list[i].price * list[i].itemsSold,
			(isItemGst)? "SR" : "ZR"
		);

		//Print out item's name
		printf("   %-20s\n", list[i].name);
	}

	//Get the total of the purchase(s)
	total = gstTotal + ngstTotal;

	//Print out total, gstTotal, ngstTotal, and gst to console
	printf("\n");
	printf("Total sales with GST: RM %6.2f\n", total + gst);
	printf("Number of items sold: %6d\n\n", totalSold);
	printf("%-20s\t%6s\t%6s\n", "GST Summary", "Amount", "Tax");
	printf("%-20s\t%6.2f\t%6.2f\n", "SR @6%", gstTotal, gst);
	printf("%-20s\t%6.2f\t%6.2f\n", "ZR @0%", ngstTotal, 0.00);
	printf("%-20s\t%6.2f\t%6.2f\n", "Total", total, gst);
}

/* This function allows customers to purchase items
Prints a receipt after the customer is finished */
void purchaseItems(struct Item *gst, struct Item *ngst){
	struct Item *list;	int listSize;
	double subTotal, gstTotal;
	char excess;

	//Set subtotal and gstTotal to 0
	subTotal = 0;	gstTotal = 0;

	//Start from beginning of array
	listSize = 0;

	//Print instructions
	puts("To purchase an item, enter it in this format:");
	puts("(item code, quantity), no brackets");
	puts("Enter (EXIT0, -1) to exit.");
	puts("------------------------------------");

	//Main purchasing loop
	char codeBuffer[6];
	int quantity;

	//Initial allocation
	list = malloc(sizeof(struct Item));

	do{ //do{}while() loop only exits when (EXIT0, -1) is entered

		memset(&codeBuffer[0], 0, sizeof(codeBuffer)); //Fill codeBuffer with zeroes
		quantity = 0;
		printf("Purchase: ");
		scanf(" %5c, %d", codeBuffer, &quantity);

		//Proceed if quantity is positive
		if(quantity > 0){

			//Check if item belongs to gst, ngst or neither
			int itemCategory = doesItemExist(gst, ngst, codeBuffer);

			//If it belongs to neither, print out error message and retry
			if(itemCategory == -1){
				printf("Invalid item code. Please try again.\n\n");
			}

			//If it belongs to either two, proceed as usual
			else{

				//Now we search for the item in either lists
				//Check if it in gst or ngst
				int isItemGst = (itemCategory == 1);

				//Find the item's position in either gst or ngst
				int position = whereIsItem(
					(isItemGst)? gst : ngst,
					(isItemGst)? GST_Items : NGST_Items,
					codeBuffer
					);

				//Create pointer to point to the structure array for easier thinking
				struct Item *selected = (isItemGst)? gst : ngst;

				//Check if there's enough stock left
				//If not, print out error message and retry
				if(selected[position].initialQuantity < selected[position].itemsSold + quantity){
					printf("Not enough stock for purchase.Please try again\n\n");
				}

				//Proceed as normal if there is enough to buy
				else{

					//Expand list's capacity
					struct Item *temp = realloc(list, (listSize + 1) * sizeof(struct Item));

					//DO NOT continue operation if temp is null
					//Means that there's not enough memory in system
					//Prompt user to close other programs before continuing
					if(temp == NULL){
						puts("Error: Not enough memory");
						printf("Please close other programs before trying again.\n\n");
					}
					//Proceed as normal if otherwise
					else{

						//Repoint list to new allocation
						list = temp;

						//Item has been found. Now load list with item found
						list[listSize] = selected[position];

						//Write quantity sold to itemsSold
						list[listSize].itemsSold = quantity;

						//Increase items sold of it in the selected list
						selected[position].itemsSold += quantity;

						//Calculate new subtotal with additional item(s)
						subTotal += (list[listSize].price * quantity) * ((isItemGst)? 1.06 : 1.00);
						gstTotal += (list[listSize].price * quantity) * ((isItemGst)? 0.06 : 0);

						//Print the item(s) bought and the new subtotal to console
						printf("Item(s) bought: %dx %s\n", list[listSize].itemsSold, list[listSize].name);
						printf("Subtotal (gst): RM %.2f (RM %.2f)\n\n", subTotal, gstTotal);

						//Increment listSize by 1 to match with total count
						listSize += 1;
					}
				}
			}
		}

		//All quantities are negative now

		else if(strcmp(codeBuffer, "EXIT0") != 0){ //If code is not "EXIT0", then prompt this error
			printf("Invalid quantity. Please try again.\n");
		}
		else{ //Prompt exit message if passed both
			printf("Purchase(s) completed. Exiting...\n\n");
		}

		fflush(stdin);
	} while((strcmp(codeBuffer, "EXIT0") != 0) || (quantity > 0));

	//Print the receipt if *list is not NULL and there are elements in list
	if(list != NULL && listSize != 0){
		puts("Printing receipt...");

		//Print the receipt
		printReceipt(list, listSize);

		//Transaction is done, we don't need list anymore
		free(list);
	}

	//Do not print receipt otherwise
	else{
		printf("No items received. No receipt printed.\n");
	}

	//Prompt user to enter a character to continue
	printf("\n\nEnter any character to continue...  ");
	scanf(" %c", &excess);
}

//This function edits an item
void editItem(){
	puts("This option allows user to edit items");
}

//This function adds an item
void addItem(){
	puts("This option allows user to add items");
}

//This function deletes an item
void deleteItem(){
	puts("This option allows user to delete items");
}

//This function shows the current inventory
void showInventory(struct Item *gst, struct Item *ngst){
	char excess;
	int i;

	//Print GST items
	printf("GST items:\n\n");
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

	//Print Non-GST items
	printf("Non-GST items:\n\n");
	printf("%s\t%-20s\t%-6s\t%-6s\n",
		"Code", "Name", "Price", "Quantity");
	puts("-------------------------------------------------");
	for(i = 0; i < NGST_Items; i++){
		int remaining = ngst[i].initialQuantity - ngst[i].itemsSold;
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
		totalItemSold += gst[i].itemsSold; //Increment total items sold
		gstTotal += gst[i].price * gst[i].itemsSold; //Calculate total sales made with GST items
	}

	gstCollected = gstTotal * 0.06; //Calculate GST collected

	for(i = 0; i < NGST_Items; i++){ //Get total from ngst
		totalItemSold += ngst[i].itemsSold; //Increment total items sold
		ngstTotal += ngst[i].price * ngst[i].itemsSold; //Calculate total sales made with Non-GST items
	}
	
	//Print out results to console
	printf("Total transaction\t: %d\n", totalItemSold);
	printf("Sales with GST\t\t: RM %.2f\n", gstTotal);
	printf("Sales without GST\t: RM %.2f\n", ngstTotal);
	printf("GST collected\t\t: RM %.2f\n", gstCollected);

	//Prompt user to enter a character to continue
	printf("\n\nEnter any character to continue...  ");
	scanf(" %c", &excess);
}
