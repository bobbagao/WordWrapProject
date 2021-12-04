#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

// wrap function
char * wrap(char sample_text[], int width)
{
    int wrap_location = 0, previous_wrap = 0;

    // go through the entire line until null terminator
    // repeat loop and increment 'i' as well as the location of the wrap
    for (int i = 0; sample_text[i] != '\0'; i++, wrap_location++)
    {
        // if the wrap is longer than the given width
        if (wrap_location >= width)
        {
            for (int j = i; j > 0; j--)
            {
                // make sure word wrap doesn't overflow past width
                if (j - previous_wrap <= width && (isspace(sample_text[j])) )
                {
                    // insert new line
                    sample_text[j] = '\n';
                    previous_wrap = j + 1;
                    break;
                }
            }
            wrap_location = i - previous_wrap;
        }
    } // end function

    for (int i = 0; i < width; i++)
        printf(" ");

    printf("|\n");

    return sample_text;
}

