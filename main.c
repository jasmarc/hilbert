#include <stdio.h>
#include <stdlib.h>

int func4();

main()
{
	short mat[3][3],i,j;
	
	for(i = 0 ; i < 3 ; i++)
		for(j = 0 ; j < 3 ; j++)
		{
			mat[i][j] = i*10 + j;
		}
	
	printf(" Initialized data to: ");
	for(i = 0 ; i < 3 ; i++)
	{
		printf("\n");
		for(j = 0 ; j < 3 ; j++)
		{
			printf("%5.2d", mat[i][j]);
		}
	}
	printf("\n");
	
	func4(mat);
}

int func4(short **mat) {
	short    i, j, *index[3];
	
	for (i = 0 ; i < 3 ; i++)
		index[i] = (short *)mat + 3*i;
	
	printf(" Declare as double-pointer, use auxiliary pointer array: ");
	for(i = 0 ; i < 3 ; i++)
	{
		printf("\n");
		for(j = 0 ; j < 3 ; j++)
		{
			printf("%5.2d", index[i][j]);
		}
	}
	printf("\n");
	
	return;
}
