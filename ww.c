#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ww.h"

int main(int argc, char *argv[])
{
    // declare file variables
    char *input_fd;
    int width;
    struct stat statbuf;

    // sample output
    char sample_text[] = "This is very good text that has been put into a file for the purpose of being an example.\n\nThat is good.";

    /* input validation -- usage should be: 
        -   ./ww [width<int>] [textfile.txt<string>]
    */

    // check if no parameters were inputed
    if (argc == 1)
    {
        printf("No parameters specified. Continuing with default parameters.\n");
        printf("This is an example with the width as 30\n\n");
        wrap(sample_text, 30);

        printf("%s", sample_text);

        return EXIT_SUCCESS;
    }

    // check if too many parameters were inputed
    if (argc > 3)
    {
        printf("ERROR: Too many parameters. \nUsage: ./ww [width<int>] [textfile.txt<string>\n");
        return EXIT_FAILURE;
    }

    // check if only a width was given (no text file)
    if (argc == 2)
    {
        // check if the column width is too large OR too small
        if ((width = atoi(argv[1])) > 10000 || width < 1)
        {
            printf("ERROR: line width must be between 1 and 10,000.\n");
            return EXIT_FAILURE;
        }

        printf("This is sample text with an inputted width of: %d\n", width);
        printf("Enter whatever you'd like and the program will wrap the text for you"
               "based on the given width: \n");

        char userText[100];
        scanf("%[^\t\n]", userText);

        wrap(userText, width);

        printf("%s", userText);
    }

    // check if a width and a text file was given
    if (argc == 3)
    {
        input_fd = argv[2];
        if (stat(input_fd, &statbuf) == -1)
        {
            perror("stat");
            return EXIT_FAILURE;
        }
        if (S_ISREG(statbuf.st_mode))
        {
            // check if the column width is too large OR too small
            if ((width = atoi(argv[1])) > 10000 || width < 1)
            {
                printf("ERROR: line width must be between 1 and 10,000.\n");
                return EXIT_FAILURE;
            }

            // get the text file from inputted parameter
            printf("Name of the file inputted: \n%s\n\nTo a width of: %d\n", input_fd, width);

            int f = open(input_fd, O_RDONLY);

            if (f == -1)
            {
                perror("FILE DOES NOT EXIST");
            }

            char *c = (char *)calloc(100, sizeof(char));
            char blank[10000];

            read(f, c, 10000);

            int a = 0, d = 0;

            while (c[a] != '\0')
            {
                if (isspace(c[a]))
                {
                    int temp = a + 1;
                    if (c[temp] != '\0')
                    {
                        while (isspace(c[temp]) && c[temp] != '\0')
                        {
                            if (isspace(c[temp]))
                            {
                                a++;
                            }
                            temp++;
                        }
                    }
                }
                blank[d] = c[a];
                a++;
                d++;
            }

            blank[d] = '\0';
            strcpy(c, blank);

            //printf("\n\nText after removing blanks\n%s\n", blank);

            // c has all the contents of the file
            c = wrap(c, width);

            // c is a string
            printf("%s", c);

            free(c);
            int close(int f);

            return EXIT_SUCCESS;
        }
        else if (S_ISDIR(statbuf.st_mode))
        {

            if ((width = atoi(argv[1])) > 10000 || width < 1)
            {
                printf("ERROR: line width must be between 1 and 10,000.\n");
                return EXIT_FAILURE;
            }
            DIR *d;
            struct dirent *dir;
            char buf[100];
            char fileName[100];
            char data[10000];
            d = opendir(input_fd);
            if (d)
            {
                while ((dir = readdir(d)) != NULL)
                {

                    //store/reset to directory name
                    strcpy(fileName, input_fd);
                    strcat(fileName, "/");
                    strcpy(buf, input_fd);
                    strcat(buf, "/");
                    //printf("%s\n", buf);
                    //printf("filename: %s\n", dir->d_name);

                    if (strncmp(dir->d_name, ".", strlen(".")) == 0)
                        // printf("String %s starts with .\n", dir->d_name);
                        continue;
                    if (strncmp(dir->d_name, "wrap.", strlen("wrap.")) == 0)
                    {
                        //printf("String %s starts with wrap. \n", dir->d_name);
                        continue;
                    }
                    if (dir->d_type == 4)
                    {
                        printf("Directory type \n");
                        continue;
                    }

                    strcat(fileName, dir->d_name);
                    strcat(buf, "wrap.");
                    strcat(buf, dir->d_name);

                    //printf(" %s \n", buf);

                    //open read file
                    int f = open(fileName, O_RDONLY);
                    //printf("Name of the file inputted: \n%s\n\nTo a width of: %d\n", fileName, width);

                    char *c = (char *)calloc(10000, sizeof(char));
                    read(f, c, 10000);
                    char blank[10000];

                    int a = 0, d = 0;

                    while (c[a] != '\0')
                    {
                        if (isspace(c[a]))
                        {
                            int temp = a + 1;
                            if (c[temp] != '\0')
                            {
                                while (isspace(c[temp]) && c[temp] != '\0')
                                {
                                    if (isspace(c[temp]))
                                    {
                                        a++;
                                    }
                                    temp++;
                                }
                            }
                        }
                        blank[d] = c[a];
                        a++;
                        d++;
                    }

                    blank[d] = '\0';
                    strcpy(c, blank);

                    // c has all the contents of the file
                    c = wrap(c, width);

                    strcpy(data, c);

                    //printf("length %ld \n", strlen(c));
                    //printf("this is the data %s \n", data);
                    free(c);
                    int close(int f);

                    //create file to write to
                    int fnew = open(buf, O_WRONLY | O_CREAT, 0666);
                    write(fnew, data, strlen(data));
                    //printf("opened\n");
                    int close(int fnew);
                }
                closedir(d);
            }
            return EXIT_SUCCESS;
        }
        else
        {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}