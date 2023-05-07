#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include "hashing.h"

#define ROWS 8
#define COLS 10
#define MAX_LEN 10

#define MAX_WORD_LENGTH 20

char matrix[ROWS][COLS];
char string[MAX_LEN + 1];
int visited[ROWS][COLS];

void findStrings(char** matrix, int row, int col, int len);

char** get_matrix_from_file(void);
void print_matrix(char **matrix);
//void search_words_in_matrix(char** matrix, int rows, int cols, int i, int j, char* word, int index);
void initVisited(bool** visited);


bool is_valid_word(char* word);
void find_words_util(char** matrix, int row, int col, char* word, bool** visited);
void find_words(char** matrix, bool** visited);

struct node* table[TABLE_SIZE];
char found[20];

int main() {
    setlocale(LC_ALL, "tr_TR");

    char** matrix = get_matrix_from_file();
    print_matrix(matrix);

    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = NULL;
    }
    FILE* fp;
    if (fopen_s(&fp, "sozluk.txt", "r") != 0) {
        printf("Error: Unable to open file\n");
        return 1;
    }

    char word[MAX_WORD_LENGTH];

    while (fgets(word, sizeof(word), fp) != NULL) {
        // Remove the newline character from the end of the word
        word[strcspn(word, "\n")] = '\0';
        insert(table, word, 1); // Set the value to 1 for each word
    }
    
    fclose(fp);

    // Allocate memory for the visited
    bool** visited = (bool**)malloc(8 * sizeof(bool*));
    for (int i = 0; i < 8; i++)
        visited[i] = (bool*)malloc(10 * sizeof(bool));

    initVisited(visited);


    int m, n;

//#pragma omp parallel for num_threads(4) shared(matrix) private(m,n, visited, string)
    for (m = 0; m < ROWS; m++) {
        for (n = 0; n < COLS; n++) {
            findStrings(matrix, m, n, 0);
        }
    }
    //findStrings(matrix, 1, 1, 0);

    /*
    omp_set_num_threads(4);
    find_words(matrix, visited);
    */

    /*
    //-----------------------------------------------------
    initVisited();
    

    memset(word, 0, sizeof(word));
    search_words_in_matrix(matrix, 8, 10, 0, 0, word, 0);
    //-----------------------------------------------------
    */

    //printf("Hash value of the word 'abaza': %d\n", get(table, "abaza"));
    //printf("Hash value of the word 'avmuy': %d\n", get(table, "avmuy"));
    return 0;
}

char** get_matrix_from_file() {
    FILE* fp;
    char* filename = "matrix.ini";
    int rows, cols, i, j;

    // Open the file in read mode
    fopen_s(&fp, filename, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        return "0";
    }
    else {
        // Read the matrix dimensions
        fscanf_s(fp, "%d %d ", &rows, &cols);
        printf("%d,%d boyutunda matris olusturuldu.\n", rows, cols);
    }

    // Allocate memory for the matrix
    char** matrix = (char**)malloc(rows * sizeof(char*));
    for (i = 0; i < rows; i++)
        matrix[i] = (char*)malloc(cols * sizeof(char));

    // Read the matrix elements
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            fscanf_s(fp, "%c ", &matrix[i][j]);
        }
    }

    // Close the file
    fclose(fp);
    return matrix;
}

void print_matrix(char** matrix) {
    FILE* fp;
    char* filename = "matrix.ini";
    int rows, cols, i, j;

    // Open the file in read mode
    fopen_s(&fp, filename, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        return;
    }
    else {
        // Read the matrix dimensions
        fscanf_s(fp, "%d %d", &rows, &cols);
    }

    // Print the matrix
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
    fclose(fp);
}

int isVowel(char c){

    return (c == 'A' || c == 'E' || c == 'I' || c == '?' || c == 'O' || c == 'Ö' || c == 'U' || c == 'Ü');
}

void initVisited(bool** visited) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 10; j++) {
            visited[i][j] = false;
        }
    }
    return;
}

/*
void search_words_in_matrix(char** matrix, int rows, int cols, int i, int j, char* word, int index) {

    printf("%s\n", word);
    if (i >= rows || j >= cols || i < 0 || j < 0 || visited[i][j] == true)
        return;
    
    if (index > 19) {
        memset(word, 0, sizeof(word));
        initVisited();
        search_words_in_matrix(matrix, rows, cols, i, j, word, 0);
        return;
    }

    char c = matrix[i][j];


    word[index] = c;
    if (strlen(word) > 2) {
        if ((isVowel(word[index]) && isVowel(word[index - 1]) && isVowel(word[index - 2])) || (!isVowel(word[index]) && !isVowel(word[index - 1]) && !isVowel(word[index - 2]))) {
            memset(word, 0, sizeof(word));
            initVisited();
            search_words_in_matrix(matrix, rows, cols, i, j, word, 0);
        }

        if (get(table, word)) {
            printf("%s bulundu.\n", word);
        }
    }
    visited[i][j] = true;

    search_words_in_matrix(matrix, rows, cols, i + 1, j, word, index + 1);
    search_words_in_matrix(matrix, rows, cols, i, j + 1, word, index + 1);
    search_words_in_matrix(matrix, rows, cols, i - 1, j, word, index + 1);
    search_words_in_matrix(matrix, rows, cols, i, j - 1, word, index + 1);
    

    if (i >= rows || j >= cols || i < 0 || j < 0) {
        printf("Ulasilan max kelime %s sifirlandi.\n", word);
        memset(word, 0, sizeof(word));
        initVisited();
        return;
    }

    return;
}
*/

bool is_valid_word(char* word) {
    // Add your own validation logic here
    return strlen(word) > 1 && strlen(word) <= 19;
}

void find_words_util(char** matrix, int row, int col, char* word, bool** visited) {
    
    

    visited[row][col] = true;
    if(visited[row][col] = true)
        strncat_s(word, MAX_WORD_LENGTH, &matrix[row][col], 1);

    //if (is_valid_word(word)) {
    //    printf("%s\n", word);
    //}

    //printf("%d\n",get(table, word));

    if (get(table, word))
        printf("%s found from thread:%d\n", word, omp_get_thread_num());

    int row_offset[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int col_offset[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int i, new_row, new_col;

#pragma omp parallel for shared(matrix, visited) private(i, new_row, new_col, word)
    for (i = 0; i < 8; i++) {
        new_row = row + row_offset[i];
        new_col = col + col_offset[i];
        if (new_row >= 0 && new_row < 8 && new_col >= 0 && new_col < 10 &&
            !visited[new_row][new_col] && strlen(word) < 10) {
            find_words_util(matrix, new_row, new_col, word, visited);
        }
    }

    visited[row][col] = false;
    word[strlen(word) - 1] = '\0';   
}

void find_words(char** matrix, bool** visited) {
    initVisited(visited);
    char word[MAX_WORD_LENGTH] = "";
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 10; col++) {
            find_words_util(matrix, row, col, word, visited);
        }
    }
}

void findStrings(char** matrix, int row, int col, int len) {
    if (len == MAX_LEN) {
        return;
    }
    visited[row][col] = 1;
    string[len] = matrix[row][col];
    string[len + 1] = '\0';

    if (strlen(string)<5 && get(table, string))
        printf("%s found\n", string);
    //printf("%s\n", string);
    int i, j;
    for (i = row - 1; i <= row + 1; i++) {
        for (j = col - 1; j <= col + 1; j++) {
            if (i >= 0 && i < ROWS && j >= 0 && j < COLS && !visited[i][j]) {
                findStrings(matrix, i, j, len + 1);
            }
        }
    }
    visited[row][col] = 0;
}