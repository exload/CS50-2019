from sys import argv, exit
from itertools import product
from crypt import crypt
from string import ascii_letters


def convertTuple(tup):
    word = ''.join(tup)
    return word


if len(argv) != 2:
    print("Usage: python crack.py hash")
    exit(1)

if (len(argv[1]) != 13):
    print("Invalid parameter lenght. Usage: python crack.py hash")
    exit(1)

salt = argv[1][:2]
for i in range(1, 6, 1):
    posible_words = product(ascii_letters, repeat=i)
    for item in posible_words:
        word = convertTuple(item)
        x = crypt(word, salt)
        if x == argv[1]:
            print(word)
            exit(0)