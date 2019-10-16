#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height = 0;
    do
    {
        height = get_int("Enter height (integer) for building the Mario pyramid. Valid values from 1 to 8:");
    }
    while (height < 1 || height > 8);
    for (int row = 1; row <= height; row++)
    {
        int i = 0;
        // Print spaces in a row
        while (i < (height - row))
        {
            printf(" ");
            i++;
        }
        // Print hashes till end
        while (i < height)
        {
            printf("#");
            i++;
        }
        printf("\n");
    }
}
