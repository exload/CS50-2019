from sys import argv, exit
from cs50 import get_string


def encrypt(c, k):
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
    print("Usage: python caesar.py key")
    exit(1)
k = int(argv[1])
# print(k)
if k < 1:
    print("Usage: python caesar.py key")
    exit(1)
k = k % 26
text = get_string("plaintext: ")
# print(text)
print("ciphertext: ", end="")
for c in text:
    print(encrypt(c, k), end="")
print()