// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')


int si = 0;

// Returns true if word is in dictionary else false
typedef struct node {
    bool is_word;
    struct node* children[27];
}node;
node *ROOT;


struct node *getNode(void){
    struct node *pNode = NULL;
    pNode = (struct node*)malloc(sizeof(node));

    if(pNode){

        pNode->is_word = false;

        for(int i =0; i<27;i++){
            pNode->children[i] = NULL;
        }
    }
    return pNode;
}

void insert(node *root, const char *key)
{
    int level;
    int length = strlen(key);
    int alpha;

    struct node *pCrawl = root;

    for (level = 0; level < length; level++)
    {
        alpha = CHAR_TO_INDEX(key[level]);
        if(alpha == -58){
            alpha = 26;
        }
        if (!pCrawl->children[alpha]){
            pCrawl->children[alpha] = getNode();
        }
        pCrawl = pCrawl->children[alpha];
    }

    // mark last node as leaf
    pCrawl->is_word = true;
}

bool search(struct node *root, const char *key)
{
    int level;
    int length = strlen(key);
    int alpha;
    struct node *pCrawl = root;

    for (level = 0; level < length; level++)
    {
      if (isalpha(key[level]) || (key[level] == '\'' ))
      {
        char A = tolower(key[level]);
        alpha = CHAR_TO_INDEX(A);
        if(alpha == -58){
            alpha = 26;
        }

        if (!pCrawl->children[alpha])
            return false;

        pCrawl = pCrawl->children[alpha];
      }
    }
    return (pCrawl != NULL && pCrawl->is_word);
}



bool check(const char *word)
{
    return search(ROOT, word);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    ROOT = getNode();
    int index = 0;
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    char *word;
    word = (char *)malloc((LENGTH+1)*sizeof(char));
    for (int c = fgetc(file); c != EOF; c = fgetc(file))
    {
        // Allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // Append character to word
            word[index] = c;
            index++;

            // Ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // Consume remainder of alphabetical string
                while ((c = fgetc(file)) != EOF && isalpha(c));

                // Prepare for new word
                index = 0;
            }
        }


        // We must have found a whole word
        else if (index > 0)
        {
            // Terminate current word
            word[index] = '\0';

            insert(ROOT, word);
            //SIZE increase by one
            si++;
            // Prepare for next word
            index = 0;
        }
    }
    free(word);
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return si;
}

void fr(struct node *pCrawl)
{
    for(int i =0; i<27; i++)
    {
        if(pCrawl->children[i]!= NULL)
        {
            fr(pCrawl->children[i]);
        }
    }
    free(pCrawl);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    fr(ROOT);
    return true;
}


