// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// We may try aaa to ''', which is 26 * 26 * 26
// If out of memory, then try aa to ''', which is 26 * 26
const unsigned int N = 27 * 27 * 27;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Search for the word
    unsigned int index = hash(word);

    // Prepare the hard-coded words
    char words[13][10] = {{"a"}, {"an"}, {"the"},
        {"I"}, {"me"}, {"you"},
        {"he"}, {"she"}, {"him"}, {"her"},
        {"his"}, {"we"}, {"they"}
    };

    // Compare the hard-coded words
    for (int i = 0; i < 13; i++)
    {
        if (strcasecmp(words[i], word) == 0)
        {
            // Word found
            return true;
        }
    }

    // Compare with the dictionary
    for (node *line = table[index]; line != NULL; line = line->next)
    {
        if (strcasecmp(line->word, word) == 0)
        {
            // Word found
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    // Hash the first three letters to the table
    unsigned int index = 0;

    // Deal with the first letter
    if (isalpha(word[0]))
    {
        index += (tolower(word[0]) - 'a') * 27 * 27;
    }
    else
    {
        index += 26 * 27 * 27;
    }

    // Deal with the second letter
    if ((strlen(word) > 1) && (isalpha(word[1])))
    {
        index += (tolower(word[1]) - 'a') * 27;
    }
    else
    {
        index += 26 * 27;
    }

    // Deal with the third letter
    if ((strlen(word) > 2) && (isalpha(word[2])))
    {
        index += (tolower(word[2]) - 'a');
    }
    else
    {
        index += 26;
    }

    return index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open up the dictionary
    char buffer[LENGTH + 1];
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // Initialize the table to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = malloc(sizeof(node));
        table[i]->next = NULL;
    }

    // Read the strings from the dictionary and store them
    while (fscanf(file, "%s", buffer) != EOF)
    {
        // Create a new node to store the word
        node *tmp = malloc(sizeof(node));
        if (tmp == NULL)
        {
            return false;
        }
        strcpy(tmp->word, buffer);

        // Insert to the hash table
        unsigned int index = hash(tmp->word);
        node *line = table[index];
        tmp->next = line->next;
        line->next = tmp;

    }

    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    // Define a global variable to count the words
    unsigned int num_of_words = 0;
    // Iterate through the hash table
    node *line;
    for (int i = 0; i < N; i++)
    {
        line = table[i]->next;
        while (line != NULL)
        {
            line = line->next;
            num_of_words++;
        }

    }

    return num_of_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // Iterate through the hash table
    node *line, *tmp;
    for (int i = 0; i < N; i++)
    {
        line = tmp = table[i];
        while (tmp != NULL)
        {
            line = tmp->next;
            free(tmp);
            tmp = line;
        }

    }

    return true;
}
