#include <stdio.h>
#include <stdlib.h>
#define CWID 4
#define CHGT 3

int func4();

main()
{
    short mat[CHGT][CWID], i, j;

    for (j = 0; j < CHGT; j++)
	for (i = 0; i < CWID; i++) {
	    mat[j][i] = j * CWID + i;
	}

    printf(" Initialized data to: ");
    for (j = 0; j < CHGT; j++) {
	printf("\n");
	for (i = 0; i < CWID; i++) {
	    printf("%5.2d", mat[j][i]);
	}
    }
    printf("\n");

    func4(mat, CWID, CHGT);
}

int func4(short **mat, int width, int height)
{
    short i, j, *index[width];

    for (i = 0; i < height; i++)
	index[i] = (short *) mat + width * i;

    printf(" Declare as double-pointer, use auxiliary pointer array: ");
    for (j = 0; j < height; j++) {
	printf("\n");
	for (i = 0; i < width; i++) {
	    printf("%5.2d", index[j][i]);
	}
    }
    printf("\n");

    return;
}
