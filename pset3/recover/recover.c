#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int block_size = 512;
    // Checking the correct number of parameters
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // Trying to opern file
    FILE *fileptr = fopen(argv[1], "r");
    if (fileptr == NULL)
    {
        fprintf(stderr, "Can not open supplyed file to read in current directory. Usage: ./recover image\n");
        return 2;
    }
    // Allocate memory on the heap
    char *buff = malloc(block_size * sizeof(char));
    char *filename = malloc(8 * sizeof(char));
    // Iterator for file name
    int i = 0;
    // Pointer to output files
    FILE *outfile = NULL;

    // Read blocks at 512B and check for EOF
    while (fread(buff, block_size, 1, fileptr) == 1)
    {
        if (((buff[0] & 0xff) == 0xff) && ((buff[1] & 0xff) == 0xd8) && ((buff[2] & 0xff) == 0xff) && ((buff[3] & 0xf0) == 0xe0))
        {
            // Generate first outfile
            if (outfile == NULL)
            {
                sprintf(filename, "%03i.jpg", i);
                i++;
                outfile = fopen(filename, "w");
            }
            else
            {
                fclose(outfile);
                sprintf(filename, "%03i.jpg", i);
                i++;
                outfile = fopen(filename, "w");
            }
        }
        // Skip until 1'st JPEG signature found
        if (i != 0)
        {
            fwrite(buff, block_size * sizeof(char), 1, outfile);
        }
    }

    // Close file pointer's
    fclose(outfile);
    fclose(fileptr);

    // Memory release
    free(buff);
    free(filename);
}