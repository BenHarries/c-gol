#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "gol.h"

#define MAXCHAR 1000

void read_in_file(FILE *infile, struct universe *u)
{

    rewind(infile);

    u->evolutions = 1;
    int width;
    int height;

    int *str;
    int ch;
    int len = 0;
    int size;
    int first_line = 0;

    size = 10;
    height = 1;
    width = -1;

    str = realloc(NULL, sizeof(int) * size); //size is start size

    while (EOF != (ch = fgetc(infile)))
    {
        width++;

        if (ch == '\n')
        {
            if (width > 512)
            {
                fprintf(stderr, "\nNumber of Columns exceeds 512 (Need to reduce the input universe to a smaller number of columns)");
                exit(1);
            }
            if (first_line)
            {
                if (width != u->width)
                {
                    fprintf(stderr, "\nMalformed Input (not rectanglular)");
                    exit(1);
                }
            }
            first_line = 1;
            u->width = width;

            width = -1;
            height++;
        }
        else
        {
            if (ch == '.')
            {
                str[len++] = 0;
            }
            else
            {
                if (ch == '*')
                {
                    printf("total %d", u->totalAlive);
                    u->totalAlive += 1;
                    printf("total %d", u->totalAlive);

                    str[len++] = 1;
                }
                else
                {
                    fprintf(stderr, "\nInput contains character  ----->  %c\nOnly * or . allowed", ch);
                    exit(1);
                }
            }
        }

        if (len == size)
        {
            str = realloc(str, sizeof(int) * (size += 16));
        }
    }

    u->height = height;

    u->arr = realloc(str, sizeof(int) * len);
}

void write_out_file(FILE *outfile, struct universe *u)
{
    int i, j;
    printf("\n");

    for (i = 0; i < u->height; i++)
        for (j = 0; j < u->width; j++)
        {

            if (i > 0 && j == 0)
            {
                fprintf(outfile, "\n");
            }

            if (*(u->arr + j + i * u->width) == 1)
            {
                fprintf(outfile, "*");
            }
            else
            {
                fprintf(outfile, ".");
            }
        }
}

int is_alive(struct universe *u, int column, int row)
{
    return *(u->arr + row * u->width + column);
}

int will_be_alive(struct universe *u, int column, int row)
{

    // printf("Lets try%d    %d ", *(u->arr + row * u->width + column), row * u->width + column);

    int i, j, counter, nColumns, nRows;
    nColumns = u->width;
    nRows = u->height;
    counter = 0;
    for (i = -1; i < 2; i++)
    {
        for (j = -1; j < 2; j++)
        {
            // printf("\nNeighbour %d,%d =", column + i, row + j);

            if (i == 0 && j == 0)
            {
            }
            else
            {
                if (((column + i) < 0) || ((column + i) > nColumns - 1) || (row + j) < 0 || (row + j) > nRows - 1)
                {
                }
                else
                {
                    // printf("TRUE");
                    if (is_alive(u, column + i, row + j))
                    {
                        counter++;
                        // printf(" ALIVE");
                    }
                }
            }
        }
    }

    if (is_alive(u, column, row))
    {
        printf("total %d", u->totalAlive);

        u->totalAlive += 1;
        printf("total %d", u->totalAlive);

        if (counter == 2 || counter == 3)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (counter == 3)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}
int will_be_alive_torus(struct universe *u, int column, int row)
{
    // printf("Lets try%d    %d ", *(u->arr + row * u->width + column), row * u->width + column);

    int i, j, counter, nColumns, nRows, changei, changej;
    nColumns = u->width;
    nRows = u->height;
    counter = 0;
    for (i = -1; i < 2; i++)
    {
        for (j = -1; j < 2; j++)
        {
            changei = 0;
            changej = 0;
            // printf("\nNeighbour %d,%d =", column + i, row + j);

            if (i == 0 && j == 0)
            {
            }
            else
            {

                if (((column + i) < 0))
                {
                    changei = u->width;
                    // printf("overtheedge");
                }
                if (((column + i) > nColumns - 1))
                {
                    changei = -(u->width);
                    // printf("overtheedge");
                }
                if ((row + j) < 0)
                {
                    changej = u->height;
                    // printf("overtheedge");
                }
                if ((row + j) > nRows - 1)
                {
                    changej = -(u->height);
                    // printf("overtheedge");
                }

                if (((column + i) < 0) || ((column + i) > nColumns - 1) || (row + j) < 0 || (row + j) > nRows - 1)
                {
                    // printf(" -> Going to %d %d", column + i + changei, row + j + changej);
                }

                // printf("TRUE");
                if (is_alive(u, column + i + changei, row + j + changej))
                {
                    counter++;
                    // printf(" ALIVE");
                }
            }
        }
    }
    // printf("\nAlive Neigbours: %d", counter);

    if (is_alive(u, column, row))
    {
        printf("total %d", u->totalAlive);

        u->totalAlive += 1;
        printf("total %d", u->totalAlive);

        if (counter == 2 || counter == 3)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (counter == 3)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}
void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row))
{
    u->evolutions++;
    int *temp = (int *)malloc(u->height * u->width * sizeof(int));

    int i, j, result;
    for (i = 0; i < u->height; i++)
    {
        for (j = 0; j < u->width; j++)
        {

            result = rule(u, j, i);
            // printf("\nresult %d", i);
            if (result == 1)
            {
                *(temp + j + i * u->width) = 1;
            }
            else
            {
                *(temp + j + i * u->width) = 0;
            }
        }
    }

    for (i = 0; i < u->height; i++)
    {
        for (j = 0; j < u->width; j++)
        {
            *(u->arr + j + i * u->width) = *(temp + j + i * u->width);
            // printf("U %d", *(u->arr + i * u->height + j));
        }
    }
}
void print_statistics(struct universe *u)
{
    // printf("Hello");
    int i, j;
    float currentStat, currentAlive, historicalStat, totalAlivefloat;
    currentAlive = 0;
    for (i = 0; i < u->height; i++)
    {
        for (j = 0; j < u->width; j++)
        {
            if (*(u->arr + j + i * u->width) == 1)
            {
                currentAlive++;
            }
            // printf("U %d", *(u->arr + i * u->height + j));
        }
    }

    currentStat = (currentAlive * 100) / (u->height * u->width);
    float nearest = roundf(currentStat * 1000) / 1000;

    printf("\n%.3f%% of cells currently alive", nearest);
    printf("\n%d yo", u->totalAlive);

    totalAlivefloat = (float)u->totalAlive;

    printf("\n%d mo", u->totalAlive);

    historicalStat = (totalAlivefloat * 100) / (u->height * u->width * u->evolutions);

    float nearestH = roundf(historicalStat * 1000) / 1000;

    printf("\n%.3f%% of cells alive on average\n", nearestH);
}
