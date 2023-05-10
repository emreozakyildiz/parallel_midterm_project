#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <time.h>
#include "hashing.h"

#define ROWS 4
#define COLS 4
#define MAX_LEN 10

char matrix[ROWS][COLS];
char string[MAX_LEN + 1];
int visited[ROWS][COLS];

char** get_matrix_from_file(char* filename);
void print_matrix(char **matrix);
bool** initVisited();
void findStrings(char** matrix, int row, int col, int len);

int main() {
    setlocale(LC_ALL, "tr_TR");

    char** matrix = get_matrix_from_file("resources/matrix_2.ini");
    print_matrix(matrix);

    init_table();

    load("resources/sozluk.txt");

    bool** visited = initVisited();

    clock_t start = clock();

    int m, n;

#pragma omp parallel for private(m, n, visited, string) shared(matrix, table) num_threads(4)
        for (m = 0; m < ROWS; m++) {
            for (n = 0; n < COLS; n++) {
                //printf("\nresults for matrix[%d][%d]=%c:\n\n", m, n, matrix[m][n]);
                int length = 0;
                findStrings(matrix, m, n, length);
            }
        }
    
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Completion time: %.2f seconds.", time_spent);
    return 0;
}

char** get_matrix_from_file(char* filename) {
    FILE* fp;
    int rows, cols, i, j;

    fopen_s(&fp, filename, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        return "0";
    }
    else {
        fscanf_s(fp, "%d %d ", &rows, &cols);
        rows = ROWS;
        cols = COLS;
        printf("%d,%d boyutunda matris olusturuldu.\n", rows, cols);
    }

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
    char* filename = "resources/matrix.ini";
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

    printf("\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    fclose(fp);
}

bool** initVisited() {
    bool** visited = (bool**)malloc(ROWS * sizeof(bool*));
    for (int i = 0; i < 8; i++)
        visited[i] = (bool*)malloc(COLS * sizeof(bool));

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            visited[i][j] = false;
        }
    }
    return visited;
}

void findStrings(char** matrix, int row, int col, int len) {
    if (len == MAX_LEN) {
        return;
    }
    visited[row][col] = 1;


    string[len] = matrix[row][col];
    string[len + 1] = '\0';
    
    if (get(table, string))
        printf("-> %s found from thread no: %d\n", string, omp_get_thread_num());
    

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