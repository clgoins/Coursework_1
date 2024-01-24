#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    //must be exactly 2 arguments provided
    if (argc != 2)
    {
        printf("Usage: ./recover input.raw\n");
        return 1;
    }

    //open the input file given by command line argument
    FILE *card = fopen(argv[1], "r");

    //make sure the provided input file is able to be opened
    if (card == NULL)
    {
        printf("Input file could not be opened");
        fclose(card);
        return 1;
    }

    //set up some variables for reading and writing

    int blockSize = 512;            //blocksize is the number of bytes in a block on a FAT system
    uint8_t block[blockSize];       //an array for storing the block of data to be read
    int fileNumber = 0;             //counter for the number of output files we've processed (used for generating file names)
    char outFileName[8];            //file name for the current output file
    FILE *output = NULL;            //pointer to current output file


    //read the first block of the input file before starting the loop
    fread(block, 1, blockSize, card);

    do
    {

        //IF the first 4 bytes of a block match a .jpg header:
        //close the currently open file (if any)
        //generate the new file name and open it for writing
        //increment fileNumber for the next file name when we get back to this block
        if (block[0] == 0xFF && block[1] == 0xD8 && block[2] == 0xFF && block[3] >= 0xE0 && block[3] <= 0xEF)
        {
            if (output != NULL)
            {
                fclose(output);
            }

            sprintf(outFileName, "%03d.jpg", fileNumber);
            output = fopen(outFileName, "w");
            fileNumber++;
        }


        //make sure a file is open for writing, and copy the contents of our current block there
        if (output != NULL)
        {
            fwrite(block, 1, blockSize, output);
        }

    }
    //check we haven't made it to the end of the input file before repeating the loop
    while (fread(block, 1, blockSize, card) == blockSize);


    //close any data streams we may have opened before exiting
    fclose(card);
    fclose(output);
    return 0;
}