# prompt user for height of pyramid
print("Height: ", end="")

# continue prompting user until valid input is recieved
while True:
    try:
        height = int(input())

        if height <= 0 or height > 8:
            print("Height: ", end="")
        else:
            break
    except ValueError:
        print("Height: ", end="")

# number of spaces to print on each line before the hash signs start
spaces = height - 1

# for each row in the pyramid
for i in range(height):

    # print the left half
    for j in range(height):
        if j < spaces:
            print(" ", end="")
        else:
            print("#", end="")

    # add two spaces in between halves
    print("  ", end="")

    # print the right half
    for j in range(height - spaces):
        print("#", end="")

    # decrement spaces and start a new line
    spaces -= 1
    print()

