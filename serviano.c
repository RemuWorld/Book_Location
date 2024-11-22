#include "_mlocate.h"

int main() {
    const char* base_path = "C:/Users/super/Desktop/libraryBooks"; // Adjust path as needed
    const char* json_file = "books.json"; // Adjust to your JSON file location

    int choice;

    while (1) {
        printf("\nChoose a method to find the book:\n");
        printf("1. Search by 6-digit code\n");
        printf("2. Search by book name\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char code[7]; // Buffer for 6-digit code + null terminator
                printf("Enter a 6-digit code: ");
                scanf("%6s", code);

                // Validate input (only digits and exactly 6 characters)
                if (strlen(code) == 6 && strspn(code, "0123456789") == 6) {
                    locate_by_code(base_path, code);
                } else {
                    printf("Invalid code. Please enter exactly 6 digits.\n");
                }
                break;
            }
            case 2:
                search_by_name(json_file, base_path);
                break;

            case 0:
                printf("Exiting the program.\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}