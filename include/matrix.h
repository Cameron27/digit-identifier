#ifndef MATRIX_INCLUDE
#define MATRIX_INCLUDE

typedef struct
{
    int width;
    int height;
    double *values;
} Matrix;

Matrix matrix_malloc(int width, int height);

void matrix_free(Matrix m);

void matrix_free_p(Matrix *m);

char *matrix_to_string(const Matrix m);

void matrix_fill_normal(Matrix *m, double std_dev);

#endif