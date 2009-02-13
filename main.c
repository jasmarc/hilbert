#include <stdio.h>
#include <stdlib.h>
#define CWID 4
#define CHGT 6

int func4();

main()
{
    short mat[CWID][CHGT], i, j;

    for (j = 0; j < CHGT; j++)
	for (i = 0; i < CWID; i++) {
	    mat[i][j] = j * CWID + i;
	}

    printf(" Initialized data to: ");
    for (j = 0; j < CHGT; j++) {
	printf("\n");
	for (i = 0; i < CWID; i++) {
	    printf("%5.2d", mat[i][j]);
	}
    }
    printf("\n");

    func4(mat, CWID, CHGT);
}

int func4(short **mat, int width, int height)
{
    short i, j, *index[height];

    for (i = 0; i < width; i++)
	index[i] = (short *) mat + height * i;

    printf(" Declare as double-pointer, use auxiliary pointer array: ");
    for (j = 0; j < height; j++) {
	printf("\n");
	for (i = 0; i < width; i++) {
	    printf("%5.2d", index[i][j]);
	}
    }
    printf("\n");

    return;
}
