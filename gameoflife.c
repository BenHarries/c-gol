// Can be used just to text library
// It ignores all command line options
#include <stdio.h>
#include <stdlib.h>
#include "gol.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define MAXCHAR 1000

int isNumber(char number[])
{
    int i = 0;

    //checking for negative numbers
    if (number[0] == '-')
    {
        return 0;
    }
    i = 1;
    for (; number[i] != 0; i++)
    {
        if (number[i] > '9' || number[i] < '0')
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{

    struct universe v;

    // printf("%d", argc);-
    int input_specified, output_specified, print_stats;

    int i;
    int numberofgenerations;
    input_specified = 0;
    output_specified = 0;
    numberofgenerations = 5;
    print_stats = 0;
    char *outputfile;
    char *inputfile;

    int (*rule)(struct universe * u, int column, int row) = &will_be_alive;

    FILE *outfp;
    FILE *infp;

    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 'i':
                inputfile = argv[i + 1];
                input_specified = 1;
                infp = fopen(argv[i + 1], "r");
                if ((i + 1) >= argc || inputfile[0] == '-')
                {
                    fprintf(stderr, "Didnt Specify an Input File after '-i' ");
                    exit(0);
                }
                break;
            case 'o':
                outputfile = argv[i + 1];
                output_specified = 1;

                if ((i + 1) >= argc || outputfile[0] == '-')
                {
                    fprintf(stderr, "Didnt Specify an Output File after '-o' ");
                    exit(0);
                }
                outfp = fopen(argv[i + 1], "w");
                break;
            case 'g':

                if (isNumber(argv[i + 1]) == 0)
                {
                    fprintf(stderr, "The number of evolutions has to be an integer greater or equal to 0");
                    exit(0);
                }
                numberofgenerations = atoi(argv[i + 1]); // stderr check that it is a positive interger

                break;
            case 's':
                print_stats = 1;
                break;
            case 't':
                rule = &will_be_alive_torus;
                break;
            default:
                // stderr
                fprintf(stderr, "Unknown option %s\n", argv[i]);
                exit(0);
            }
        }
    }

    if (input_specified == 0)
    {
        read_in_file(stdin, &v);
    }
    else
    {
        read_in_file(infp, &v);
    }

    while (numberofgenerations > 0)
    {
        evolve(&v, rule);
        numberofgenerations--;
    }

    if (output_specified == 0)
    {
        write_out_file(stdout, &v);
    }
    else
    {
        write_out_file(outfp, &v);
    }

    if (print_stats == 1)
    {
        print_statistics(&v);
    }

    return 0;
}
