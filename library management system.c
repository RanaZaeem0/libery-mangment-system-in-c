#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define IN 1
#define OUT 0

void Addbook();
void Searchbook();
void Displaybook();
void Author();
void Stock();
void Issue();
void bookReturn();
void Addmembr();
void DisplayIssuedBooks();
void ShowBooksIssuedByMember();
void Exit();

char info[500];

struct
{
    int bid;
    char bname[25];
    char author[25];
    char titles[500];
    int status;
} book;

struct
{
    int mid;
    char mname[25];
    char department[25];
    int availibcard;
    int phno;
} membr;

FILE *librecord;
FILE *membrrecord;
FILE *fp1;
FILE *fp2;
FILE *temp1;
FILE *temp2;

int main()
{
    int choice = 0, i;

    printf("\n\t\t---Library Management System by rana zaeem  ---\n");
    do
    {
        printf("\n\t--MENU--\n \n 1. Add A New Book\n 2. Search a book \n 3. Display Complete Information\n 4. Display All Books of An Author\n 5. List Count of Books (Issued & On Stock)\n 6. To Issue a Book \n 7. To Return a Book \n 8. Add A New Member\n 9. Display all Issue book Member\n  10.Exit the program\n\n\t Enter your choice <1-9>: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("! Invalid input. Please enter a number between 1 and 9.\n");
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
            bookReturn();
            break;
        case 8:
            Addmembr();
            break;
        case 9:
            DisplayIssuedBooks();
            break;
        case 10:
            Exit();
        default:
            printf(" ! Invalid Input...\n");
        }
    } while (choice != 10);
    return (0);
}

void Addbook()
{
    int found = 0;
    int existing_id;
    char existing_bname[25], existing_author[25];
    int existing_status;
    int MAX_NAME_LENGTH = 25;

    librecord = fopen("librecord.txt", "a+");
    if (librecord == NULL)
    {
        printf("! Error opening file.\n");
        return;
    }

    printf("Enter the Unique ID of the Book (Integer):\n");
    scanf("%d", &book.bid);
    // %*[^\n]: Skips everything else on the line until the newline character (\n).
    while (fscanf(librecord, "%d%*[^\n]", &existing_id) == 1)
    {
        if (existing_id == book.bid)
        {
            found = 1;
            break;
        }
    }

    if (found)
    {
        printf("! Invalid Book ID. The ID already exists.\n");
        fclose(librecord);
        return;
    }

    fseek(librecord, 0, SEEK_END);

    while (getchar() != '\n')
        ;

    printf("Enter the Name of the Book (Max %d characters):\n", MAX_NAME_LENGTH - 1);
    fgets(book.bname, MAX_NAME_LENGTH, stdin);
    book.bname[strcspn(book.bname, "\n")] = '\0';

    if (strlen(book.bname) >= MAX_NAME_LENGTH)
    {
        printf("! Error: Book name too long.\n");
        fclose(librecord);
        return;
    }

    printf("Enter the Name of the Author (Max %d characters):\n", MAX_NAME_LENGTH - 1);
    fgets(book.author, MAX_NAME_LENGTH, stdin);
    book.author[strcspn(book.author, "\n")] = '\0';

    if (strlen(book.author) >= MAX_NAME_LENGTH)
    {
        printf("! Error: Author name too long.\n");
        fclose(librecord);
        return;
    }

    book.status = 1;

    fprintf(librecord, "%d\t%s\t%s\t%d\n", book.bid, book.bname, book.author, book.status);
    fclose(librecord);

    printf("(' ') A New Book has been Added Successfully...\n");
}

void Displaybook()
{
    librecord = fopen("librecord.txt", "r");
    if (librecord == NULL)
    {
        printf("\n! Error: Could not open librecord.txt or file does not exist.\n");
        return;
    }

    printf("\n--- Book Records ---\n");
    printf("%-8s %-24s %-24s %-6s\n", "Book ID", "Book Name", "Author", "Status");

    int bookID, status;
    char bookName[25], author[25];
    while (fscanf(librecord, "%d %[^\t] %[^\t] %d", &bookID, bookName, author, &status) == 4)
    {
        printf("%-8d %-24s %-24s %-6d\n", bookID, bookName, author, status);
    }
    fclose(librecord);

    membrrecord = fopen("membrrecord.txt", "r");
    if (membrrecord == NULL)
    {
        printf("\n! Error: Could not open membrrecord.txt or file does not exist.\n");
        return;
    }

    printf("\n--- Member Records ---\n");
    printf("%-12s %-16s %-16s %-10s %-15s\n", "Member ID", "Name", "Department", "Phone", "Available Cards");

    int memberID, availableCards;
    char name[16], department[16], phone[10];
    while (fscanf(membrrecord, "%d %[^\t] %[^\t] %[^\t] %d", &memberID, name, department, phone, &availableCards) == 5)
    {
        printf("%-12d %-16s %-16s %-10s %-15d\n", memberID, name, department, phone, availableCards);
    }
    fclose(membrrecord);
}

void Searchbook()
{
    int i;
    char Target[25], stats[3];
    int Found = 0;
    if ((librecord = fopen("librecord.txt", "r")) == NULL)
        printf(" ! The File is Empty...\n\n");
    else
    {
        printf("\nEnter The Name Of Book : ");
        scanf("%s", Target);
        while (!feof(librecord) && Found == 0)
        {
            fscanf(librecord, "%d %s %s %d", &book.bid, book.bname, book.author, &book.status);
            if (strcmp(Target, book.bname) == 0)
                Found = 1;
        }
        if (Found)
        {
            if (book.status == IN)
                strcpy(stats, "IN");
            else
                strcpy(stats, "OUT");

            printf("\nThe Unique ID of The Book:  %d\nThe Name of Book is:  %s\nThe Author is:  %s\nThe Book Status:%s\n\n", book.bid, book.bname, book.author, stats);
        }
        else if (!Found)
            printf("! There is no such Entry...\n");
        fclose(librecord);
    }
}

void Author()
{
    int i;
    char Target[500];
    int Found = 0;

    if ((librecord = fopen("librecord.txt", "r")) == NULL)
        printf(" ! The file is empty... \n\n");
    else
    {
        printf("\nEnter The Name Of Author: ");
        scanf("%s", Target);
        printf("\nBooks:");

        while (fscanf(librecord, "%d %s %s %d", &book.bid, book.bname, book.author, &book.status) == 4)
        {
            if (strcmp(Target, book.author) == 0)
            {
                Found = 1;
                printf("\n Book Name:\t%s", book.bname);
            }
        }

        if (!Found)
            printf(" ! There is no such Entry...\n");

        fclose(librecord);
    }
}

void Stock()
{
    int issuecount = 0, stockcount = 0;

    if ((librecord = fopen("librecord.txt", "r")) == NULL)
    {
        printf("! The file is empty or cannot be opened.\n\n");
        return;
    }

    while (fscanf(librecord, "%d %[^\t] %[^\t] %d", &book.bid, book.bname, book.author, &book.status) == 4)
    {
        if (book.status == IN)
        {
            stockcount++;
        }
        else
        {
            issuecount++;
        }
    }

    fclose(librecord);

    printf("\nCount of issued Books: %d\n", issuecount);
    printf("Count of Books in Stock: %d\n", stockcount);
}

void Addmembr()
{
    int existing_id, found = 0;
    char existing_name[50], existing_department[50];
    long existing_phno;

    // Open file for appending and reading
    membrrecord = fopen("membrrecord.txt", "a+");
    if (membrrecord == NULL)
    {
        printf("! Error opening file.\n");
        return;
    }

    // Get User ID
    printf("Enter The User ID of the Member (Integer):\n");
    scanf("%d", &membr.mid);

    // Check if ID already exists
    while (fscanf(membrrecord, "%d %s %s %ld %d", &existing_id, existing_name, existing_department, &existing_phno, &existing_id) == 5)
    {
        if (existing_id == membr.mid)
        {
            found = 1;
            break;
        }
    }

    if (found)
    {
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
    int mid, Found1 = 0, Found2 = 0;
    char issubookname[25]; // Book name for issuing

    // Get Member ID
    printf("\nEnter The User ID of the Member:\n");
    if (scanf("%d", &mid) != 1)
    {
        printf("! Invalid input for User ID.\n");
        return;
    }

    // Check if the member exists
    membrrecord = fopen("membrrecord.txt", "r");
    if (membrrecord == NULL)
    {
        printf("! Error: Member record file is empty or cannot be opened.\n");
        return;
    }

    while (fscanf(membrrecord, "%d %s %s %d %d",
                  &membr.mid, membr.mname, membr.department, &membr.phno, &membr.availibcard) == 5)
    {
        if (mid == membr.mid)
        {
            Found1 = 1;
            break;
        }
    }
    fclose(membrrecord);

    if (!Found1)
    {
        printf("! Invalid User ID.\n");
        return;
    }

    if (membr.availibcard < 1)
    {
        printf("! No library cards available. Return some books first.\n");
        return;
    }

    // Get Book Name
    printf("\nEnter The Name of Book:\n");
    if (scanf(" %[^\n]", issubookname) != 1)
    {
        printf("! Invalid input for Book Name.\n");
        return;
    }

    // Check if the book exists
    librecord = fopen("librecord.txt", "r");
    if (librecord == NULL)
    {
        printf("! Error: Library record file is empty or cannot be opened.\n");
        return;
    }

    while (fscanf(librecord, "%d %s %s %d",
                  &book.bid, book.bname, book.author, &book.status) == 4)
    {
        if (strcmp(issubookname, book.bname) == 0)
        {
            Found2 = 1;
            break;
        }
    }
    fclose(librecord);

    if (!Found2)
    {
        printf("! Book not found in library records.\n");
        return;
    }

    if (book.status == 0)
    {
        printf("! Book is already issued to someone else.\n");
        return;
    }

    FILE *tempMembr;
    // Update Member Record
    tempMembr = fopen("fp2.txt", "w");
    if (tempMembr == NULL)
    {
        printf("! Error creating temporary member file.\n");
        return;
    }

    membrrecord = fopen("membrrecord.txt", "r");
    while (fscanf(membrrecord, "%d %s %s %d %d",
                  &membr.mid, membr.mname, membr.department, &membr.phno, &membr.availibcard) == 5)
    {
        if (mid == membr.mid)
        {
            membr.availibcard--;
        }
        fprintf(tempMembr, "%d\t%s\t%s\t%d\t%d\n",
                membr.mid, membr.mname, membr.department, membr.phno, membr.availibcard);
    }
    fclose(membrrecord);
    fclose(tempMembr);

    FILE *tempLib;
    // Update Library Record
    tempLib = fopen("fp1.txt", "w");
    if (tempLib == NULL)
    {
        printf("! Error creating temporary library file.\n");
        remove("fp2.txt"); // Clean up first temporary file
        return;
    }

    librecord = fopen("librecord.txt", "r");
    FILE *issuedBooks = fopen("issuedbooks.txt", "a");
    if (issuedBooks == NULL)
    {
        printf("! Error: Could not open issued books file.\n");
        fclose(librecord);
        fclose(tempLib);
        remove("fp2.txt");
        return;
    }

    while (fscanf(librecord, "%d %s %s %d",
                  &book.bid, book.bname, book.author, &book.status) == 4)
    {
        if (strcmp(issubookname, book.bname) == 0)
        {
            book.status = 0; // Mark book as issued
            // Add correct book details to issuedbooks.txt
            fprintf(issuedBooks, "%d\t%d\t%s\t%s\n",
                    mid, book.bid, book.bname, book.author);
        }
        fprintf(tempLib, "%d\t%s\t%s\t%d\n",
                book.bid, book.bname, book.author, book.status);
    }
    fclose(librecord);
    fclose(tempLib);
    fclose(issuedBooks);

    // Replace original files with updated ones
    remove("librecord.txt");
    rename("fp1.txt", "librecord.txt");
    remove("membrrecord.txt");
    rename("fp2.txt", "membrrecord.txt");

    printf("\n('') Book Successfully Issued!\n");
}

void bookReturn() {
    int mid, Found1 = 0, Found2 = 0;
    char retbookname[50];
    FILE *issuedBooksFile, *tempIssuedBooks;
    int foundRecord = 0;

    // Get member ID
    printf("\nEnter the User ID of the Member:\n");
    scanf("%d", &mid);

    // Check if member exists
    membrrecord = fopen("membrrecord.txt", "r");
    if (membrrecord == NULL) {
        printf("! Error: Member record file is empty or cannot be opened.\n");
        return;
    }

    // Search for member
    while (fscanf(membrrecord, "%d %s %s %d %d", 
                  &membr.mid, membr.mname, membr.department, &membr.phno, &membr.availibcard) == 5) {
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

    // Get book name
    printf("\nEnter the Name of the Book:\n");
    scanf(" %[^\n]", retbookname);  // Notice the space before %[^\n] to allow multi-word inputs

    // Check if book exists and is issued
    librecord = fopen("librecord.txt", "r");
    if (librecord == NULL) {
        printf("! Error: Library record file cannot be opened.\n");
        return;
    }

    // Create temporary file for library records
    FILE *tempLib = fopen("fp1.txt", "w");
    if (tempLib == NULL) {
        printf("! Error: Unable to create temporary file.\n");
        fclose(librecord);
        return;
    }

    // Search for book and update its status
    while (fscanf(librecord, "%d %s %s %d", 
                  &book.bid, book.bname, book.author, &book.status) == 4) {
        if (strcmp(retbookname, book.bname) == 0) {
            Found2 = 1;
            if (book.status == 1) {
                printf("! Error: Book is already in stock.\n");
                fclose(librecord);
                fclose(tempLib);
                remove("fp1.txt");
                return;
            }
            book.status = 1; // Mark as returned
        }
        fprintf(tempLib, "%d\t%s\t%s\t%d\n",
                book.bid, book.bname, book.author, book.status);
    }
    fclose(librecord);
    fclose(tempLib);

    if (!Found2) {
        printf("! Error: Book not found in library records.\n");
        remove("fp1.txt");
        return;
    }

    // Update member record
    FILE *tempMembr = fopen("fp2.txt", "w");
    if (tempMembr == NULL) {
        printf("! Error: Unable to create temporary member file.\n");
        remove("fp1.txt");
        return;
    }

    membrrecord = fopen("membrrecord.txt", "r");
    while (fscanf(membrrecord, "%d %s %s %d %d", 
                  &membr.mid, membr.mname, membr.department, &membr.phno, &membr.availibcard) == 5) {
        if (mid == membr.mid) {
            membr.availibcard++; // Increment available cards
        }
        fprintf(tempMembr, "%d\t%s\t%s\t%d\t%d\n", 
                membr.mid, membr.mname, membr.department, membr.phno, membr.availibcard);
    }
    fclose(membrrecord);
    fclose(tempMembr);

    // Replace original files with updated ones
    remove("librecord.txt");
    rename("fp1.txt", "librecord.txt");
    remove("membrrecord.txt");
    rename("fp2.txt", "membrrecord.txt");

    // Handle issued books file: Remove the issued book record
    issuedBooksFile = fopen("issuedbooks.txt", "r");
    if (issuedBooksFile == NULL) {
        printf("! Error: Issued books file cannot be opened.\n");
        return;
    }

    tempIssuedBooks = fopen("fp3.txt", "w");
    if (tempIssuedBooks == NULL) {
        printf("! Error: Unable to create temporary file for issued books.\n");
        fclose(issuedBooksFile);
        return;
    }

    // Remove the issued book record from the file
    while (fscanf(issuedBooksFile, "%d\t%d\t%s\t%s\n", 
                  &mid, &book.bid, book.bname, book.author) == 4) {
        if (mid == membr.mid && strcmp(book.bname, retbookname) == 0) {
            foundRecord = 1; // Found the record to remove
        } else {
            fprintf(tempIssuedBooks, "%d\t%d\t%s\t%s\n", 
                    mid, book.bid, book.bname, book.author);
        }
    }

    fclose(issuedBooksFile);
    fclose(tempIssuedBooks);

    if (foundRecord) {
        // Replace original issued books file with the updated one
        remove("issuedbooks.txt");
        rename("fp3.txt", "issuedbooks.txt");
        printf("Book Successfully Returned...\n");
    } else {
        printf("! Error: No record of this book being issued to the member.\n");
        remove("fp3.txt");
    }
}

void DisplayIssuedBooks()
{
    FILE *issuedBooks;
    char buffer[200]; // Buffer to read lines from the file

    // Open the issued books file
    issuedBooks = fopen("issuedbooks.txt", "r");
    if (issuedBooks == NULL)
    {
        printf("! Error: No issued books record found.\n");
        return;
    }

    printf("\n--- Issued Books Details ---\n");

    // Read and print each line of the file
    while (fgets(buffer, sizeof(buffer), issuedBooks) != NULL)
    {
        printf("%s", buffer);
    }

    fclose(issuedBooks);
    printf("\n--- End of List ---\n");
}
// void ShowBooksIssuedByMember() {
//     int memberId;
//     int issuedMemberId, bookId;
//     char bookName[50], author[50];
//     int found = 0;

//     // Prompt the user for Member ID
//     printf("Enter the Member ID to check issued books: ");
//     scanf("%d", &memberId);

//     // Open the issued books file
//     FILE *issuedBooksFile = fopen("issuedbooks.txt", "r");
//     if (issuedBooksFile == NULL) {
//         printf("! Error: Issued books file cannot be opened or does not exist.\n");
//         return;
//     }

//     printf("\nBooks issued to Member ID %d:\n", memberId);
//     printf("--------------------------------------------------\n");

    
//     // Rewind the file to start checking for the Member ID
//     rewind(issuedBooksFile);

//     // Read through the file and display books issued to the given Member ID
//     while (fscanf(issuedBooksFile, "%d %d %[^\t] %[^\n]", &issuedMemberId, &bookId, bookName, author) == 4) {
//         if (issuedMemberId == memberId) {
//             found = 1;
//             printf("Book ID: %d\tBook Name: %s\tAuthor: %s\n", bookId, bookName, author);
//         }
//     }


//       fclose(issuedBooksFile);
//     if (!found) {
//         printf("No books issued to Member ID %d.\n", memberId);
//     }
// }


void Exit()
{
    exit(0);
}
