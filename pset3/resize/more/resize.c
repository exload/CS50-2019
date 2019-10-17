// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

#include <math.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize f infile outfile\n");
        return 1;
    }
    double f = atof(argv[1]);
    if (f <= 0 || f > 101)
    {
        printf("n must be (0.0, 100.0]. Usage: ./resize f infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 4;
    }
    // Just in case
    // DWORD old_bfSize = bf.bfSize;
    LONG old_biWidth = bi.biWidth;
    LONG old_biHeight = bi.biHeight;
    // DWORD old_biSizeImage = bi.biSizeImage

    // Determine padding for scanlines for old file (smaller)
    int old_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    int n = 0;
    if (f >= 1)
    {
        n = round(f);

        // Edit meta-data
        bi.biWidth = bi.biWidth * n;
        bi.biHeight = bi.biHeight * n;
        // determine padding for scanlines for new file (larger)
        int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
        bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
        bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
        // write outfile's BITMAPFILEHEADER
        fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
        // write outfile's BITMAPINFOHEADER
        fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

        // iterate over infile's scanlines
        for (int i = 0, old_biHeight_a = abs(old_biHeight); i < old_biHeight_a; i++)
        {
            // Adjustment of the number of rows
            for (int row = 0; row < n; row++)
            {
                // iterate over pixels in scanline
                for (int j = 0; j < old_biWidth; j++)
                {
                    // temporary storage
                    RGBTRIPLE triple;
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                    // write RGB triple to outfile
                    for (int col = 0; col < n; col++)
                    {
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
                }
                // skip over padding, if any
                fseek(inptr, old_padding, SEEK_CUR);
                // then add it back (to demonstrate how)
                for (int k = 0; k < padding; k++)
                {
                    fputc(0x00, outptr);
                }
                // Cursor shift to the beginning of the line if this line not last
                if ((row + 1) != n)
                {
                    fseek(inptr, (-(sizeof(RGBTRIPLE) * old_biWidth + old_padding)), SEEK_CUR);
                }
            }
        }
    }
    else
    {
        // Edit meta-data
        bi.biWidth = floor(bi.biWidth * f);
        bi.biHeight = floor(bi.biHeight * f);
        // determine padding for scanlines for new file (larger)
        int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
        bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
        bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;

        // write outfile's BITMAPFILEHEADER
        fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
        // write outfile's BITMAPINFOHEADER
        fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
        int shift_over = (old_biWidth / bi.biWidth) - 1;

        for (int row = 0, biHeight_a = abs(bi.biHeight); row < biHeight_a; row++)
        {
            // iterate over infile's scanline
            int i = 0;
            // iterate over outfile scanlines
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                // skip shifted pixels
                if ((j + 1) != bi.biWidth)
                {
                    fseek(inptr, (sizeof(RGBTRIPLE) * shift_over), SEEK_CUR);
                    i = i + shift_over + 1;
                }
            }
            // skip to the end of scanline in infile
            fseek(inptr, ((sizeof(RGBTRIPLE) * (old_biWidth - (i + 1))) + old_padding), SEEK_CUR);
            // then add padding to outfile
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
            // skip infile lines
            fseek(inptr, (sizeof(RGBTRIPLE) * old_biWidth + old_padding) * shift_over, SEEK_CUR);
        }
    }

    // close infile
    fclose(inptr);
    // close outfile
    fclose(outptr);
    // success
    return 0;
}