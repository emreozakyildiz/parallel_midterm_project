#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include "hashing.h"

#define ROWS 4
#define COLS 4
#define MAX_LEN 10

#define MAX_WORD_LENGTH 20

char matrix[ROWS][COLS];
char string[MAX_LEN + 1];
int visited[ROWS][COLS];

char** get_matrix_from_file(void);
void print_matrix(char **matrix);
void initVisited(bool** visited);
void findStrings(char** matrix, int row, int col, int len);

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
        word[strcspn(word, "\n")] = '\0';
        insert(table, word, 1);
    }
    
    fclose(fp);

    bool** visited = (bool**)malloc(ROWS * sizeof(bool*));
    for (int i = 0; i < 8; i++)
        visited[i] = (bool*)malloc(COLS * sizeof(bool));

    initVisited(visited);


    int m, n;

//#pragma omp parallel for num_threads(4) shared(matrix, table) private(m, n, visited, string)
    for (m = 0; m < ROWS; m++) {
        for (n = 0; n < COLS; n++) {
            int length = 0;
            findStrings(matrix, m, n, length);
        }
    }
    return 0;
}

char** get_matrix_from_file() {
    FILE* fp;
    char* filename = "matrix.ini";
    int rows, cols, i, j;

    fopen_s(&fp, filename, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        return "0";
    }
    else {
        fscanf_s(fp, "%d %d ", &rows, &cols);
        printf("%d,%d boyutunda matris olusturuldu.\n", rows, cols);
    }
    rows = ROWS;
    cols = COLS;

    char** matrix = (char**)malloc(rows * sizeof(char*));
    for (i = 0; i < rows; i++)
        matrix[i] = (char*)malloc(cols * sizeof(char));

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            fscanf_s(fp, "%c ", &matrix[i][j]);
        }
    }

    fclose(fp);
    return matrix;
}

void print_matrix(char** matrix) {
    FILE* fp;
    char* filename = "matrix.ini";
    int rows, cols, i, j;

    fopen_s(&fp, filename, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        return;
    }
    else {
        fscanf_s(fp, "%d %d", &rows, &cols);
    }

    rows = ROWS;
    cols = COLS;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
    fclose(fp);
}

void initVisited(bool** visited) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            visited[i][j] = false;
        }
    }
    return;
}

void findStrings(char** matrix, int row, int col, int len) {
    if (len == MAX_LEN) {
        return;
    }
    visited[row][col] = 1;
    string[len] = matrix[row][col];
    string[len + 1] = '\0';

    if (get(table, string))
        printf("%s found from thread no: %d\n", string, omp_get_thread_num());

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