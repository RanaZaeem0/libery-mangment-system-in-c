#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

#define IN 1
#define OUT 0

void Addbook();
void Searchbook();
void Displaybook();
void Author();
void Stock();
void Issue();
void bookret();
void Addmembr();
void Exit();

char info[500];

struct
{
  int bid;
  char bname[25] ;
  char author[25];
  char titles[500];
  int status;
  int stock;
}book;
struct
{
  int mid;
  char mname[25] ;
  char department[25];
  int availibcard;
  int phno;

}membr;

//initializing the files used in the program

FILE *librecord;
FILE *membrrecord;
FILE *fp1;
FILE *fp2;
FILE *temp1;
FILE *temp2;
int main()
{
    int choice=0,i;

    printf("\n\t\t---Library Management System by rana zaeem  ---\n");
    do{
    printf("\n\t--MENU--\n \n 1. Add A New Book\n 2. Search a book \n 3. Display Complete Information\n 4. Display All Books of An Author\n 5. List Count of Books (Issued & On Stock)\n 6. To Issue a Book \n 7. To Return a Book \n 8. Add A New Member\n 9.Exit the program\n\n\t Enter your choice <1-9>: ");
     if (scanf("%d", &choice) != 1) {
            // Handle invalid input (non-integer)
            printf("! Invalid input. Please enter a number between 1 and 5.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

    switch (choice)
    {
        case 1:
            Addbook();
            break;
        case 2:
            Searchbook();
            break;
        case 3:
            Displaybook();
            break;
        case 4:
            Author();
            break;
        case 5:
            Stock();
            break;
        case 6:
            Issue();
            break;
        case 7:
            bookret();
            break;
        case 8:
            Addmembr();
            break;
        case 9:
            Exit();
        default:
            printf(" ! Invalid Input...\n");
    }
}while(choice!=10);
 return (0);
}
void Addbook() {
    int found = 0;
    int existing_id;
    char existing_bname[25], existing_author[25];
    int existing_status;
    int MAX_NAME_LENGTH = 25;

    // Open the librecord file in append+read mode
    librecord = fopen("librecord.txt", "a+");
    if (librecord == NULL) {
        printf("! Error opening file.\n");
        return;
    }

    // Get Book ID
    printf("Enter the Unique ID of the Book (Integer):\n");
    scanf("%d", &book.bid);

    // Check for duplicate Book ID
    while (fscanf(librecord, "%d %[^\t] %[^\t] %d", &existing_id, existing_bname, existing_author, &existing_status) == 4) {
        if (existing_id == book.bid) {
            found = 1;
            break;
        }
    }

    if (found) {
        printf("! Invalid Book ID. The ID already exists.\n");
        fclose(librecord);
        return;
    }

    // Clear the file pointer for appending
    fseek(librecord, 0, SEEK_END);

    // Clear input buffer
    while (getchar() != '\n'); // Consume any leftover characters in the buffer

    // Get Book Name
    printf("Enter the Name of the Book (Max %d characters):\n", MAX_NAME_LENGTH - 1);
    fgets(book.bname, MAX_NAME_LENGTH, stdin);
    book.bname[strcspn(book.bname, "\n")] = '\0'; // Remove trailing newline

    if (strlen(book.bname) >= MAX_NAME_LENGTH) {
        printf("! Error: Book name too long.\n");
        fclose(librecord);
        return;
    }

    // Get Author Name
    printf("Enter the Name of the Author (Max %d characters):\n", MAX_NAME_LENGTH - 1);
    fgets(book.author, MAX_NAME_LENGTH, stdin);
    book.author[strcspn(book.author, "\n")] = '\0'; // Remove trailing newline

    if (strlen(book.author) >= MAX_NAME_LENGTH) {
        printf("! Error: Author name too long.\n");
        fclose(librecord);
        return;
    }

    // Get Stock
    printf("Enter the Stock of the Book (Integer):\n");
    scanf("%d", &book.stock);

    // Set Book Status
    book.status = 1; // Assuming '1' means the book is available (IN)

    // Write book details to the file
    fprintf(librecord, "%d\t%s\t%s\t%d\t%d\n", book.bid, book.bname, book.author, book.status,book.stock);
    fclose(librecord);

    printf("(' ') A New Book has been Added Successfully...\n");
}

void Displaybook() {
    // Open library record file in read mode
    librecord = fopen("librecord.txt", "r");
    if (librecord == NULL) {
        printf("\n! Error: Could not open librecord.txt or file does not exist.\n");
        return;
    }

    printf("\n--- Book Records ---\n");
    printf("%-8s %-24s %-24s %-6s %-6s\n", "Book ID", "Book Name", "Author", "Status","Stock");

    // Read and display book records
    int bookID, status,stock;
    char bookName[25], author[25];
    while (fscanf(librecord, "%d %[^\t] %[^\t] %d \t %d", &bookID, bookName, author, &status,&stock) == 5) {
        printf("%-8d %-24s %-24s %-6d %-6d\n", bookID, bookName, author, status,stock);
    }
    fclose(librecord);

    // Open member record file in read mode
    membrrecord = fopen("membrrecord.txt", "r");
    if (membrrecord == NULL) {
        printf("\n! Error: Could not open membrrecord.txt or file does not exist.\n");
        return;
    }

    printf("\n--- Member Records ---\n");
    printf("%-12s %-16s %-16s %-10s %-15s\n", "Member ID", "Name", "Department", "Phone", "Available Cards");

    // Read and display member records
    int memberID, availableCards;
    char name[16], department[16], phone[10];
    while (fscanf(membrrecord, "%d %[^\t] %[^\t] %[^\t] %d", &memberID, name, department, phone, &availableCards) == 5) {
        printf("%-12d %-16s %-16s %-10s %-15d\n", memberID, name, department, phone, availableCards);
    }
    fclose(membrrecord);
}



void Searchbook()
{
    int i;
    char Target[25],stats[3];
    int Found=0;
    if((librecord=fopen("librecord.txt","r"))==NULL)
        printf(" ! The File is Empty...\n\n");
    else
    {
        printf("\nEnter The Name Of Book : ");
            scanf("%s",Target);
        while(!feof(librecord)&& Found==0)
        {
        fscanf(librecord,"%d %s %s %d %d", &book.bid,book.bname,book.author,&book.status,&book.stock);
            if(strcmp(Target,book.bname)==0)
                Found=1;
        }
        if(Found)
        {
            if(book.status==IN)
                strcpy(stats,"IN");
            else
                strcpy(stats,"OUT");

            printf("\nThe Unique ID of The Book:  %d\nThe Name of Book is:  %s\nThe Author is:  %s\nThe Book Status:%s\n\n",book.bid,book.bname,book.author,stats);
            }
        else if(!Found)
            printf("! There is no such Entry...\n");
        fclose(librecord);
    }

}

void Author()
{
    int i;
    char Target[500];
    int Found=0;
    if((librecord=fopen("librecord.txt","r"))==NULL)
    printf(" ! The file is empty... \n\n");
    else
    {
        printf("\nEnter The Name Of Author : ");
            scanf("%s",Target);
        printf("\nBooks:");
        while(!feof(librecord))
        {
            fscanf(librecord,"%d %s %s %d %d",&book.bid,book.bname,book.author,&book.status,book.stock);
            if(strcmp(Target,book.author)==0)
            {
                Found=1;
                printf("\n Book Name :\t%s",book.bname);
            }
        }
        if(!Found)
            printf(" ! There is no such Entry...\n");
        fclose(librecord);
    }

}

void Stock() {
    int issuecount = 0, stockcount = 0;

    // Open the library record file
    if ((librecord = fopen("librecord.txt", "r")) == NULL) {
        printf("! The file is empty or cannot be opened.\n\n");
        return;
    }

    // Read each record from the file
    while (fscanf(librecord, "%d %[^\t] %[^\t] %d %d", &book.bid, book.bname, book.author, &book.status,&book.stock) == 5) {
        if (book.status == IN) {
            stockcount++;
        } else {
            issuecount++;
        }
    }

    fclose(librecord);

    // Display the stock and issue counts
    printf("\nCount of issued Books: %d\n", issuecount);
    printf("Count of Books in Stock: %d\n", stockcount);
}

void Addmembr() {
    int existing_id, found = 0;
    char existing_name[50], existing_department[50];
    long existing_phno;

    // Open file for appending and reading
    membrrecord = fopen("membrrecord.txt", "a+");
    if (membrrecord == NULL) {
        printf("! Error opening file.\n");
        return;
    }

    // Get User ID
    printf("Enter The User ID of the Member (Integer):\n");
    scanf("%d", &membr.mid);

    // Check if ID already exists
    while (fscanf(membrrecord, "%d %s %s %ld %d", &existing_id, existing_name, existing_department, &existing_phno, &existing_id) == 5) {
        if (existing_id == membr.mid) {
            found = 1;
            break;
        }
    }

    if (found) {
        printf("! Invalid User ID. The ID already exists.\n");
        fclose(membrrecord);
        return;
    }

    // Get Member Details
    printf("Enter The Name of the Member:\n");
    scanf("%s", membr.mname);
    printf("Enter The Department:\n");
    scanf("%s", membr.department);
    printf("Enter The Phone Number of the Member:\n");
    scanf("%ld", &membr.phno);

    // Set Library Card Availability
    membr.availibcard = 5;

    // Write Member Data to File
    fprintf(membrrecord, "%d\t%s\t%s\t%ld\t%d\n", membr.mid, membr.mname, membr.department, membr.phno, membr.availibcard);
    fclose(membrrecord);

    printf("\n(' ') Added A New Member Successfully...\n");
}

void Issue()
{
    int mid,i,Found1=0,Found2=0;char issubookname[20];
    //temp1=librecord;temp2=membrrecord;
    printf("\nEnter The userid of the Member : \n");
        scanf("%d",&mid);
    if((membrrecord=fopen("membrrecord.txt","r"))==NULL)
        printf(" ! The file is empty...\n\n");
    else
    {
        while(!feof(membrrecord)&& Found1==0)
        {
            fscanf(membrrecord,"%d %s %s %d %d ",&membr.mid,membr.mname,membr.department,&membr.phno,&membr.availibcard);
            if(mid==membr.mid)
            {
                Found1=1;
            }
        }
        if(Found1)
        {
            if(membr.availibcard<1)
            {
                printf(" ! Library card not available...\n");
            }
            else
            {    printf("\nEnter The Name of book :");
                scanf("%s",issubookname);
                if((librecord=fopen("librecord.txt","r"))==NULL)
                    printf(" ! The file is empty...\n\n");
                else
                {
                    while(!feof(librecord)&& Found2==0)
                    {
                        fscanf(librecord,"%d %s %s %d %d", &book.bid,book.bname,book.author,&book.status,&book.stock);
                        if(strcmp(issubookname,book.bname)==0)
                            Found2=1;
                    }
                    if(Found2)
                    {
                        if(book.status==0)
                        {
                            printf(" ! Book already issued...\n");
                        }
                        else
                        {

                            fp2=fopen("fp2.txt","w");
                            if((temp2=fopen("membrrecord.txt","r"))==NULL)
                                printf(" ! The file is empty...\n\n");
                            else
                            {
                                while(!feof(temp2))
                                {
                                    fscanf(temp2,"%d %s %s %d %d ",&membr.mid,membr.mname,membr.department,&membr.phno,&membr.availibcard);


                                    if(mid==membr.mid)
                                    {
                                        membr.availibcard--;
                                        fprintf(fp2,"\n %d\t%s\t%s\t%d\t%d\t",membr.mid,membr.mname,membr.department,membr.phno,    membr.availibcard);
                                    }
                                    else{
                                        fprintf(fp2,"\n %d\t%s\t%s\t%d\t%d\t",membr.mid,membr.mname,membr.department,membr.phno,membr.availibcard);}
                                    if(feof(temp2))
                                        break;
                                }
                            }
                            fclose(temp2);
                            fclose(fp2);


                            fp1=fopen("fp1.txt","w");
                            if((temp1=fopen("librecord.txt","r"))==NULL)
                                printf(" ! The file is empty...\n\n");
                            else
                            {
                                while(!feof(temp1))
                                {
                                      fscanf(temp1,"%d %s %s %d %d", &book.bid,book.bname,book.author,&book.status,&book.stock);
                                    if(feof(temp1))
                                        break;
                                    if(strcmp(issubookname,book.bname)!=0)
                                    {
                                        fprintf(fp1,"\n%d\t%s\t%s\t%d\t%d    \t",book.bid,book.bname,book.author,book.status,&book.stock);
                                    }
                                    else
                                    {
                                        fprintf(fp1,"\n%d\t%s\t%s\t%d\t%d\t%d\t",book.bid,book.bname,book.author,book.stock,0);
                                    }
                                }
                            }
                            fclose(temp1);
                            fclose(fp1);
                            fclose(librecord);
                            fclose(membrrecord);
                            remove("librecord.txt");
                            rename("fp1.txt","librecord.txt");
                            remove("membrrecord.txt");
                            rename("fp2.txt","membrrecord.txt");
                            printf(" (' ') Book Successfully issued...\n");
                        }
                    }
                    else if(!Found2)
                        printf(" ! There is no such Book...\n");

                }
            }
        }
        else if(!Found1)
            printf(" ! Invalid User id...\n");


    }

}


void bookret() {
    int mid, Found1 = 0, Found2 = 0;
    char retbookname[50]; // Increased size to handle larger names

    printf("\nEnter the User ID of the Member:\n");
    scanf("%d", &mid);

    // Open the member record file
    if ((membrrecord = fopen("membrrecord.txt", "r")) == NULL) {
        printf("! Error: Member record file is empty or cannot be opened.\n\n");
        return;
    }

    // Find the member
    while (fscanf(membrrecord, "%d %s %s %d %d", &membr.mid, membr.mname, membr.department, &membr.phno, &membr.availibcard) == 5) {
        if (mid == membr.mid) {
            Found1 = 1;
            break;
        }
    }
    fclose(membrrecord);

    if (!Found1) {
        printf("! Invalid User ID.\n");
        return;
    }

    if (membr.availibcard >= 5) {
        printf("! Error: Member has already returned all books.\n");
        return;
    }

    printf("\nEnter the Name of the Book:\n");
    scanf(" %[^\n]", retbookname); // Allows multi-word book names

    // Open the library record file
    if ((librecord = fopen("librecord.txt", "r")) == NULL) {
        printf("! Error: Library record file is empty or cannot be opened.\n\n");
        return;
    }

    FILE *tempLib = fopen("fp1.txt", "w");
    if (!tempLib) {
        printf("! Error: Unable to create temporary library file.\n");
        fclose(librecord);
        return;
    }

    // Search for the book and update its status
    while (fscanf(librecord, "%d %s %s %d %d", &book.bid, book.bname, book.author, &book.status, &book.stock) == 5) {
        if (strcmp(retbookname, book.bname) == 0) {
            Found2 = 1;
            if (book.status == 1) {
                printf("! Error: Book is already in stock.\n");
                fclose(librecord);
                fclose(tempLib);
                remove("fp1.txt");
                return;
            }
            book.status = 1; // Mark book as returned
        }
        fprintf(tempLib, "%d %s %s %d %d\n", book.bid, book.bname, book.author, book.status, book.stock);
    }
    fclose(librecord);
    fclose(tempLib);

    if (!Found2) {
        printf("! Error: Book not found in library records.\n");
        remove("fp1.txt");
        return;
    }

    // Update the member record
    FILE *tempMembr = fopen("fp2.txt", "w");
    if (!tempMembr) {
        printf("! Error: Unable to create temporary member file.\n");
        return;
    }

    membrrecord = fopen("membrrecord.txt", "r");
    while (fscanf(membrrecord, "%d %s %s %d %d", &membr.mid, membr.mname, membr.department, &membr.phno, &membr.availibcard) == 5) {
        if (mid == membr.mid) {
            membr.availibcard++;
        }
        fprintf(tempMembr, "%d %s %s %d %d\n", membr.mid, membr.mname, membr.department, membr.phno, membr.availibcard);
    }
    fclose(membrrecord);
    fclose(tempMembr);

    // Replace the old files with updated ones
    remove("librecord.txt");
    rename("fp1.txt", "librecord.txt");

    remove("membrrecord.txt");
    rename("fp2.txt", "membrrecord.txt");

    printf("('') Book Successfully Returned...\n");
}

void Exit()
{
  exit(0);
}
