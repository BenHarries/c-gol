#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXCHAR 1000

struct universe
{
    /*Put some appropriate things here*/
    int width;
    int height;
    int *arr;
};

void read_in_file(FILE *infile, struct universe *u)
{
    char str[MAXCHAR];
    int width;
    int height;
    int i;
    int j;

    if (infile == NULL)
    {
        printf("Could not open file");
    }
    height = 0;

    while (fgets(str, MAXCHAR, infile) != NULL)
    {
        width = strlen(str);
        height++;
    }

    u->height = height;
    u->width = width;

    int r = width, c = height;
    u->arr = (int *)malloc(r * c * sizeof(int));

    int booli;

    rewind(infile);
    j = 0;
    while (fgets(str, MAXCHAR, infile) != NULL)
    {
        for (i = 0; str[i] != 0; i++)
        {
            if (i == 0)
            {
                // printf("\n");
            }
            if (str[i] == '.')
            {
                // printf("%c", str[i]);
                booli = 0;
            }
            else
            {
                // printf("%c", str[i]);

                booli = 1;
            }
            // printf("\n%d %d", j, i);
            *(u->arr + i + j * r) = booli;
        }
        j++;
    }
    // printf("\n");

    // printf("width: %d height: %d", u->width, u->height);

    fclose(infile);
}

void write_out_file(FILE *outfile, struct universe *u)
{
    int r, i, j, c;

    for (i = 0; i < u->height; i++)
        for (j = 0; j < u->width; j++)
        {
            // printf("\n%d %d",j,i);

            if (i > 0 && j == 0)
            {
                printf("\n");
            }

            if (*(u->arr + j + i * u->width) == 1)
            {
                printf("*");
            }
            else
            {
                printf(".");
            }
        }

    fclose(outfile);
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
                    changei = u->height;
                    printf("overtheedge");
                }
                if (((column + i) > nColumns - 1))
                {
                    changei = -(u->height);
                    printf("overtheedge");
                }
                if ((row + j) < 0)
                {
                    changej = u->width;
                    printf("overtheedge");
                }
                if ((row + j) > nRows - 1)
                {
                    changej = -(u->width);
                    printf("overtheedge");
                }

                if (((column + i) < 0) || ((column + i) > nColumns - 1) || (row + j) < 0 || (row + j) > nRows - 1)
                {
                    printf(" -> Going to %d %d", column + i + changei, row + j + changej);
                }

                printf("TRUE");
                if (is_alive(u, column + i + changei, row + j + changej))
                {
                    counter++;
                    printf(" ALIVE");
                }
            }
        }
    }
    printf("\nAlive Neigbours: %d", counter);

    if (is_alive(u, column, row))
    {
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
}

// int main()
// {
//     char *filename = "./glider.txt";
//     FILE *fp;

//     struct universe v;

//     fp = fopen(filename, "r");
//     read_in_file(fp, &v);

//     int r, i, j, c, result;
//     r = v.height;
//     c = v.width;
//     printf("\n\n\n\n%d \n%d\n\n\n\n", r, c);

//     for (i = 0; i < r; i++)
//         for (j = 0; j < c; j++)
//             printf("%d ", *(v.arr + i * c + j));

//     // result = will_be_alive_torus(&v, 19, 19);
//     // printf("\nWill Be Alive? %d \n", result);

//     printf("Evolving\n");

//     int (*fun_ptr_arr[])(struct universe * u, int column, int row) = {will_be_alive, will_be_alive_torus};

//     evolve(&v, fun_ptr_arr[0]);

// }