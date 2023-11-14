/* Program that decodes a given input file and returns into stdout 
of an out file. header file contains struct definitions and methods used 
between hencode and hdecode*/

#include "utility.h"

void hdecode(int inFD, int outFD);

int main(int argc, char* argv[]) {
    //check input file was passed
    int inFD;
    //check if the first argument is passed and if not then read from stdIN
    if ((argc < 2) | (*argv[1] == '-'))
    {
        inFD = STDIN_FILENO;
        // perror("Pass in an input file!");
        // exit(1);
    }
    else
    {
        inFD = open(argv[1], O_RDONLY);
    }

    if (inFD == -1)
    {
        perror("File not found!");
        exit(1);
    }

    //Only create output file if in the command prompt, 
    //else outFile will be Stdout
    int outFD = STDOUT_FILENO;
    if (argc > 2)
    {
        outFD = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outFD == -1) 
        {
            perror("Error opening outFile!");
            exit(1);
        }
    }

    //decode time!!
    hdecode(inFD, outFD);
    return 0;

}

void hdecode(int inFD, int outFD)
{
    int letter;
    //build historgram from encoded file
    int histogram[MAXSIZE] = {0};
    unsigned char byteBuffer = 0;
    unsigned int count = 0;
    //success
    if ((letter = read(inFD, &byteBuffer, sizeof(byteBuffer))) < 0)
    {
        perror("Error reading inFD!");
        exit(1);
        //return 1;
    }
    //printf("%d", letter);
    //first byte of header tells u how many unique letters there are
    int uniqCnt = (int)(byteBuffer);
    int i;
    //int letter;
    //variable for counting character frequency
    int letterCnt = 0;
    int totalCnt = 0;

    //build histogram
    for (i = 0; i < uniqCnt + 1; i++)
    {
        //process: char = 1 byte; count = 4 bytes = 5 bytes read each run
        if ((read(inFD, &byteBuffer, sizeof(char))) < 0)
        {
            perror("Error reading character from header!");
            exit(1);
        }
        //printf("%d", letter);
        letter = (int)(byteBuffer);

        if ((read(inFD, &count, sizeof(int))) == -1)
        {
            perror("Error reading character count from header!");
            exit(1);
        }
        //printf("%d", letter);
        letterCnt = (int)htonl(count);
        //add to histogram
        histogram[letter] = letterCnt;
        totalCnt += letterCnt;
    }
    //printf("%d", letter);

    //use histogram to rebuild tree
    HuffmanNode* root = create_huff_tree(histogram);
    HuffmanNode* current = root;
    int bit;
    // unsigned char *byte = (unsigned char*)malloc(sizeof(unsigned char));
    unsigned char byte = 0;
    //used for debug purposes
    int x;

    while ((x = read(inFD, &byte, sizeof(unsigned char))) > 0)
    {
        i = 7;
        while( i >= 0)
        {
            //base case
            if (current->left == NULL && current->right == NULL)
            {
                // We found a leaf node, write the character to the output file.
                write(outFD, &(current->character), sizeof(char));
                current = root; // Reset the current node to the root.
                totalCnt = totalCnt - 1;
                if (totalCnt == 0)
                {
                    break;
                    //return 0;
                }
                
            }
    
            bit = (byte >> i) & 1; 
            if (bit == 0)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
            i--;
            //check one more time when nearing end to get last letter
            if (current->left == NULL && current->right == NULL)
            {
                // We found a leaf node, write the character to the output file.
                write(outFD, &(current->character), sizeof(char));
                current = root; // Reset the current node to the root.
                totalCnt = totalCnt - 1;
                if (totalCnt == 0)
                {
                    break;
                    //return 0;
                }
                
            }
        }

        if (totalCnt == 0)
        {
            break;
        }
    }

    //return 0;
}
