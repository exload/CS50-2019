#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int shift(int c, int k);
int param_to_key(int k);

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        // Calc the lenght of parameter
        int param_len = strlen(argv[1]);
        // Check for valid parameter
        for (int i = 0; i < param_len; i++)
        {
            if (!isalpha(argv[1][i]))
            {
                printf("Usage: ./vigenere keyword\n");
                return 1;
            }
        }
        // Request for string
        string text = get_string("plaintext: ");
        // Calc the lenght of string
        int len = strlen(text);
        int ciphertext[len];
        // Default key value
        int k = 0;
        // Iterator for text[i]
        int i = 0;
        // Iterator for argv[1][j]
        int j = 0;
        for (; i < len; i++)
        {
            if ((text[i] >= 65 && text[i] <= 90) || (text[i] >= 97 && text[i] <= 122))
            {
                k = param_to_key(argv[1][j]);
                // For debug purpose
                //printf("argv = %c; k = %i\n", argv[1][j], k);
                // For call shift function
                ciphertext[i] = shift(text[i], k);
                if ((j + 1) == param_len)
                {
                    j = 0;
                }
                else
                {
                    j++;
                }
            }
            else
            {
                ciphertext[i] = text[i];
            }
        }
        // Print ciphertext
        printf("ciphertext: ");
        for (i = 0; i < len; i++)
        {
            printf("%c", ciphertext[i]);
        }
        printf("\n");
    }
    else
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
}
// Calculate key as integer from char representation
int param_to_key(int k)
{
    if (k >= 65 && k <= 90)
    {
        k = k % 65;
    }
    if (k >= 97 && k <= 122)
    {
        k = k % 97;
    }
    return k;
}

// Shift only when 'c' is alphabetic
int shift(int c, int k)
{
    if (c >= 65 && c <= 90)
    {
        if ((c + k) > 90)
        {
            c = c + k - 26;
        }
        else
        {
            c = c + k;
        }
    }
    if (c >= 97 && c <= 122)
    {
        if ((c + k) > 122)
        {
            c = c + k - 26;
        }
        else
        {
            c = c + k;
        }
    }
    return c;
}
