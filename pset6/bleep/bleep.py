from cs50 import get_string
from sys import argv, exit


def main():
    if len(argv) != 2:
        print("Usage: python bleep.py dictionary")
        exit(1)
    # Prompt the user to provide a message
    print("What message would you like to censor?")
    text = get_string()

    # Open file
    dict_file = open(argv[1], 'r')
    dictionary = set()
    for line in dict_file:
        dictionary.add(line.strip())
    dict_file.close()

    # print("------------")
    # print(dictionary)
    # print("------------")
    # Entered text modification
    splited_text = text.split(' ')
    for word in splited_text:
        # print(word)
        # print("++++++")
        if word.lower() in dictionary:
            x = len(word)
            print("*" * x, end="")
        else:
            print(word, end="")
        print(" ", end="")
    print()


if __name__ == "__main__":
    main()