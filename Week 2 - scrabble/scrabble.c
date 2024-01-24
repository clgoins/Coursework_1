#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    //if score one is higher, player 1 wins
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    //if score 2 is higher, player 2 wins
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    //if scores are equal, it's a tie
    else
    {
        printf("Tie!\n");
    }
}



//computes scrabble score of a given string
int compute_score(string word)
{
    //initialize score to 0
    int score = 0;
    char c;

    for (int i = 0; i < strlen(word); i++)
    {
        //if a character is in the alphabet, capitalize it and get its score
        if (isalpha(word[i]) > 0)
        {
            c = toupper(word[i]);
            //POINTS[] starts at 0; ASCII capital alphabet starts at 65.
            //subtract 65 to convert from ASCII table to POINTS[] array
            score += POINTS[c - 65];
        }
    }

    return score;
}
