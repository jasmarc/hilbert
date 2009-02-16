#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define hilbert_size(x) ((int)pow((double)2,(double)(x+1))-1)
#define MAX_ORDER 7
#define MAX_SIZE hilbert_size(MAX_ORDER)

enum direction { LEFT, RIGHT, UP, DOWN };
enum curve_type { A, B, C, D };
enum start_positions { BOTTOM_LEFT, TOP_RIGHT };

typedef struct {
    char buffer[MAX_SIZE][MAX_SIZE];
    int size;
} hilbert;

typedef struct {
    int x, y;
} pen;

void do_hilbert(hilbert *hil, pen *p, int order, int type);
void init(hilbert *hil, int size);
void print(hilbert *hil);
void move_absolute(pen *p, int x, int y);
void move(pen *p, int dir);
void start(hilbert *hil, pen *p, int start_pos);
void mark(hilbert *hil, pen *p);

int main(int argc, char *argv[])
{
    hilbert *foo = malloc(sizeof(hilbert));
    pen *p = malloc(sizeof(pen));
    init(foo, hilbert_size(5));
    start(foo, p, BOTTOM_LEFT);
    do_hilbert(foo, p, 5, C);
    print(foo);
    free(foo);
    free(p);
    return 0;
}

void do_hilbert(hilbert *hil, pen *p, int order, int type)
{
    assert(order > 0);
    if(order == 1) {
        switch (type) {
            case A:
                mark(hil, p);
                move(p, LEFT);
                mark(hil, p);
                move(p, LEFT);
                mark(hil, p);
                move(p, DOWN);
                mark(hil, p);
                move(p, DOWN);
                mark(hil, p);
                move(p, RIGHT);
                mark(hil, p);
                move(p, RIGHT);
                mark(hil, p);
                break;
            case B:
                mark(hil, p);
                move(p, UP);
                mark(hil, p);
                move(p, UP);
                mark(hil, p);
                move(p, RIGHT);
                mark(hil, p);
                move(p, RIGHT);
                mark(hil, p);
                move(p, DOWN);
                mark(hil, p);
                move(p, DOWN);
                mark(hil, p);
                break;
            case C:
                mark(hil, p);
                move(p, RIGHT);
                mark(hil, p);
                move(p, RIGHT);
                mark(hil, p);
                move(p, UP);
                mark(hil, p);
                move(p, UP);
                mark(hil, p);
                move(p, LEFT);
                mark(hil, p);
                move(p, LEFT);
                mark(hil, p);
                break;
            case D:
                mark(hil, p);
                move(p, DOWN);
                mark(hil, p);
                move(p, DOWN);
                mark(hil, p);
                move(p, LEFT);
                mark(hil, p);
                move(p, LEFT);
                mark(hil, p);
                move(p, UP);
                mark(hil, p);
                move(p, UP);
                mark(hil, p);
                break;
            default:
                fprintf(stderr, "Unknown hilbert type: %d", type);
                break;
        } // switch
    } // if
    else {
        switch (type) {
            case A:
                do_hilbert(hil, p, order - 1, D);
                move(p, LEFT);
                mark(hil, p);
                move(p, LEFT);
                do_hilbert(hil, p, order - 1, A);
                move(p, DOWN);
                mark(hil, p);
                move(p, DOWN);
                do_hilbert(hil, p, order - 1, A);
                move(p, RIGHT);
                mark(hil, p);
                move(p, RIGHT);
                do_hilbert(hil, p, order - 1, B);
                break;
            case B:
                do_hilbert(hil, p, order - 1, C);
                move(p, UP);
                mark(hil, p);
                move(p, UP);
                do_hilbert(hil, p, order - 1, B);
                move(p, RIGHT);
                mark(hil, p);
                move(p, RIGHT);
                do_hilbert(hil, p, order - 1, B);
                move(p, DOWN);
                mark(hil, p);
                move(p, DOWN);
                do_hilbert(hil, p, order - 1, A);
                break;
            case C:
                do_hilbert(hil, p, order - 1, B);
                move(p, RIGHT);
                mark(hil, p);
                move(p, RIGHT);
                do_hilbert(hil, p, order - 1, C);
                move(p, UP);
                mark(hil, p);
                move(p, UP);
                do_hilbert(hil, p, order - 1, C);
                move(p, LEFT);
                mark(hil, p);
                move(p, LEFT);
                do_hilbert(hil, p, order - 1, D);
                break;
            case D:
                do_hilbert(hil, p, order - 1, A);
                move(p, DOWN);
                mark(hil, p);
                move(p, DOWN);
                do_hilbert(hil, p, order - 1, D);
                move(p, LEFT);
                mark(hil, p);
                move(p, LEFT);
                do_hilbert(hil, p, order - 1, D);
                move(p, UP);
                mark(hil, p);
                move(p, UP);
                do_hilbert(hil, p, order - 1, C);
                break;
            default:
                fprintf(stderr, "Unknown hilbert type: %d", type);
                break;
        } // switch
    } // else
}

void init(hilbert *hil, int size)
{
    memset(hil->buffer, ' ', sizeof(hil->buffer));
    hil->size = size;
}

void print(hilbert *hil)
{
    int i, j;
    for (i = 0; i < hil->size; i++) {
        for (j = 0; j < hil->size; j++)
            printf("%c", hil->buffer[j][i]);
        printf("\n");
    }

    return;
}

void move_absolute(pen *p, int x, int y)
{
    p->x = x;
    p->y = y;
    return;
}

void move(pen *p, int dir)
{
    switch (dir) {
        case LEFT:
            move_absolute(p, p->x - 1, p->y);
            break;
        case RIGHT:
            move_absolute(p, p->x + 1, p->y);
            break;
        case UP:
            move_absolute(p, p->x, p->y - 1);
            break;
        case DOWN:
            move_absolute(p, p->x, p->y + 1);
            break;
        default:
            break;
    }
}

void start(hilbert *hil, pen *p, int start_pos)
{
    if(start_pos == BOTTOM_LEFT) {
        p->x = 0;
        p->y = hil->size - 1;
    }
    else if(start_pos == TOP_RIGHT) {
        p->x = hil->size - 1;
        p->y = 0;
    }
    else {
        fprintf(stderr, "Unknown start position: %d", start_pos);
    }
    return;
}

void mark(hilbert *hil, pen *p)
{
    assert(p->x < hil->size);
    assert(p->y < hil->size);
    hil->buffer[p->x][p->y] = '*';
    return;
}
