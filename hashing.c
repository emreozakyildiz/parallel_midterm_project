#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 20
#define DICTIONARY_SIZE 49253 // a prime number larger than the number of words

// node structure for the hash table
typedef struct hash_node {
    char word[MAX_WORD_LENGTH + 1];
    struct hash_node* next;
} hash_node;

// hash function for strings
int hash_string(const char* str, int table_size) {
    int hash_val = 0;
    for (; *str != '\0'; str++)
        hash_val = 31 * hash_val + *str;
    return hash_val % table_size;
}

// insert a word into the hash table
void insert_word(hash_node* table[], const char* word, int table_size) {
    int index = hash_string(word, table_size);
    hash_node* new_node = malloc(sizeof(hash_node));
    strncpy_s(new_node->word, MAX_WORD_LENGTH, word, MAX_WORD_LENGTH - 1);
    new_node->next = table[index];
    table[index] = new_node;
}

// check if a word exists in the hash table
int is_word_in_hash(hash_node* table[], const char* word, int table_size) {
    int index = hash_string(word, table_size);
    hash_node* node = table[index];
    while (node != NULL) {
        if (strcmp(node->word, word) == 0)
            return 1;
        node = node->next;
    }
    return 0;
}

// read words from a file and insert them into the hash table
void load_dictionary(hash_node* table[], const char* filename, int table_size) {
    FILE* fp;
    char word[MAX_WORD_LENGTH + 1];

    fopen_s(&fp, filename, "r");
    if (fp == NULL) {
        printf("Error: could not open dictionary file %s", filename);
        exit(1);
    }

    while (fscanf_s(fp, "%s", word, MAX_WORD_LENGTH) != EOF) {
        insert_word(table, word, table_size);
    }

    fclose(fp);
}
/*
int main() {
    hash_node* table[DICTIONARY_SIZE] = { NULL };
    load_dictionary(table, "sozluk.txt", DICTIONARY_SIZE);

    // test the hash table
    printf("%s\n", is_word_in_hash(table, "hello", DICTIONARY_SIZE) ? "Found" : "Not found");
    printf("%s\n", is_word_in_hash(table, "world", DICTIONARY_SIZE) ? "Found" : "Not found");

    return 0;
}
*/