#include<stdio.h>

FILE *test;

struct {
    int bookNumber;
} book;

int main() {
    // Open the file in "a+" mode (append and read)
    test = fopen("test.txt", "a+");
    if (test == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    // Get book number input
    printf("Enter the Unique ID of the Book (Integer):\n");
    scanf("%d", &book.bookNumber);

    // Write to the file
    fprintf(test, "%d\n", book.bookNumber);
    fflush(test); // Ensure the data is written to the file

    // Rewind the file pointer to the beginning of the file
    rewind(test);

    // Read and print all book numbers
    printf("Book numbers in the file:\n");
    int bookNumber;
    while (fscanf(test, "%d", &bookNumber) == 1) {
        printf("Number: %d\n", bookNumber);
    }

    // Close the file
    fclose(test);

    return 0;
}
