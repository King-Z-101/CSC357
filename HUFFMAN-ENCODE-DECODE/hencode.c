/* Program that encodes a given input file and returns into stdout 
of an out file. header file contains struct definitions and methods used 
between hencode and hdecode*/

#include "utility.h"

void hencode(int inFile, int outFile, int* histogram, char** hcodes);

int main(int argc, char* argv[]) {
    //check input file was passed
    if (argc < 2)
    {
        perror("Pass in an input file!");
        exit(1);
    }

    int inFD = open(argv[1], O_RDONLY);
    //int outFD = STDOUT_FILENO;

    if (inFD == -1)
    {
        perror("File not found!");
        exit(1);
    }

    // //Check if the inputFile is empty
    // unsigned char firstByteOfinFD = NULL;
    // if (read(inFD, &firstByteOfinFD, 1) == 0)
    // {
    //     return 0;
    // }
    // /*Reset offset to point to start of inFile*/
    // lseek(inFD, 0, SEEK_SET);



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
    //create histogram
    int histogram[MAXSIZE] = {0};
    cnt_freq(inFD, histogram);

    //create huffman tree
    HuffmanNode* huff_tree = create_huff_tree(histogram);

    //create codes from huffman tree
    char** hcodes = create_code(huff_tree);


    /*Reset offset to point to start of inFile*/
    lseek(inFD, 0, SEEK_SET);

    hencode(inFD, outFD, histogram, hcodes);



    return 0;
}

void hencode(int inFile, int outFile, int* histogram, char** hcodes)
{
    //create header's (num - 1) = count characters in inputFile
    unsigned num = 0, indexByte = 0, i;
    for (i = 0; i < MAXSIZE; i++) 
    {
        if (histogram[i] > 0) 
        {
            num++;
        }
    }

    uint8_t headerStart = num - 1;
    write(outFile, &headerStart, sizeof(headerStart));
    indexByte++;

    //write rest of header
    for (i = 0; i < MAXSIZE; i++)
    {
        if (histogram[i] > 0) 
        {
            //write letter's hex value to buffer
            uint8_t charAscii = (uint8_t) i;
            write(outFile, &charAscii, 1);
            uint32_t count = ntohl(histogram[i]);
            write(outFile, &count, 4);
        }
    }

    //write body
    unsigned char letter;
    unsigned char* buffer = (unsigned char*)malloc(MAXSIZE);
    int codeLength;
    char* code = NULL;
    //allocate 8 bits/1 byte to all 0s (00000000)
    unsigned int bitCounter = 0;
    unsigned char byteBuffer = 0;
    //read bytes from inputFile until EOF
    while ((indexByte = read(inFile, buffer, MAXSIZE)) > 0) 
    {
        for (i = 0; i < indexByte; i++) 
        {
            letter = buffer[i];
            code = hcodes[(int)letter];
            codeLength = strlen(code);
            int j;

            for (j = 0; j < codeLength; j++) 
            {
                // Left-shift the current bit of the code into the byteBuffer
                //byteBuffer = byteBuffer | code[j] << 1;
                byteBuffer = (byteBuffer << 1) | (code[j] - '0');
                bitCounter++;

                // Write only after we reach 8 bits written
                if (bitCounter == BYTE_SIZE) 
                {
                    write(outFile, &byteBuffer, 1);
                    bitCounter = 0;
                    byteBuffer = 0;
                }
            }
        }
    }

// padd remaing bit spaces with 0's
    if (bitCounter > 0) 
    {
        while (bitCounter < BYTE_SIZE) 
        {
            byteBuffer = byteBuffer << 1;
            bitCounter++;
        }
        write(outFile, &byteBuffer, 1);
    }
    
}
