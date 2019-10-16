#include <stdio.h>
#include <cs50.h>
#include <string.h>

// Protopype of function to calculate checksumm
int sum(long rem, int x);

int main(void)
{
    long card = 0;
    long value = 0;
    int rem = 0;
    int check_sum = 0;
    int mult = 0;
    // Represent digital position
    int x = 1;
    bool end = 0;
    string answer = "INVALID";
    do
    {
        card = get_long("Supply credit card number for synthetic check: ");
    }
    while (card <= 0);
    
    while (!end)
    {
        value = card / 10;
        rem = card % 10;
        // Detect last digit
        if (value == 0)
        {
            if (card == 4 && (x == 13 || x == 16))
            {
                answer = "VISA";
                check_sum += sum(rem, x);
                break;
            }
            else if (card == 3 || card == 5)
            {
                check_sum += sum(rem, x);
                break;
            }
            else
            {
                break;
            }
        }
        // Not last digit
        else
        {
            // More then 2 digits left
            if (card > 99)
            {
                check_sum += sum(rem, x);
                card = value;
                x++;
                continue;
            }
            if ((card == 34 || card == 37) && x == 14)
            {
                answer = "AMEX";
                check_sum += sum(rem, x);
                card = value;
                x++;
                continue;
            }
            if ((card >= 51 && card <= 55) && x == 15)
            {
                answer = "MASTERCARD";
                check_sum += sum(rem, x);
                card = value;
                x++;
                continue;
            }
            if ((value == 4 && (x == 12 || x == 15)))
            {
                check_sum += sum(rem, x);
                card = value;
                x++;
                continue;
            }
            else
            {
                break;
            }
        }
    }
    // Check to exit the cycle through the break
    if (strncmp(answer, "INVALID", 1))
    {
        // Validating
        if ((check_sum % 10) != 0)
        {
            answer = "INVALID";
        }
    }
	
    printf("%s\n", answer);
}

// Function calculates checksumm based on 'second-to-last digit'
int sum(long rem, int x)
{
    int check_sum = 0;
    if (x % 2)
    {
        check_sum = rem;
    }
    else
    {
        int mult = rem * 2;
        if (mult > 9)
        {
            check_sum = mult - 9;
        }
        else
        {
            check_sum = mult;
        }
    }
    return check_sum;
}
