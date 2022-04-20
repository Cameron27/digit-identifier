#include <matrix.h>
#include <stdlib.h>
#include <string.h>
#include <string_ext.h>
#include <stdio.h>
#include <math_ext.h>
#include <random.h>

/**
 * @brief Creates a matrix that represents an error.
 *
 * @return a matrix that represents an error.
 */
Matrix matrix_error()
{
    Matrix err;
    err.width = -1;
    err.height = -1;
    err.values = 0;
}

/**
 * @brief Allocates memory for a new matrix.
 *
 * @param width width of the matrix.
 * @param height height of the matrix.
 * @return a new matrix with memory allocated for the values.
 */
Matrix matrix_malloc(int width, int height)
{
    if (width <= 0 || height <= 0)
    {
        return matrix_error();
    }

    Matrix new;

    new.width = width;
    new.height = height;
    new.values = malloc(sizeof(double) * width * height);

    return new;
}

/**
 * @brief Frees memory used by a matrix.
 *
 * @param m matrix to free memory of.
 */
void matrix_free(Matrix m)
{
    free(m.values);
}

/**
 * @brief Frees memory used by a matrix and frees the matrix itself.
 *
 * @param m matrix to free memory of.
 */
void matrix_free_p(Matrix *m)
{
    matrix_free(*m);
    free(m);
}

/**
 * @brief Converts a row of a matrix to a string.
 *
 * @param out pointer to where the resulting c string should be stored.
 * @param m matrix with row to convert to string.
 * @param i row of the matrix to convert to a string.
 * @return the length of the string.
 */
int row_to_string(char **out, Matrix m, int i)
{
    const int MAX_SIZE = 14;
    char **values = malloc(sizeof(char *) * m.width);
    int length_sum = 0;
    for (int j = 0; j < m.width; j++)
    {
        values[j] = malloc(sizeof(char) * MAX_SIZE);
        int length = snprintf(values[j], MAX_SIZE, "%g", m.values[i * m.height + j]);
        length_sum += MIN(MAX_SIZE - 1, length);
    }

    char *row = calloc(length_sum + m.width + 4, sizeof(char));
    strcat(row, "| ");
    strjoin(row, values, m.width, " ");
    strcat(row, " |");

    free(values);

    *out = row;

    return length_sum + m.width + 3;
}

/**
 * @brief Converts a matrix to a string.
 *
 * @param m matrix to convert to a string.
 * @return the resulting c string.
 */
char *matrix_to_string(const Matrix m)
{
    char **rows = malloc(sizeof(char *) * m.height);
    int length_sum_rows = 0;
    for (int i = 0; i < m.height; i++)
        length_sum_rows += row_to_string(rows + i, m, i);

    char *res = calloc(length_sum_rows + m.height, sizeof(char));
    strjoin(res, rows, m.height, "\n");

    for (int i = 0; i < m.height; i++)
        free(rows[i]);
    free(rows);

    return res;
}

/**
 * @brief Initialize a matrix with random values sampled from a normal distribution;
 *
 * @param m matrix to initialize.
 * @param std_dev standard deviation of normal distribution to sample.
 */
void matrix_fill_normal(Matrix *m, double std_dev)
{
    for (int i = 0; i < m->width * m->height; i++)
        m->values[i] = rnd_normal(std_dev);
}
