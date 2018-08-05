#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *arg[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Input\n");
        return 1;
    }

    //reading input file
    char *infile = arg[1];

    FILE *inptr = fopen(infile, "r");


    //check for null
    if(inptr == NULL)
    {
        fprintf(stderr, "Cannot read file %s.\n",infile);
        return 1;
    }


    unsigned char buffer[512];

    char outfile[8];

    int i = 0;
    FILE *outpr = NULL;
    while(fread(buffer, 512, 1, inptr) == 1)
    {
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) ==0xe0)
        {
            if(i>0){fclose(outpr);}
            sprintf(outfile,"%03d.jpg", i);
            i++;
            outpr = fopen(outfile,"a");
        }
        if(i>0)
        {
        fwrite(&buffer, 512, 1, outpr);
        }

    }

    fclose(inptr);
    fclose(outpr);
    return 0;
}