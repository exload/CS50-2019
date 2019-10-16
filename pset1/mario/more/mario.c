#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height = 0;
    do
    {
        height = get_int("Enter height (integer) for building the Mario adjacent pyramids. Valid values from 1 to 8: ");
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
        // Save number of hashes/bricks needed to print in a row for adjacent pyramid
        int k = height - i;
        // Print hashes till end
        while (i < height)
        {
            printf("#");
            i++;
        }
        for (int j = 0; j < 2 ; j++)
        {
            printf(" ");
        }
        // Print adjacent pyramid
        while (k > 0)
        {
            printf("#");
            k--;
        }
        printf("\n");
    }
}
