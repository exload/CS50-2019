from cs50 import get_int

height = 0
while True:
    height = get_int("Height: ")
    if height > 0 and height < 9:
        break
for row in range(1, (height + 1), 1):
    # Print spaces in a row
    print(" " * (height - row), end='')
    # Print hashes till end
    print("#" * row, end='')
    print()