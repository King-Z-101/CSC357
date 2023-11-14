#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXSIZE 256
#define BYTE_SIZE 8

// Define a structure for HuffmanNode
typedef struct HuffmanNode {
    int character;
    int count;
    struct HuffmanNode* left;
    struct HuffmanNode* right;
} HuffmanNode;

// Define a structure for the Node
typedef struct Node {
    HuffmanNode* item;
    struct Node* next;
    struct Node* prev;
} Node;

// Define a structure for the OrderedList
typedef struct OrderedList {
    Node* head;
} OrderedList;

void initOrderedList(OrderedList* list);

void cnt_freq(int fileIn, int* char_counts);

int add(OrderedList* list, HuffmanNode* item);

HuffmanNode* pop(OrderedList* list);

HuffmanNode* create_huff_tree(int* list_of_freqs);

void create_code_helper(HuffmanNode* leafnode, char* huffman_code, 
char** huffman_code_list); 

char** create_code(HuffmanNode* root_node);

#endif
