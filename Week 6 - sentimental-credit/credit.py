import re


def main():
    # prompt user for number
    print('Number: ', end='')

    # AMEX: starts with 34 or 37, must be 15 digits long
    # MASTER: starts with 51-55, must be 16 digits long
    # VISA: starts with 4, must be 13 or 16 digits long

    # get number from user input
    ccNumber = input()

    # using some regex patterns to verify input
    if re.match(r'34\d{13}', ccNumber) or re.match(r'37\d{13}', ccNumber):
        cardType = 'AMEX'
    elif re.match(r'5[1-5]\d{14}', ccNumber):
        cardType = 'MASTERCARD'
    elif re.match(r'4\d{12}', ccNumber) or re.match(r'4\d{15}', ccNumber):
        cardType = 'VISA'
    else:
        cardType = 'INVALID'

    if cardType != 'INVALID' and luhn(ccNumber) != 0:
        cardType = 'INVALID'

    print(cardType)


def luhn(num):
    # remove any trailing white space from end of string
    num = num.rstrip()

    # initialize a variable to hold the sum
    result = 0

    # keep repeating this process until the string is empty
    while num != '':
        # add the very last digit to the sum
        result += int(num[len(num) - 1])

        # grab the second to last digit and multiply it by 2
        if len(num) > 1:
            temp = int(num[len(num) - 2]) * 2

            # if temp is a single digit, add it to result
            if temp <= 9:
                result += temp
            # if temp is two digits, add each digit to result individually
            else:
                result += 1 + (temp % 10)

        num = num[:len(num) - 2]

    return result % 10


main()
