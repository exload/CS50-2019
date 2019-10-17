#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <cs50.h>
#define _XOPEN_SOURCE above
#include <unistd.h>
#include <crypt.h>

int brute_hash(char *pass, int depth, int max_depth, char *hash, char *salt);
int avail_char(int dec);

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        // Calc the lenght of parameter
        int hash_len = strlen(argv[1]);
        // Check for valid parameter
        if (hash_len != 13)
        {
            printf("Invalid parameter lenght. Usage: ./crack hash\n");
            return 2;
        }
        // Create 'salt'
        char salt[3] = {argv[1][0], argv[1][1], '\0'};
        printf("salt - %s ; ", salt);
		
        // Create array for storing password
        char pass[6];
        // Filling with NULL
        for (int i = 0; i <= 6; i++)
        {
            pass[i] = '\0';
        }
        printf("pass - '%s' ; param  - %s\n", pass, argv[1]);
		
        // Brute force
        if (brute_hash(pass, 0, 4, argv[1], salt))
        {
            return 0;
        }
    }
    else
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
}

// Depth & max_depth starts with 0
int brute_hash(char *pass, int depth, int max_depth, char *hash, char *salt)
{
    for (int j = 0; j < 52; j++)
    {
        pass[depth] = avail_char(j);
		
        if (!strcmp(hash, crypt(pass, salt)))
        {
            printf("%s\n", pass);
            return 1;
        }
		
        if (depth < max_depth)
        {
            if (brute_hash(pass, (depth + 1), max_depth, hash, salt))
            {
                return 1;
            }
        }
		
        if ((j + 1) == 52)
        {
            pass[depth] = '\0';
            //printf("=============max_depth - %i ; depth - %i ; mod string  = %s\n", max_depth, depth, pass);
        }
    }
    return 0;
}

// Return char based on decimal
int avail_char(int dec)
{
    if (dec < 26)
    {
        dec += 65;
    }
    else
    {
        dec += 71;
    }
    return dec;
}
