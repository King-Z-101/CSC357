#include "utility.h"

// Initialize the OrderedList
void initOrderedList(OrderedList* list) {
    list->head = (Node*)malloc(sizeof(Node));
    //change to 0?
    list->head->item = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    list->head->item->character = 0;
    list->head->item->count = 0;    
    list->head->next = list->head;
    list->head->prev = list->head;
}

void cnt_freq(int fileIn, int* histogram) {

    unsigned char c;
    int letter;
    while (1) 
    {
        letter = read(fileIn, &c, sizeof(char));
        if (letter == -1) {
            perror("Error occurred in reading of file in cnt_freq()");
            exit(1);
        }
        if (letter == 0) {
            break; // End of file
        }
        histogram[(int) c]++;
    }
}

// Function to add an item to the OrderedList
int add(OrderedList* list, HuffmanNode* item) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->item = item;

    //If the linked list is empty
    if (list->head->next == list->head) {
        list->head->next = newNode;
        list->head->prev = newNode;
        newNode->prev = list->head;
        newNode->next = list->head;
    } else { 
        Node* currentNode = list->head->next;
        Node* prevNode = list->head;
        
        while (currentNode != list->head)
        {
            //check frequency first then character
            if (newNode->item->count > currentNode->item->count) {
                currentNode = currentNode->next;
                prevNode = currentNode->prev;
                continue;
            } else if (newNode->item->count < currentNode->item->count) {
                newNode->next = currentNode;
                newNode->prev = prevNode;
                prevNode->next = newNode;
                currentNode->prev = newNode;
                // free(newNode);
                // free(currentNode);
                // free(prevNode);
                break;
            } 
            else if (newNode->item->count == currentNode->item->count )
            {
                //check character
                if (newNode->item->character > currentNode->item->character) {
                    currentNode = currentNode->next;
                    prevNode = currentNode->prev;
                    // free(newNode);
                    // free(currentNode);
                    // free(prevNode);
                    continue;
                }
                else if (newNode->item->character <= 
                currentNode->item->character)
                {
                    newNode->next = currentNode;
                    newNode->prev = prevNode;
                    prevNode->next = newNode;
                    currentNode->prev = newNode;
                    // free(newNode);
                    // free(currentNode);
                    // free(prevNode);
                    break;
                }
                
            }
        }
        if (currentNode == list->head) {
            newNode->next = list->head;
            newNode->prev = prevNode;
            prevNode->next = newNode;
            list->head->prev = newNode;
        }
        
    }
    return 0;
}

// Function to remove and return an item at a given index
HuffmanNode* pop(OrderedList* list) {
    HuffmanNode* popNode = list->head->next->item;
    list->head->next = list->head->next->next;
    return popNode;
}

HuffmanNode* create_huff_tree(int* list_of_freqs) {
    OrderedList huff_list;
    initOrderedList(&huff_list);
    //printf("hufflist good\n");
    HuffmanNode* root_node = NULL;
    int i;

    for (i = 0; i < MAXSIZE; i++) {
        if (list_of_freqs[i] >= 1) {
            HuffmanNode* new_node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
            new_node->character = i;
            new_node->count = list_of_freqs[i];
            add(&huff_list, new_node);
        }
    }

    while (huff_list.head->next->next != huff_list.head) {
        HuffmanNode* left_child = pop(&huff_list);
        HuffmanNode* right_child = pop(&huff_list);
        int new_parent_char = '\0';
        HuffmanNode* new_parent = (HuffmanNode*)malloc(sizeof(HuffmanNode));
        new_parent->character = new_parent_char;
        new_parent->count = left_child->count + right_child->count;
        new_parent->left = left_child;
        new_parent->right = right_child;
        add(&huff_list, new_parent);
        //free(new_parent);
    }

    if (huff_list.head->next == huff_list.head) {
        root_node = NULL;
    } else {
        root_node = (HuffmanNode*)huff_list.head->next->item;
    }

    return root_node;
}

// Function to create Huffman codes
void create_code_helper(HuffmanNode* leafnode, char* huffman_code, 
char** huffman_code_list) 
{
    if (leafnode->left == NULL && leafnode->right == NULL) {
        strcpy(huffman_code_list[(int)(leafnode->character)], huffman_code);
    } else {
        char* left_huffman_code = (char*)malloc(strlen(huffman_code) + 2);
        char* right_huffman_code = (char*)malloc(strlen(huffman_code) + 2);

        strcpy(left_huffman_code, huffman_code);
        strcat(left_huffman_code, "0");
        create_code_helper(leafnode->left, left_huffman_code, 
        huffman_code_list);
        
        strcpy(right_huffman_code, huffman_code);
        strcat(right_huffman_code, "1");
        create_code_helper(leafnode->right, right_huffman_code, 
        huffman_code_list);

        free(left_huffman_code);
        free(right_huffman_code);
    }
}

// Function to create Huffman codes
char** create_code(HuffmanNode* root_node) {
    char** huffman_code_list = (char**)malloc(MAXSIZE * sizeof(char*));
    int i;
    for (i = 0; i < MAXSIZE; i++) {
        huffman_code_list[i] = (char*)malloc(MAXSIZE * sizeof(char));
    }
    char* huffman_code = (char*)malloc(MAXSIZE * sizeof(char));
    create_code_helper(root_node, huffman_code, huffman_code_list);
    free(huffman_code);
    return huffman_code_list;
}
