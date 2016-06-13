void editItem(struct ItemArray gst, struct ItemArray ngst){

  struct Item list;
  char code[6], name[100];
  double price;
  int choice, quantity, i;

  FILE *gst_file, *ngst_file;

  puts("Please enter the item code of the item you want to change.");
  puts("Enter -1 into the \"Item Code\" to exit.");

  // Takes in the input of the user
  do {

    printf("Item Code:\n");

    // Checks whether item code is empty
    do {
      scanf("%s", code);
    } while(getchar() != '\n');

    // Checks if the user wants to exit or not
    if(strcmp(code, "-1") != 0 ) {
      int itemCategory = doesItemExist(gst, ngst, code);

      // If item not found. Try again/
      if (itemCategory == -1) {
        printf("Item not found. Please try again.\n\n");
      }
      else {
        int isItemGst = (itemCategory == 1);
        int position = whereIsItem(
							(isItemGst)? gst : ngst,
							code
							);
        struct ItemArray selected = (isItemGst)? gst : ngst;

        printf("What would you like to edit?\n");
        printf("----------------------------\n");
        printf("1. Item Name\n");
        printf("2. Item Price\n");
        printf("3. Item Quantity\n");
        printf("----------------------------\n\n");

        // Checks if the user input is accurate
        do {
          scanf("%d", &choice);

          if(choice != 1 || choice != 2 || choice != 3) {
            printf("Invalid Choice, please select again.\n");
          }
        } while (choice != 1 || choice != 2 || choice != 3);

        // Choices
        switch(choice) {
          case '1': // Item name Change
                    printf("What is the new item name: \n");
                    scanf("%s", name); //Needs some adjustment
                    selected.array[position].name = name;
                    break;
          case '2': //Item Price Change
                    printf("What is the new item price: \n");
                    scanf("%f", &price);
                    selected.array[position].price = price;
                    break;
          case '3': // Item Quantity Change
                    printf("What is the new quantity: \n");
                    scanf("%d", &quantity);
                    selected.array[position].initialQuantity = quantity;
                    break;
        }

        // Change the file
        // To change the GST file
        if (isItemGst) {
          gst_file = fopen("gst.txt", "w");

          // Reprint the items into GST file
          for (i = 0; i < gst.size; i+=1) {
            if (isItemGst) {
              fprintf(gst_file, "%s;", selected.array[i].code);
              fprintf(gst_file, "%s;", selected.array[i].name);
              fprintf(gst_file, "%s;", selected.array[i].price);
              fprintf(gst_file, "%s;", selected.array[i].initialQuantity);
            }
          }

        }
        // To change the NGST file
        else {
          ngst_file = fopen("ngst.txt", "w");

          // Reprint the items into NGST file
          for (i = 0; i < ngst.size; i+=1) {
            if (!isItemGst) {
              fprintf(ngst_file, "%s;", selected.array[i].code);
              fprintf(ngst_file, "%s;", selected.array[i].name);
              fprintf(ngst_file, "%s;", selected.array[i].price);
              fprintf(ngst_file, "%s;", selected.array[i].initialQuantity);
            }
          }
        }
      }

    } // end if strcmp(code, "-1") != 0
    else {
      printf("Goodbye!\n");
      break;
    }

    puts("");
  } while (code != "-1"); // First do loop


}
