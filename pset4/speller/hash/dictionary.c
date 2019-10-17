// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "dictionary.h"

#include <stdlib.h>
#include <string.h>

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

unsigned int dict_size = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Create pointer vault for node*
    // node *new_node = NULL;

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // TODO
        // Allocating memory for the new node
        node *new_node = malloc(sizeof(node));
        // Validate memory allocation
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        // Filling data in new node
        strcpy(new_node->word, word);
        new_node->next = NULL;
        // Calculate dictionary size
        dict_size++;
        // Calcalate hash of the word
        int h = hash(word);

        if (hashtable[h] == NULL)
        {
            hashtable[h] = new_node;
        }
        else
        {
            new_node->next = hashtable[h];
            hashtable[h] = new_node;
        }
        new_node = NULL;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return dict_size;
}
// Different name for variable because exist in next function, error may appear: 'declaration shadows a variable in the global scope'
char lo_word[LENGTH + 1];

bool check_linked_list(node *ptr, char *low_word)
{
    if (strcmp(ptr->word, low_word) == 0)
    {
        return true;
    }
    else
    {
        if (ptr->next == NULL)
        {
            return false;
        }
        else
        {
            return check_linked_list(ptr->next, low_word);
        }
    }

    // Bogus return
    //return false;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    int h = hash(word);
    // Convert to lower case
    int i = 0;
    while (1)
    {
        lo_word[i] = tolower(word[i]);
        if (word[i] == '\0')
        {
            break;
        }
        i++;
    }
    return check_linked_list(hashtable[h], lo_word);
}

void free_linked_list(node *ptr)
{
    if (ptr->next != NULL)
    {
        free_linked_list(ptr->next);
    }
    // Debug valgrind
    // printf("unload - %s\n", ptr->word);
    ptr->next = NULL;
    free(ptr);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        if (hashtable[i] != NULL)
        {
            free_linked_list(hashtable[i]);
            hashtable[i] = NULL;
        }
    }
    // Indicate success
    return true;
}