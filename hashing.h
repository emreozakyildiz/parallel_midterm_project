#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1000
#define WORD_SIZE 25

struct node {
    char key[50];
    struct node* next;
};

struct node* table[TABLE_SIZE];

//djb2 Algorithm by Daniel J. Bernstein
int hash(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLE_SIZE;
}

void init_table(void) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = NULL;
    }
}

void insert(struct node** table, const char* key) {
    int index = hash(key);
    struct node* current = table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return;
        }
        current = current->next;
    }
    struct node* new_node = malloc(sizeof(struct node));
    if (new_node == NULL) {
        printf("Error: Unable to allocate memory\n");
        return;
    }
    if (strcpy_s(new_node->key, sizeof(new_node->key), key) != 0) {
        printf("Error: Unable to copy string\n");
        return;
    }
    new_node->next = table[index];
    table[index] = new_node;
}

void load(char* filename) {
    FILE* fp;
    if (fopen_s(&fp, filename, "r") != 0) {
        printf("Error: Unable to open file\n");
        return 1;
    }

    char word[WORD_SIZE];

    while (fgets(word, sizeof(word), fp) != NULL) {
        word[strcspn(word, "\n")] = '\0';
        insert(table, word, 1);
    }

    fclose(fp);
}

bool get(struct node** table, const char* key) {
    if (key == NULL || key == '\0')
        return 0;
    int index = hash(key);
    struct node* current = table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}