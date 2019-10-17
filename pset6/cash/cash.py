from cs50 import get_float


def calc(value, base, i):
    change = 0
    remainder = 0
    x = value // base[i]
    if x > 0:
        change += x
    remainder = value % base[i]
    if remainder > 0:
        i += 1
        change += calc(remainder, base, i)
    return change


while True:
    money = get_float("Change owned: ")
    if money > 0:
        break
base = [25, 10, 5, 1]
i = 0
value = money * 100
change = calc(value, base, i)
# Explicit type convertion float to int
change = int(change)
print(change)