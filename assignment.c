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

struct Item {
	char code[6];
	char name[31];
	double price;
	int initialQuantity;
	int itemsSold;
};

struct ItemArray {
	int size; //Store the size of array here
	struct Item *array;
};

//Functions not listed here: doesItemExist(), whereIsItem(), and printReceipt()
//Functions not listed here are only to be used by other functions, not the main loop
void clearScreen(){ system("clear");} //Clears screen(Only works on Linux/Unix)
void dumpRemainingInput(){ while(getchar() != '\n'); }
void printOptions();
void purchaseItems(struct ItemArray gst, struct ItemArray ngst);
void editItem(struct ItemArray gst, struct ItemArray ngst);
void addItem(struct ItemArray *gst, struct ItemArray *ngst);
void deleteItem(struct ItemArray gst, struct ItemArray ngst);
void showInventory(struct ItemArray gst, struct ItemArray ngst);
void dailyTransactions(struct ItemArray gst, struct ItemArray ngst);
int countLines(FILE *file);
struct ItemArray readFile(FILE *file);
void quickSort(struct Item *input, int size);

int main(){
	//File pointers here
	FILE *gst_file, *ngst_file;

	//Dynamic arrays to store the items from files, and items sold
	struct ItemArray gst, ngst, sold;

	//A variable to store the choices made
	char choice;

	//--Execution starts from here--
	//Clear screen for program
	clearScreen();

	//Open both files
	gst_file = fopen("gst.txt", "r+");
	ngst_file = fopen("ngst.txt", "r+");

	if((gst_file != NULL) && (ngst_file != NULL)){ //If opening files is successful, proceed as normal
		//Read both files
		gst = readFile(gst_file);
		ngst = readFile(ngst_file);
		
		//Close files. We don't need them for now.
		fclose(gst_file);
		fclose(ngst_file);

		//Main operation loop
		do{
			printOptions();
			printf("Choice: ");
			scanf(" %c", &choice);
			//Remove extra characters
			dumpRemainingInput();
			sleep(1); //Pause for 1 second
			
			//Clear the screen
			clearScreen();
			
			switch(choice){
				case '1':	purchaseItems(gst, ngst);		break;
				case '2':	editItem(gst, ngst);			break;
				case '3':	addItem(&gst, &ngst);			break;
				case '4':	deleteItem(gst, ngst);			break;
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

/* This function checks whether the item is in gst, ngst, list, or none
Returns 2 if it is already in list, 1 if it has gst, 0 if it does not
Returns -1 if none */
int doesItemExist(struct ItemArray gst, struct ItemArray ngst, struct ItemArray list, char *itemCode){
	//Check if it exists in the list already
	if(list.array != NULL){
		int i;
		for(i = 0; i < list.size; i++){
			if(strcmp(itemCode, list.array[i].code) == 0)
				return 2;
		}
	}
	
	//If not, proceed to gst and ngst arrays
	//All gst Item have 'G' as 2nd character
	if(itemCode[1] == 'G'){
		int j;
		for(j = 0; j < gst.size; j++)
			if(strcmp(itemCode, gst.array[j].code) == 0)
				return 1;
	}

	//All ngst Item have 'N' as 2nd character
	else if(itemCode[1] == 'N'){
		int k;
		for(k = 0; k < ngst.size; k++)
			if(strcmp(itemCode, ngst.array[k].code) == 0)
				return 0;
	}

	return -1; //Return -1 when G or N is not found as the 2nd character
}

/* This function finds where the item is listed
MUST be used after doesItemExist(). NO EXCEPTIONS.
Returns the item's position in the array if found
If failed, return -1 (SHOULD NEVER HAPPEN)
Used to further refine the search for the item */
int whereIsItem(struct ItemArray list, char *itemCode){
	int i;
	for(i = 0; i < list.size; i++){
		//Return position of list(currently stored in i), if found
		if(strcmp(itemCode, list.array[i].code) == 0)
			return i;
	}

	return -1; //This should NEVER HAPPEN if implemented correctly
}

//This function is used to print the receipt
void printReceiptOnScreen(struct ItemArray list){
	int i, totalSold;
	double gstTotal, ngstTotal, gst, total;

	//Set all to 0
	totalSold = 0;
	gstTotal = 0; ngstTotal = 0;
	gst = 0; total = 0;

	//--Execution starts here--
	printf("Receipt:\n\n");
	for(i = 0; i < list.size; i++){

		//Item code can only be 'G' or 'N', and now stored as an int
		int isItemGst = (list.array[i].code[1] == 'G');

		//Add item to gstTotal or ngstTotal depending on if it has GST
		if(isItemGst){
			gstTotal += (list.array[i].price * list.array[i].itemsSold);
			gst += (list.array[i].price * list.array[i].itemsSold) * 6 / 100;
		}
		else{
			ngstTotal += (list.array[i].price * list.array[i].itemsSold);
		}

		//Add quantity of items sold to totalSold
		totalSold += list.array[i].itemsSold;

		//Print out item's quantity, code, price, and subtotal to console
		printf("%2dx %-10s %5.2f %6.2f%s\n",
			list.array[i].itemsSold,	list.array[i].code,
			list.array[i].price,
			list.array[i].price * list.array[i].itemsSold,
			(isItemGst)? "SR" : "ZR"
		);

		//Print out item's name
		printf("   %-20s\n", list.array[i].name);
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

//This function prints/updates the file purchase.txt
void printPurchasedFile(struct ItemArray gst, struct ItemArray ngst){
	
}

/* This function allows customers to purchase items
Prints a receipt after the customer is finished */
void purchaseItems(struct ItemArray gst, struct ItemArray ngst){
	struct ItemArray list;
	double subTotal, gstTotal;
	char excess;

	//Set subtotal and gstTotal to 0
	subTotal = 0;	gstTotal = 0;

	//Start from beginning of array
	list.size = 0;

	//Print instructions
	puts("Purchase Option Selected");
	puts("To exit:");
	puts("Enter \"-1\" in \"Item Code\" to exit ");
	puts("------------------------------------");

	//Main purchasing loop
	char code[6];

	//Initial allocation
	list.array = malloc(sizeof(struct Item));

	do{ //do{}while() loop only exits when (EXIT0, -1) is entered
		char buffer[300];
		memset(&code[0], 0, sizeof(code)); //Fill codeBuffer with zeroes
		
		//Item code enter first
		printf("Item Code: ");
		scanf(" %s", buffer);
		//Remove remaining input before continuing
		dumpRemainingInput();
		strncpy(code, buffer, 5);
		
		if(strcmp(code, "-1") != 0){ //If code is not "-1", then continue
			if(strlen(buffer) == 5){ //Check if input is 5 characters long
				int quantity;
				//Quantity enter
				printf("Quantity:  ");
				scanf(" %d", &quantity);
				//Proceed if quantity is positive
				if(quantity > 0){
					//Check if item belongs to gst, ngst or neither
					int itemCategory = doesItemExist(gst, ngst, list, code);

					//If it belongs to neither, print out error message and retry
					if(itemCategory == -1){
						printf("Invalid item code. Please try again.\n\n");
					}
					
					//If it already is in list, check in list and update there
					else if(itemCategory == 2){
						//Find the item's position in list
						int position = whereIsItem(list, code);
						int isItemGst = (list.array[position].code[1] == 'G');
						struct ItemArray selected = (isItemGst)? gst : ngst;
						
						//Increment counters to new quantity sold
						list.array[position].itemsSold += quantity;
						//Increase items sold of it in the selected list
						selected.array[whereIsItem(selected, code)].itemsSold += quantity;
						
						//Calculate new subtotal with additional item(s)
						subTotal += (list.array[position].price * quantity) * ((isItemGst)? 1.06 : 1.00);
						gstTotal += (list.array[position].price * quantity) * ((isItemGst)? 0.06 : 0);
						
						//Print the item(s) bought and the new subtotal to console
						printf("Item(s) bought again: %dx %s\n", quantity, list.array[position].name);
						printf("Subtotal (gst): RM %.2f (RM %.2f)\n\n", subTotal, gstTotal);
					}
					
					//If not, check in gst or ngst for new item
					else{
						//Now we search for the item in either lists
						//Check if it in gst or ngst
						int isItemGst = (itemCategory == 1);

						//Find the item's position in either gst or ngst
						int position = whereIsItem(
							(isItemGst)? gst : ngst,
							code
							);

						//Create pointer to point to the structure array for easier thinking
						struct ItemArray selected = (isItemGst)? gst : ngst;

						//Check if there's enough stock left
						//If not, print out error message and retry
						if(selected.array[position].initialQuantity < selected.array[position].itemsSold + quantity){
							printf("Not enough stock for purchase.\n");
							printf("Remaining stock: %d", 
								selected.array[position].initialQuantity - selected.array[position].itemsSold);
						}

						//Proceed as normal if there is enough to buy
						else{

							//Expand list's capacity
							struct Item *temp = realloc(list.array, (list.size + 1) * sizeof(struct Item));

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
								list.array = temp;

								//Item has been found. Now load list with item found
								list.array[list.size] = selected.array[position];

								//Write quantity sold to itemsSold
								list.array[list.size].itemsSold = quantity;

								//Increase items sold of it in the selected list
								selected.array[position].itemsSold += quantity;
								
								//Calculate new subtotal with additional item(s)
								subTotal += (list.array[list.size].price * quantity) * ((isItemGst)? 1.06 : 1.00);
								gstTotal += (list.array[list.size].price * quantity) * ((isItemGst)? 0.06 : 0);

								//Print the item(s) bought and the new subtotal to console
								printf("Item(s) bought: %dx %s\n", list.array[list.size].itemsSold, list.array[list.size].name);
								printf("Subtotal (gst): RM %.2f (RM %.2f)\n\n", subTotal, gstTotal);

								//Increment listSize by 1 to match with total count
								list.size += 1;
							}
						}
					}
				}
				//All quantities are negative now
				else{
					printf("Invalid quantity. Please try again.\n\n");
				}	
			}
			else{ //Print error otherwise
				printf("Invalid code entered. Try again.\n\n");
			}
		}
		else{
			printf("Purchase(s) completed. Exiting...\n\n");
		}
		fflush(stdin);
	} while(strcmp(code, "-1") != 0);

	//Clear input stream
	//dumpRemainingInput();
	
	//Print the receipt if *list is not NULL and there are elements in list
	if(list.array != NULL && list.size != 0){
		puts("Printing receipt...");

		//Print the receipt
		printReceiptOnScreen(list);
		//printReceiptToFile(list);

		//Transaction is done, we don't need list anymore
		free(list.array);
	}

	//Do not print receipt otherwise
	else{
		printf("No items received. No receipt printed.\n");
	}

	//Prompt user to enter to continue
	printf("\n\nEnter to continue...  ");
	dumpRemainingInput();
}

//This function edits an item
void editItem(struct ItemArray gst, struct ItemArray ngst){
	puts("This option allows user to edit items");
}

//This function checks if the code entered is correct
//Returns 1 if correct, an error message and 0 if not
int isCodeCorrect(char *input){
	int result;
	int i;
	//Check 1st character if it is a digit. Quit if yes
	if(strchr("1234567890", input[0]) != NULL){
		printf("1st character must be a letter. Try again.\n");
		return 0;
	}
	//Checks if last 3 characters are digits
	for(i = 2; i <= 4; i++){
		if(strchr("1234567890", input[i]) == NULL){
			printf("Last 3 characters must be numbers. Try again.\n");
			return 0;
		}
	}
	//Checks if has 'G' or 'N' @ 2nd character, depending on isInputGst
	if(strchr("GN", input[1]) == NULL ){
		printf("Second character of code is not \'G\' or \'N\'. Try again.\n");
		return 0;
	}
	//Code now follows the standard. Return 1 now.
	return 1;
}

//To be used for qsort
int compareItems(const void* a, const void* b){
	struct Item *item1 = (struct Item *)a;
	struct Item *item2 = (struct Item *)b;
	return strcmp(item1->code, item2->code);
}

//This function adds a new item to the selected file
//It is not possible to append data without rewriting old data
//As such, it simply rewrites the file with the new entry
void appendItemToFile(struct ItemArray source, char *fileName){
	FILE *target = fopen(fileName, "w");
	int i;
	if(target != NULL){
		for(i = 0; i < source.size; i++){
			fprintf(target, "%s;%s;%.2f;%d",
				source.array[i].code, source.array[i].name,
				source.array[i].price, source.array[i].initialQuantity
				);
			if(i != source.size - 1){
				fprintf(target, "\n");
			}
		}
	}
	else{
		printf("Cannot open file %s. Please try again.\n\n", fileName);
	}
	fclose(target);
}

//This function adds an item
void addItem(struct ItemArray *gst, struct ItemArray *ngst){
	char choice;
	int isInputGst = 0;
	struct Item input;
	puts("This option allows user to add items");
	//Request item's code. Keep requesting if wrong.
	int codeInputSuccessful = 0;
	do{		
		char buffer[30];
		printf("Enter the item's code in uppercase: ");
		scanf(" %[^\n]", buffer);
		//Remove remaining input before continuing
		dumpRemainingInput();
		if(strlen(buffer) == 5){
			//Check if the code's format is correct
			if(isCodeCorrect(buffer)){
				//This ItemArray does nothing but fill a parameter
				struct ItemArray nothing = {0, NULL};
				//Check if the code already exists
				int itemExists = doesItemExist(*gst, *ngst, nothing, buffer);
				if(itemExists == -1 ){
					//Perform copy operation
					strncpy(input.code, buffer, 6);
					input.code[5] = '\0';
					codeInputSuccessful = 1;
				}
				else{
					printf("This code already exists. Try again.%d\n\n", itemExists);
				}
			}
			else{
				printf("\n");
			}
		}
		else{ //Print error otherwise
			printf("Code entered is not 5 characters long. Try again.\n\n");
		}
	}while(codeInputSuccessful == 0);
	//Determine if item has GST based on code
	isInputGst = (input.code[1] == 'G');
	//Request item's name. Keep requesting if wrong.
	int nameInputSuccessful = 0;
	do{		
		char buffer[31]; 
		printf("Enter the item's name: ");
		scanf(" %[^\n]", buffer);
		//Remove remaining input before continuing
		dumpRemainingInput();
		int slength = strlen(buffer);
		if((slength <= 30) && (slength > 0)){
			strncpy(input.name, buffer, slength);
			input.name[slength] = '\0';
			nameInputSuccessful = 1;
		}
		else{ //Print error otherwise
			printf("Name entered is not 1 to 30 characters long. Try again.\n\n");
		}
	}while(nameInputSuccessful == 0);
	//Request item's price. Keep requesting if wrong.
	int priceInputSuccessful = 0;
	do{
		double priceBuffer;
		printf("Enter the item's price: ");
		scanf(" %lf", &priceBuffer);
		//Remove remaining input before continuing
		dumpRemainingInput();
		if(priceBuffer > 0.00){
			input.price = priceBuffer;
			priceInputSuccessful = 1;
		}
		else
			printf("Item must have a price of more than 0.00. Try again.\n\n");	
	}while(priceInputSuccessful == 0);
	//Request item's quantity. Keep requesting if wrong.
	int quantityInputSuccessful = 0;
	do{
		int quantityBuffer;
		printf("Enter the item's initial quantity: ");
		scanf(" %d", &quantityBuffer);
		//Remove remaining input before continuing
		dumpRemainingInput();
		if(quantityBuffer > 0){
			input.initialQuantity = quantityBuffer;
			quantityInputSuccessful = 1;
		}
		else
			printf("Item must have a quantity of more than 0. Try again.\n\n");	
	}while(quantityInputSuccessful == 0);
	//Items sold of new item is 0 by default.
	input.itemsSold = 0;
	//Allocate temporary array to create new allocation
	struct ItemArray temp;
	temp.size = (isInputGst? gst->size : ngst->size) + 1;
	temp.array = realloc(isInputGst? gst->array : ngst->array, 
						temp.size * sizeof(struct Item));
	//DO NOT continue operation if temp is null
	//Means that there's not enough memory in system
	//Prompt user to close other programs before continuing
	if(temp.array == NULL){
		puts("Error: Not enough memory");
		printf("Please close other programs before trying again.\n\n");
	}
	else{
		//Use old array's size since it now can help point to the last element
		temp.array[temp.size - 1] = input;
		//Sort the array using qsort
		qsort(temp.array, temp.size, sizeof(struct Item), compareItems);
		//Append item to gst.txt or ngst.txt, depending on if the item has GST
		appendItemToFile(temp, isInputGst? "gst.txt" : "ngst.txt");
		//Branch into two options, depending on if the item has GST
		if(isInputGst){			
			//repoint array pointer to temp
			gst->array = temp.array;
			//Increment gst's size by 1 
			gst->size += 1;
		}
		else{
			//repoint array pointer to temp
			ngst->array = temp.array;
			//Increment ngst's size by 1 
			ngst->size += 1;
		}
		//Entry is successfull. Print out entered item.
		printf("The following item was successfully entered:\n");
		printf("Item code: %s\n", input.code);
		printf("Item name: %s\n", input.name);
		printf("Price    : RM %.2f\n", input.price);
		printf("Quantity : %d\n", input.quantity);
	}
}

//This function deletes an item
void deleteItem(struct ItemArray gst, struct ItemArray ngst){
	puts("This option allows user to delete items");
}

//This function shows the current inventory
void showInventory(struct ItemArray gst, struct ItemArray ngst){
	int i;

	//Print GST items
	printf("GST items:\n\n");
	printf("%s\t%-30s\t%8s    %s\n",
		"Code", "Name", "Price", "Quantity");
	puts("-------------------------------------------------------------");
	for(i = 0; i < gst.size; i++){
		int remaining = gst.array[i].initialQuantity - gst.array[i].itemsSold;
		printf("%s\t%-30s\t%8.2f    %8d\n",
			gst.array[i].code, gst.array[i].name, gst.array[i].price, remaining
			);
	}
	printf("\n\n");

	//Print Non-GST items
	printf("Non-GST items:\n\n");
	printf("%s\t%-30s\t%8s    %s\n",
		"Code", "Name", "Price", "Quantity");
	puts("---------------------------------------------------------------");
	for(i = 0; i < ngst.size; i++){
		int remaining = ngst.array[i].initialQuantity - ngst.array[i].itemsSold;
		printf("%s\t%-30s\t%8.2f    %8d\n",
			ngst.array[i].code, ngst.array[i].name, ngst.array[i].price, remaining
			);
	}
	
	//Prompt user to enter to continue
	printf("\nEnter to continue...  ");
	dumpRemainingInput();
}

//This function shows the transactions made today
void dailyTransactions(struct ItemArray gst, struct ItemArray ngst){
	int i; char excess;
	int totalItemSold;
	double gstTotal, ngstTotal, gstCollected;

	//Initalise all to 0
	totalItemSold = 0;
	gstTotal = 0; ngstTotal = 0;
	gstCollected = 0;

	//Get total from gst
	for(i = 0; i < gst.size; i++){
		totalItemSold += gst.array[i].itemsSold; //Increment total items sold
		gstTotal += gst.array[i].price * gst.array[i].itemsSold; //Calculate total sales made with GST items
	}

	gstCollected = gstTotal * 0.06; //Calculate GST collected

	for(i = 0; i < ngst.size; i++){ //Get total from ngst
		totalItemSold += ngst.array[i].itemsSold; //Increment total items sold
		ngstTotal += ngst.array[i].price * ngst.array[i].itemsSold; //Calculate total sales made with Non-GST items
	}
	
	//Print out results to console
	printf("Total transaction\t: %d\n", totalItemSold);
	printf("Sales with GST\t\t: RM %.2f\n", gstTotal);
	printf("Sales without GST\t: RM %.2f\n", ngstTotal);
	printf("GST collected\t\t: RM %.2f\n", gstCollected);

	//Prompt user to enter to continue
	printf("\n\nEnter to continue...  ");
	dumpRemainingInput();
}

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