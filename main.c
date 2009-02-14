#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CWID1 4
#define CHGT1 4
#define CWID2 2
#define CHGT2 2

void mat_print();
void mat_copy();

main()
{
    char mat1[CWID1][CHGT1], mat2[CWID2][CHGT2];
	short i, j;
	
	/* for (j = 0; j < CHGT1; j++)
		for (i = 0; i < CWID1; i++)
			mat1[i][j] = 'a' + j * CWID1 + i;

	printf(" Initialized data to: ");
	for (j = 0; j < CHGT1; j++) {
		printf("\n");
		for (i = 0; i < CWID1; i++)
			printf("%c", mat1[i][j]);
	} */
	
	memset(mat1, '*', CWID1*CHGT1);
	memset(mat2, '~', CWID2*CHGT2);	
	mat_copy(mat2, mat1, CWID2, CHGT2, CWID1, CHGT1, 2, 2);
	printf("calling the print routine.\n");
    mat_print(mat1, CWID1, CHGT1);
}

void mat_print(char **mat, int width, int height)
{
    char i, j, *index[width];
	
    for (i = 0; i < width; i++)
		index[i] = (char *) mat + height * i;
	
    for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++)
			printf("%c", index[i][j]);
		printf("\n");
    }
	
    return;
}

void mat_copy(char	**src,		char **dest, 
			  int	s_width,	int s_height, 
			  int	d_width,	int d_height, 
			  int	offset_x,	int offset_y)
{
    char i, j, *s_index[s_width], *d_index[d_width];
	
    for (i = 0; i < s_width; i++)
		s_index[i] = (char *) src + s_height * i;
    for (i = 0; i < d_width; i++)
		d_index[i] = (char *) dest + d_height * i;
	
    for (j = 0; j < s_height; j++)
		for (i = 0; i < s_width; i++)
			d_index[offset_x + i][offset_y + j] = s_index[i][j];
	
    return;
}