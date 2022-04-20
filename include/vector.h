#ifndef VECTOR_INCLUDE
#define VECTOR_INCLUDE

#include <matrix.h>

typedef struct
{
    int size;
    double *values;
} Vector;

Vector vector_malloc(int);

Vector vector_calloc(int);

void vector_free(Vector v);

void vector_free_p(Vector *v);

char *vector_to_string(const Vector v);

void vector_fill(Vector *v, double *values);

void vector_fill_zero(Vector *v);

void vector_fill_normal(Vector *v, double std_dev);

Vector *vector_copy(Vector *dest, Vector *src);

Vector *vector_ncopy(Vector *dest, Vector *src, int offset);

Vector *vector_add(Vector *dest, Vector *v);

double vector_magnitude(Vector *v);

Vector *vector_sum_1(Vector *v);

double vector_sq_diff_sum(Vector *a, Vector *b);

int vector_max_index(Vector *v);

Vector *vector_matrix_mult(Vector *dest, Vector *v, Matrix *m);

Vector *vector_sigmoid(Vector *v);

#endif