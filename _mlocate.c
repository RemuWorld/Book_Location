#include <stdio.h>
#include "_mlocate.h"

// Function to read file content
char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(length + 1);
    if (!content) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    fread(content, 1, length, file);
    content[length] = '\0';
    fclose(file);
    return content;
}

// Method 1: Locate book by 6-digit code
void locate_by_code(const char* base_path, const char* code) {


    // Validate input length
    if (strlen(code) != 6) {
        printf("Error: Code must be exactly 6 digits.\n");
        return;
    }

    // Build folder and file path
    char folder1[20], folder2[20], folder3[20], file_path[300];
    int top_level = code[0] - '0'; // First digit
    int sub_level = code[1] - '0'; // Second digit
    int sub_sub_level = code[2] - '0'; // Third digit
    sprintf(folder1, "%d00000", top_level);
    sprintf(folder2, "%d%d0000", top_level, sub_level);
    sprintf(folder3, "%d", sub_sub_level);
    sprintf(file_path, "%s/%s/%s/%s/location.png", base_path, folder1, folder2, folder3);

    // Open the file
    FILE* file = fopen(file_path, "r");
    if (!file) {
        printf("File not found: %s\n", file_path);
        return;
    }
    fclose(file);

    // Display the picture
    printf("Opening file: %s\n", file_path);
    char command[350];
    sprintf(command, "start \"\" \"%s\"", file_path); // Windows-specific
    system(command);
}

// Method 2: Search by book name
void search_by_name(const char* filename, const char* base_path) {
    char* content = read_file(filename);
    if (!content) return;

    cJSON* root = cJSON_Parse(content);
    if (!root) {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        free(content);
        return;
    }

    // Get the "books" array
    cJSON* books = cJSON_GetObjectItem(root, "books");
    if (!cJSON_IsArray(books)) {
        printf("Invalid JSON structure: 'books' is not an array.\n");
        cJSON_Delete(root);
        free(content);
        return;
    }

    // Get user input
    char query[100];
    printf("Enter part of the book name: ");
    scanf(" %[^\n]s", query); // Read input with spaces

    // Search for matches
    printf("\nSearch Results:\n");
    cJSON* book;
    int index = 1;
    cJSON_ArrayForEach(book, books) {
        cJSON* title = cJSON_GetObjectItem(book, "title");
        if (cJSON_IsString(title) && strstr(title->valuestring, query)) {
            printf("%d. %s\n", index++, title->valuestring);
        }
    }

    // Get user selection
    if (index == 1) {
        printf("No matches found.\n");
    } else {
        int choice;
        printf("\nEnter the number of your choice: ");
        scanf("%d", &choice);

        if (choice > 0 && choice < index) {
            cJSON* selected = cJSON_GetArrayItem(books, choice - 1);
            cJSON* code = cJSON_GetObjectItem(selected, "code");
            if (cJSON_IsString(code)) {
                printf("Selected book code: %s\n", code->valuestring);

                // Call locate_by_code to find and open the picture
                locate_by_code(base_path, code->valuestring);
            }
        } else {
            printf("Invalid choice.\n");
        }
    }

    // Cleanup
    cJSON_Delete(root);
    free(content);
}