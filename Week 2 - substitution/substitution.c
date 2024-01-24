#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    string plaintext;
    string ciphertext = "";

    //check for the correct number of arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    //check if the 'key' argument is exactly 26 characters long
    if (strlen(argv[1]) != 26)
    {
        printf("Invalid key. Key must contain 26 unique letters.\n");
        return 1;
    }

    //check if the 'key' argument is strictly alphabetic
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (isalpha(argv[1][i]) == 0)
        {
            printf("Invalid key. Key must contain 26 unique letters.\n");
            return 1;
        }
    }

    //check if the 'key' argument contains duplicate characters
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        char c = argv[1][i];

        for (int j = i + 1; j < strlen(argv[1]); j++)
        {
            if (c == argv[1][j])
            {
                printf("Invalid key. Key must contain 26 unique letters.\n");
                return 1;
            }
        }
    }

    //get plain text from user
    plaintext = get_string("plaintext: ");
    ciphertext = plaintext;


    //encrypt the plaintext string
    for (int i = 0; i < strlen(plaintext); i++)
    {
        //check if a plaintext character is uppercase and replace it
        if (isupper(plaintext[i]) > 0)
        {
            //uppercase alphabet starts at 65
            ciphertext[i] = toupper(argv[1][plaintext[i] - 65]);
        }
        //check if a plaintext character is lowercase and replace it
        else if (islower(plaintext[i]) > 0)
        {
            //lowercase alphabet starts at 97
            ciphertext[i] = tolower(argv[1][plaintext[i] - 97]);
        }

        //if the character is neither uppercase, nor lowercase,
        //it must be a number or symbol, so do nothing

    }


    //output the ciphertext string
    printf("ciphertext: %s\n", ciphertext);
    return 0;

}

