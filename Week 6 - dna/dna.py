import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: dna.py database.csv sequence.txt")
        return 1

    # open database csv
    with open(sys.argv[1]) as dnaDatabase:
        reader = csv.DictReader(dnaDatabase)
        people = []

        # store each line as a dictionary in a list
        for line in reader:
            people.append(line)

    # open sequence txt and store sequence as a string
    with open(sys.argv[2]) as dnaSequence:
        sequence = dnaSequence.read()

    # Find longest match of each STR in DNA sequence
    counts = {}
    count = 0

    # generates a list of keys from people dictionary
    # keys[0] will be 'name', the rest of the elements should be STR sequences
    keys = list(people[0].keys())

    # populates counts{} with info from longest_match
    for i in range(len(keys)):
        if i > 0:
            count = longest_match(sequence, keys[i])
            counts[keys[i]] = count

    # compare keys from each item in people to the keys in counts
    for item in people:
        isMatch = True
        for i in range(len(keys) - 1):
            if int(item[keys[i+1]]) != counts[keys[i+1]]:
                isMatch = False

        if isMatch:
            print(item['name'])
            return

    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
