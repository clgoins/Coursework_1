#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int main(void)
{
    int grade = 0;
    double index = 0.0;
    string text = "";
    int letters = 0;
    int words = 1;
    int sentences = 0;
    float lettersPerHundred;
    float sentencesPerHundred;


    //TODO: get string from user input
    text = get_string("Text: ");

    //analyze text for letters, words, and sentences
    for (int i = 0; i < strlen(text); i++)
    {
        //count letters in text
        if (isalpha(text[i]))
        {
            letters++;
        }

        //count words in text
        if (text[i] == ' ')
        {
            words++;
        }

        //count sentences in text
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }

    //get letters per 100 words
    lettersPerHundred = (float) letters / (float) words * 100;

    //get sentences per 100 words
    sentencesPerHundred = (float) sentences / (float) words * 100;

    //calculate coleman-liau index
    index = 0.0588 * lettersPerHundred - 0.296 * sentencesPerHundred - 15.8;

    //DEBUG
    /*printf("Letters: %i\n",letters);
    printf("Words: %i\n",words);
    printf("Sentences: %i\n",sentences);
    printf("LpH: %f\n",lettersPerHundred);
    printf("SpH: %f\n",sentencesPerHundred);
    printf("Grade: %f\n",grade);*/

    //round grade to nearest integer
    grade = round(index);

    //display output
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) grade);
    }

    return 0;
}