#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <inttypes.h>
#include <sys/types.h>
#include <limits.h>

#define hilbert_size(x) ((int)pow((double)2,(double)(x+1))-1)
#define MAX_ORDER 7
#define MAX_SIZE hilbert_size(MAX_ORDER)

enum direction { LEFT, RIGHT, UP, DOWN };
enum curve_type { A, B, C, D };

typedef struct {
    char buffer[MAX_SIZE][MAX_SIZE];
    int size;
} hilbert;

typedef struct {
    int x, y;
} pen;

typedef struct {
    hilbert *hil;
    pen *p;
    int order;
    int type;
} thread_data;

void set_thread_data(thread_data *t, hilbert *hil, pen *p, int order, int type);
void *do_hilbert(void *t);
void init(hilbert *hil, int size);
void print(hilbert *hil);
void move_absolute(pen *p, int x, int y);
void move(pen *p, int dir);
void start(hilbert *hil, pen *p, int type);
void mark(hilbert *hil, pen *p);

int main(int argc, char *argv[])
{
    int type, order;
    if(argc != 3
        || strlen(argv[1]) > 1
        || (type = argv[1][0] - 'A') < 0
        || type > 3
        || (order = strtol(argv[2], NULL, 10)) <= 0
        || order > MAX_ORDER) {
        fprintf(stderr, "Usage:\t%s type order\n\ttype must be A-D.\n\torder must be 1 through %d.\n", argv[0], MAX_ORDER);
        return 1;
    }
    hilbert *foo = malloc(sizeof(hilbert));
    pen *p = malloc(sizeof(pen));
    init(foo, hilbert_size(order));
    start(foo, p, type);
    pthread_t thread;
    pthread_attr_t attr;
    thread_data ta;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    set_thread_data(&ta, foo, p, order, type);
    pthread_create(&thread, &attr, do_hilbert, (void *)&ta);
    pthread_join(thread, NULL);
    print(foo);
    free(foo);
    free(p);
    return 0;
}

void set_thread_data(thread_data *t, hilbert *hil, pen *p, int order, int type)
{
    t->hil = hil;
    t->p = p;
    t->order = order;
    t->type = type;
    return;
}

void *do_hilbert(void *t)
{
    hilbert * hil = ((thread_data *)t)->hil;
    pen *p = ((thread_data *)t)->p ;
    int order = ((thread_data *)t)->order;
    int type = ((thread_data *)t)->type;
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
        pthread_t thread[4];
        pthread_attr_t attr;
        thread_data *ta[4];
        pen *start = malloc(sizeof(pen));
        pen *p2 = malloc(sizeof(pen));
        pen *p3 = malloc(sizeof(pen));
        pen *p4 = malloc(sizeof(pen));
        start->x = p->x;
        start->y = p->y;

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        switch (type) {
            case A:
                ta[0] = malloc(sizeof(thread_data));
                set_thread_data(ta[0], hil, p, order - 1, D);
                pthread_create(&thread[0], &attr, do_hilbert, (void *)ta[0]);

                // 1st worker thread
                move_absolute(p2, start->x - hilbert_size(order - 1), start->y);
                mark(hil, p2);
                move(p2, LEFT);
                ta[1] = malloc(sizeof(thread_data));
                set_thread_data(ta[1], hil, p2, order - 1, A);
                pthread_create(&thread[1], &attr, do_hilbert, (void *)ta[1]);

                // 2nd worker thread
                move_absolute(p3, start->x - hilbert_size(order - 1) - 1, start->y + hilbert_size(order - 1));
                mark(hil, p3);
                move(p3, DOWN);
                ta[2] = malloc(sizeof(thread_data));
                set_thread_data(ta[2], hil, p3, order - 1, A);
                pthread_create(&thread[2], &attr, do_hilbert, (void *)ta[2]);

                // 3rd worker thread
                move_absolute(p4, start->x - hilbert_size(order - 1), start->y + 2 * hilbert_size(order - 1));
                mark(hil, p4);
                move(p4, RIGHT);
                ta[3] = malloc(sizeof(thread_data));
                set_thread_data(ta[3], hil, p4, order - 1, B);
                pthread_create(&thread[3], &attr, do_hilbert, (void *)ta[3]);
                
                pthread_join(thread[0], NULL);
                pthread_join(thread[1], NULL);
                pthread_join(thread[2], NULL);
                pthread_join(thread[3], NULL);
                break;
            case B:
                ta[0] = malloc(sizeof(thread_data));
                set_thread_data(ta[0], hil, p, order - 1, C);
                pthread_create(&thread[0], &attr, do_hilbert, (void *)ta[0]);

                // 1st worker thread
                move_absolute(p2, start->x, start->y - hilbert_size(order - 1));
                mark(hil, p2);
                move(p2, UP);
                ta[1] = malloc(sizeof(thread_data));
                set_thread_data(ta[1], hil, p2, order - 1, B);
                pthread_create(&thread[1], &attr, do_hilbert, (void *)ta[1]);

                // 2nd worker thread
                move_absolute(p3, start->x + hilbert_size(order - 1), start->y - hilbert_size(order - 1) - 1);
                mark(hil, p3);
                move(p3, RIGHT);
                ta[2] = malloc(sizeof(thread_data));
                set_thread_data(ta[2], hil, p3, order - 1, B);
                pthread_create(&thread[2], &attr, do_hilbert, (void *)ta[2]);

                // 3rd worker thread
                move_absolute(p4, start->x + 2 * hilbert_size(order - 1), start->y - hilbert_size(order - 1));
                mark(hil, p4);
                move(p4, DOWN);
                ta[3] = malloc(sizeof(thread_data));
                set_thread_data(ta[3], hil, p4, order - 1, A);
                pthread_create(&thread[3], &attr, do_hilbert, (void *)ta[3]);
                
                pthread_join(thread[0], NULL);
                pthread_join(thread[1], NULL);
                pthread_join(thread[2], NULL);
                pthread_join(thread[3], NULL);
                break;
            case C:
                ta[0] = malloc(sizeof(thread_data));
                set_thread_data(ta[0], hil, p, order - 1, B);
                pthread_create(&thread[0], &attr, do_hilbert, (void *)ta[0]);

                // 1st worker thread
                move_absolute(p2, start->x + hilbert_size(order - 1), start->y);
                mark(hil, p2);
                move(p2, RIGHT);
                ta[1] = malloc(sizeof(thread_data));
                set_thread_data(ta[1], hil, p2, order - 1, C);
                pthread_create(&thread[1], &attr, do_hilbert, (void *)ta[1]);

                // 2nd worker thread
                move_absolute(p3, start->x + hilbert_size(order - 1) + 1, start->y - hilbert_size(order - 1));
                mark(hil, p3);
                move(p3, UP);
                ta[2] = malloc(sizeof(thread_data));
                set_thread_data(ta[2], hil, p3, order - 1, C);
                pthread_create(&thread[2], &attr, do_hilbert, (void *)ta[2]);

                // 3rd worker thread
                move_absolute(p4, start->x + hilbert_size(order - 1), start->y - 2 * hilbert_size(order - 1));
                mark(hil, p4);
                move(p4, LEFT);
                ta[3] = malloc(sizeof(thread_data));
                set_thread_data(ta[3], hil, p4, order - 1, D);
                pthread_create(&thread[3], &attr, do_hilbert, (void *)ta[3]);
                
                pthread_join(thread[0], NULL);
                pthread_join(thread[1], NULL);
                pthread_join(thread[2], NULL);
                pthread_join(thread[3], NULL);
                break;
            case D:
                ta[0] = malloc(sizeof(thread_data));
                set_thread_data(ta[0], hil, p, order - 1, A);
                pthread_create(&thread[0], &attr, do_hilbert, (void *)ta[0]);

                // 1st worker thread
                move_absolute(p2, start->x, start->y + hilbert_size(order - 1));
                mark(hil, p2);
                move(p2, DOWN);
                ta[1] = malloc(sizeof(thread_data));
                set_thread_data(ta[1], hil, p2, order - 1, D);
                pthread_create(&thread[1], &attr, do_hilbert, (void *)ta[1]);

                // 2nd worker thread
                move_absolute(p3, start->x - hilbert_size(order - 1), start->y + hilbert_size(order - 1) + 1);
                mark(hil, p3);
                move(p3, LEFT);
                ta[2] = malloc(sizeof(thread_data));
                set_thread_data(ta[2], hil, p3, order - 1, D);
                pthread_create(&thread[2], &attr, do_hilbert, (void *)ta[2]);

                // 3rd worker thread
                move_absolute(p4, start->x - 2 * hilbert_size(order - 1), start->y + hilbert_size(order - 1));
                mark(hil, p4);
                move(p4, UP);
                ta[3] = malloc(sizeof(thread_data));
                set_thread_data(ta[3], hil, p4, order - 1, C);
                pthread_create(&thread[3], &attr, do_hilbert, (void *)ta[3]);
                
                pthread_join(thread[0], NULL);
                pthread_join(thread[1], NULL);
                pthread_join(thread[2], NULL);
                pthread_join(thread[3], NULL);
                break;
            default:
                fprintf(stderr, "Unknown hilbert type: %d", type);
                break;
        } // switch
        free(start);
        free(p2);
        free(p3);
        free(p4);
    } // else
    pthread_exit(NULL);
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

void start(hilbert *hil, pen *p, int type)
{
    if(type == B || type == C) { // Bottom Left
        p->x = 0;
        p->y = hil->size - 1;
    }
    else if(type == A || type == D) { // Top Right
        p->x = hil->size - 1;
        p->y = 0;
    }
    else {
        fprintf(stderr, "Unknown type: %d", type);
    }
    return;
}

void mark(hilbert *hil, pen *p)
{
    assert(p->x < hil->size);
    assert(p->y < hil->size);
    assert(p->x >= 0);
    assert(p->y >= 0);
    hil->buffer[p->x][p->y] = '*';
    return;
}
