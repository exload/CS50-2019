from sys import argv, exit
from cs50 import get_string


def param_to_key(k):
    # Calculate key as integer from char representation
    k = ord(k)
    if (k >= 65 and k <= 90):
        k = k % 65
    if (k >= 97 and k <= 122):
        k = k % 97
    return k


def encrypt(c, k):
    # Shift only when 'c' is alphabetic
    c = ord(c)
    if (c >= 65 and c <= 90):
        if ((c + k) > 90):
            c = c + k - 26
        else:
            c = c + k
    if (c >= 97 and c <= 122):
        if ((c + k) > 122):
            c = c + k - 26
        else:
            c = c + k
    c = chr(c)
    return c


if len(argv) != 2:
    print("Usage: python vigenere.py keyword")
    exit(1)

if not argv[1].isalpha():
    print("Usage: python vigenere.py keyword")
    exit(1)

param_len = len(argv[1])

text = get_string("plaintext: ")

# Iterator for keyword
j = 0
print("ciphertext: ", end="")
for c in text:
    x = ord(c)
    if ((x >= 65 and x <= 90) or (x >= 97 and x <= 122)):
        k = param_to_key(argv[1][j])
        print(encrypt(c, k), end="")
        if ((j + 1) == param_len):
            j = 0
        else:
            j += 1
    else:
        print(c, end="")
print()