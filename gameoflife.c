// Can be used just to text library
// It ignores all command line options
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    // Uncomment to store mulitple universes
    // struct universe r;

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
    char prev_arguments[5] = "";
    int numarguments = 0;

    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            printf("%s", prev_arguments);
            if (strchr(prev_arguments, argv[i][1]) != NULL)
            {
                fprintf(stderr, "Already specified this argument ------> -%c  (no conflicting arguments allowed)", argv[i][1]);
                exit(1);
            }
            else
            {
                prev_arguments[numarguments] = argv[i][1];
                numarguments++;
            }
            switch (argv[i][1])
            {
            case 'i':

                inputfile = argv[i + 1];
                input_specified = 1;
                infp = fopen(argv[i + 1], "r");
                // infp2 = fopen(argv[i + 1], "r");
                if ((i + 1) >= argc || inputfile[0] == '-')
                {
                    fprintf(stderr, "Didnt Specify an Input File after '-i' ");
                    exit(1);
                }
                break;
            case 'o':
                outputfile = argv[i + 1];
                output_specified = 1;

                if ((i + 1) >= argc || outputfile[0] == '-')
                {
                    fprintf(stderr, "Didnt Specify an Output File after '-o' ");
                    exit(1);
                }
                outfp = fopen(argv[i + 1], "w");
                break;
            case 'g':

                if (isNumber(argv[i + 1]) == 0)
                {
                    fprintf(stderr, "The number of evolutions has to be an integer greater or equal to 0");
                    exit(1);
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
                exit(1);
            }
        }
    }

    if (input_specified == 0)
    {

        read_in_file(stdin, &v);
        // read_in_file(stdin, &r);
    }
    else
    {
        read_in_file(infp, &v);
        // read_in_file(infp, &r);
    }

    while (numberofgenerations > 0)
    {
        evolve(&v, rule);
        // evolve(&r, rule);
        numberofgenerations--;
    }

    if (output_specified == 0)
    {
        write_out_file(stdout, &v);
        // write_out_file(stdout, &r);
    }
    else
    {
        write_out_file(outfp, &v);
        // write_out_file(outfp, &r);
    }

    if (print_stats == 1)
    {
        print_statistics(&v);
        // print_statistics(&r);
    }

    return 0;
}
