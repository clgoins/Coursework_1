#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //declare some variables
    int sum = 0;
    int tempSum = 0;
    int firstDigit;
    int secondDigit;
    int numberOfDigits = 2;
    //status should default to invalid to save us some lines of code if the number fails any of the checks down the line
    string status = "INVALID";
    bool cardIsValid = false;
    long ccNumber;
    long tempNumber;



    //get cc number from user input
    ccNumber = get_long("Card Number: ");


    //put it in a temporary variable so we can work on it without losing the original card number
    tempNumber = ccNumber;


    //get leading digits && number of digits to determine what type of card this is
    //tempNumber = tempNumber / 10 will remove the last digit
    //is tempNumber is < 100, we know it's down to two digits
    while (tempNumber >= 100)
    {
        tempNumber = tempNumber / 10;
        numberOfDigits++;
    }


    //tempNumber % 10 will return the very last digit in tempNumber.
    //at this point tempNumber should always be either one or two digits long.
    secondDigit = tempNumber % 10;
    tempNumber = tempNumber / 10;
    firstDigit = tempNumber % 10;



    //check if leading digits & number of digits are valid
    if (firstDigit == 3)
    {
        if ((secondDigit == 4 || secondDigit == 7) && numberOfDigits == 15)
        {
            cardIsValid = true;
            status = "AMEX";
        }
    }
    else if (firstDigit == 4)
    {
        if (numberOfDigits == 13 || numberOfDigits == 16)
        {
            cardIsValid = true;
            status = "VISA";
        }
    }
    else if (firstDigit == 5)
    {
        if (secondDigit >= 1 && secondDigit <= 5 && numberOfDigits == 16)
        {
            cardIsValid = true;
            status = "MASTERCARD";
        }
    }


    //get the original card number again
    tempNumber = ccNumber;


    //perform Luhn's algorithm
    while (tempNumber > 0 && cardIsValid)
    {
        //add the last digit to our sum, and get rid of it
        sum += tempNumber % 10;
        tempNumber = tempNumber / 10;

        //get the second to last digit and double it
        tempSum = (tempNumber % 10) * 2;

        //if tempSum is two digits; add those two digits to the total sum individually
        if (tempSum > 9)
        {
            sum += tempSum % 10;
            tempSum = tempSum / 10;
            sum += tempSum;
        }
        else
        {
            sum += tempSum;
        }

        //get rid of the second to last digit, and repeat until there are no digits left in tempNumber
        tempNumber = tempNumber / 10;

        //if this loop runs while there's only one digit left in tempNumber, tempSum will evaluate to 0
    }


    //check if our sum ends in 0
    if (sum % 10 != 0)
    {
        cardIsValid = false;
        status = "INVALID";
    }


    //output the final result
    printf("%s\n", status);

}