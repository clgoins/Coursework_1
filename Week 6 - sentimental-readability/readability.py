# prompt user for input
print('Text: ', end='')
inputText = input()

letterCount = 0
wordCount = 1
sentenceCount = 0

# check every character in inputText, and increment values accordingly
for char in inputText:
    # if character is alphabetic, add it to the letter count
    if char.isalpha():
        letterCount += 1

    # if character is white space, increment the word count
    if char == ' ' or char == '\n':
        wordCount += 1

    # if character is punctuation, increment the sentence count
    if char == '.' or char == '!' or char == '?':
        sentenceCount += 1

# calculate the coleman-liau index
index = 0.0588 * ((letterCount / wordCount) * 100) - 0.296 * ((sentenceCount / wordCount) * 100) - 15.8

# round the index to the nearest int and print the grade level
if round(index) < 1:
    print('Before Grade 1')
elif round(index) >= 16:
    print('Grade 16+')
else:
    print(f'Grade {round(index)}')