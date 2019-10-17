#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>

int shift(int c, int k);

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        // Default key value
        int k = 0;
        // Convert console parameter to integer
        k = atoi(argv[1]);
        if (!(k <= 0))
        {
            // Request for string
            string text = get_string("plaintext: ");
            // Calc the lenght of string
            int len = strlen(text);
            int ciphertext[len];
            k = k % 26;
            for (int i = 0; i < len; i++)
            {
                // For simplicity call shift function
                ciphertext[i] = shift(text[i], k);
            }
            printf("ciphertext: ");
            for (int i = 0; i < len; i++)
            {
                printf("%c", ciphertext[i]);
            }
            printf("\n");
        }
        else
        {
            printf("Usage: ./caesar key\n");
        }
    }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}
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
