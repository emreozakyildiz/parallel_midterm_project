#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 49222

struct node {
    char key[50];
    int value;
    struct node* next;
};

int hash(const char* str) {
    //printf("Hashed %s: ", str);
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    //printf("%d\n", hash % TABLE_SIZE);
    return hash % TABLE_SIZE;
}

void insert(struct node** table, const char* key, int value) {
    int index = hash(key);
    struct node* current = table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return;
        }
        current = current->next;
    }
    struct node* new_node = malloc(sizeof(struct node));
    if (new_node == NULL) {
        printf("Error: Unable to allocate memory\n");
        exit(1);
    }
    if (strcpy_s(new_node->key, sizeof(new_node->key), key) != 0) {
        printf("Error: Unable to copy string\n");
        exit(1);
    }
    new_node->value = value;
    new_node->next = table[index];
    table[index] = new_node;
}

int get(struct node** table, const char* key) {
    if (key == NULL || key == '\0')
        return 0;
    int index = hash(key);
    struct node* current = table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return 0; // Not found
}