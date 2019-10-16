#include <stdio.h>
#include <cs50.h>
#include <math.h>
// Prototipe for self calling function
int calc(int value, int base[], int i);
// This programm is a "greedy Algorithm" for calculate min number of coins needed for exchange
int main(void)
{
    int base[] = {25, 10, 5, 1};
    int i = 0;
    float money = 0;
    do
    {
        money = get_float("Enter how many money you have to exchange. Only 0 and positive numbers accepted: ");
    }
    while (money < 0);
    int value = round(money * 100);
    int change = 0;
    int remaider = 0;
    change = calc(value, base, i);
    printf("%d\n", change);
}
// Calculate integer number of coins using the array value as a basis
int calc(int value, int base[], int i)
{
    int change = 0;
    int remaider = 0;
    int x = value / base[i];
    if (x > 0)
    {
        change += x;
    }
    remaider = value % base[i];
    if (remaider > 0)
    {
        i++;
        change += calc(remaider, base, i);
    }
    return change;
}
