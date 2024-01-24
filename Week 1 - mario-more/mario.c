#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int pyramidHeight;
    int numOfBlocks = 1;    //start at 1, because there's always exactly one block on each side of the top level of the pyramid


    //get height from user input
    do
    {
        pyramidHeight = get_int("Height: ");
    }
    while (pyramidHeight < 1 || pyramidHeight > 8);


    //print the pyramid
    for (int i = 0; i < pyramidHeight; i++)
    {
        //print left side of row
        for (int j = 0; j < pyramidHeight; j++)
        {
            if (j >= pyramidHeight - numOfBlocks)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }


        //add spaces in the middle
        printf("  ");


        //print right side of row
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        //increment numOfBlocks for next line, then start a new line
        numOfBlocks++;
        printf("\n");
    }
}