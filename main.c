#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define hilbert_size(x) (3 * x + 1)

enum hilbert_types { A, B, C, D };

const char Bcurve[3][3] = { "***",
							"*  ",
							"***" };

const char Acurve[3][3] = { "***",
							"* *",
							"* *" };

const char Dcurve[3][3] = { "***",
							"  *",
							"***" };

const char Ccurve[3][3] = { "* *",
							"* *",
							"***" };

void hilbert(char **dest, int order, int hilbert_type);
void mat_print(char **mat, int size);
void mat_set(char **mat, int size, int x, int y, char val);
void mat_copy(char **dest,	char **src,
			  int d_size,	int s_size,
			  int offset_x,	int offset_y);

int main(int argc, char *argv[])
{
	char **bar;
	char **foo;
	const int order = 2;
	bar = malloc(hilbert_size(order)*hilbert_size(order));
	foo = malloc(hilbert_size(order-1)*hilbert_size(order-1));
	memset(bar, ' ', hilbert_size(order)*hilbert_size(order));
	memcpy(foo, Dcurve, hilbert_size(order-1)*hilbert_size(order-1));
	mat_copy(bar, foo, 7, 3, 0, 4);
	mat_set(bar, 7, 0, 3, '1');
	memcpy(foo, Acurve, 9);
	mat_copy(bar, foo, 7, 3, 0, 0);
	mat_set(bar, 7, 3, 2, '2');
	mat_copy(bar, foo, 7, 3, 4, 0);
	mat_set(bar, 7, 6, 3, '3');
	memcpy(foo, Bcurve, 9);
	mat_copy(bar, foo, 7, 3, 4, 4);
	mat_print(bar, 7);
	return 0;
}

void hilbert(char **dest, int order, int hilbert_type)
{
	int size = hilbert_size(order);
	int i, j;
	char *index[size];
	
	for (i = 0; i < size; i++)
		index[i] = (char *) dest + size * i;
	
	if(order == 1) {
		switch (hilbert_type) {
			/* case A:
				index = { "***",
						  "*  ",
						  "***" }; 
				break;
			case B:
				*index = { "***",
						  "* *",
						  "* *" };
				break;
			case C:
				*index = { "***",
						  "  *",
						  "***" };
				break;
			case D:
				*index = { "* *",
						  "* *",
						  "***" };
				break; */
			default:
				fprintf(stderr, "Unknown hilbert type: %d", hilbert_type);
				break;
		}
	}
	else if(order > 1) {
		switch (hilbert_type) {
			case A:
				;
				break;
			case B:
				;
				break;
			case C:
				;
				break;
			case D:
				;
				break;
			default:
				fprintf(stderr, "Unknown hilbert type: %d", hilbert_type);
				break;
		}
	}
	else {
		fprintf(stderr, "Order must be greater than or equal to 1.");
	}
	
}

void mat_print(char **mat, int size)
{
	char i, j, *index[size];

	for (i = 0; i < size; i++)
		index[i] = (char *) mat + size * i;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++)
			printf("%c", index[j][i]);
		printf("\n");
	}

	return;
}

void mat_set(char **mat, int size, int x, int y, char val)
{
	char i, j, *index[size];

	for (i = 0; i < size; i++)
		index[i] = (char *) mat + size * i;

	index[y][x] = val;

	return;
}

void mat_copy(char **dest,	char **src,
			  int d_size,	int s_size,
			  int offset_x,	int offset_y)
{
	char i, j, *s_index[s_size], *d_index[d_size];

	for (i = 0; i < s_size; i++)
		s_index[i] = (char *) src + s_size * i;
	for (i = 0; i < d_size; i++)
		d_index[i] = (char *) dest + d_size * i;

	for (i = 0; i < s_size; i++)
		for (j = 0; j < s_size; j++)
			d_index[offset_y + j][offset_x + i] = s_index[j][i];

	return;
}
