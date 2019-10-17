from cs50 import get_string

while True:
    card = get_string("Number: ")
    if card.isdigit():
        break
lenght = len(card)
x = int(card[0])
y = int(card[:2])
even_check = False
summ = 0
vendor = ["VISA", "AMEX", "MASTERCARD"]
selector = 10
# Primary check
if (x == 4 and (lenght == 13 or lenght == 16)):
    Pri_check = True
    selector = 0
elif ((y == 34 or y == 37) and lenght == 15):
    Pri_check = True
    selector = 1
elif ((y in range(51, 56)) and lenght == 16):
    Pri_check = True
    selector = 2
else:
    Pri_check = False
if Pri_check:
    for i in range((lenght - 1), -1, -1):
        x = int(card[i])
        if even_check:
            mult = x * 2
            if mult > 9:
                mult = mult - 9
            summ += mult
        else:
            summ += x
        even_check = not even_check

    if (summ % 10) == 0:
        print(vendor[selector])
    else:
        print("INVALID")
else:
    print("INVALID")