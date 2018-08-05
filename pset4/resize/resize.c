// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }

    float alpha = strtof(argv[1], NULL);
    if(alpha == 0){
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 1;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);


    int Oldpadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    long oldH = bi.biHeight;
    long oldW = bi.biWidth;

    //Edit InfoHeader
    int ULine = floor(alpha*bi.biWidth)-floor(alpha)*bi.biWidth;

    bi.biWidth = bi.biWidth*alpha;
    bi.biHeight = bi.biHeight*alpha;

    // determine padding for scanlines
    int padding = ((4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4);

    bi.biSizeImage = (bi.biWidth*sizeof(RGBTRIPLE)+padding)*abs(bi.biHeight);

    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 1;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);


    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    RGBTRIPLE ver[bi.biWidth];
    // iterate over infile's scanlines
    for (int i = 0, biHeight = labs(oldH); i < biHeight; i++)
    {
        int iter = 0;
        // iterate over pixels in scanline
        for (int j = 0; j < oldW; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            if(j<ULine){
                for(int k =0; k< ceil(alpha); k++)
                {
                    ver[k+iter] = triple;
                }
                iter+=ceil(alpha);
            }
            else{
                for(int k =0; k< floor(alpha); k++)
                {
                    ver[k+iter] = triple;
                }
                iter+=floor(alpha);
            }

        }

        // skip over padding, if any
            fseek(inptr, Oldpadding, SEEK_CUR);

        // then add it back (to demonstrate how)
            if(i<ULine)
            {
                for(int j =0; j < ceil(alpha); j++)
                {
                    fwrite(ver,sizeof(RGBTRIPLE),bi.biWidth,outptr);
                    for (int k = 0; k < padding; k++){fputc(0x00, outptr);}
                }
            }
            else
            {
                for(int j =0; j < floor(alpha); j++)
                {
                fwrite(ver,sizeof(RGBTRIPLE),bi.biWidth,outptr);
                for (int k = 0; k < padding; k++){fputc(0x00, outptr);}
                }
            }

    }


    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

