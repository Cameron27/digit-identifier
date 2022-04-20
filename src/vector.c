#include <vector.h>
#include <stdlib.h>
#include <random.h>
#include <math.h>
#include <math_ext.h>
#include <string.h>
#include <string_ext.h>
#include <stdio.h>

/**
 * @brief Creates a vector that represents an error.
 *
 * @return a vector that represents an error.
 */
Vector vector_error()
{
    Vector err;
    err.size = -1;
    err.values = 0;
}

/**
 * @brief Allocates memory for a new vector.
 *
 * @param size size of the vector.
 * @return a new vector with memory allocated for the values.
 */
Vector vector_malloc(int size)
{
    if (size <= 0)
    {
        return vector_error();
    }

    Vector new;

    new.size = size;
    new.values = malloc(sizeof(double) * size);

    return new;
}

/**
 * @brief Allocates memory for a new vector, with all values set to 0.
 *
 * @param size size of the vector.
 * @return a new vector with memory allocated for the values.
 */
Vector vector_calloc(int size)
{
    if (size <= 0)
    {
        return vector_error();
    }

    Vector new;

    new.size = size;
    new.values = calloc(size, sizeof(double));

    return new;
}

/**
 * @brief Frees memory used by a vector.
 *
 * @param v vector to free memory of.
 */
void vector_free(Vector v)
{
    free(v.values);
}

/**
 * @brief Frees memory used by a vector and frees the vector itself.
 *
 * @param v vector to free memory of.
 */
void vector_free_p(Vector *v)
{
    vector_free(*v);
    free(v);
}

/**
 * @brief Converts a vector to a string.
 *
 * @param v vector to convert to a string.
 * @return the resulting c string.
 */
char *vector_to_string(const Vector v)
{
    const int MAX_SIZE = 14;
    char **values = malloc(sizeof(char *) * v.size);
    int length_sum = 0;
    for (int i = 0; i < v.size; i++)
    {
        values[i] = malloc(sizeof(char) * MAX_SIZE);
        int length = snprintf(values[i], MAX_SIZE, "%g", v.values[i]);
        length_sum += MIN(MAX_SIZE - 1, length);
    }

    char *res = calloc(length_sum + v.size + 4, sizeof(char));
    strcat(res, "| ");
    strjoin(res, values, v.size, " ");
    strcat(res, " |");

    free(values);

    return res;
}

/**
 * @brief Fill a vector with values.
 *
 * @param v vector to fill.
 * @param values values to put in vector.
 */
void vector_fill(Vector *v, double *values)
{
    for (int i = 0; i < v->size; i++)
        v->values[i] = values[i];
}

/**
 * @brief Fill a vector with 0 value.
 *
 * @param v vector ot fill.
 */
void vector_fill_zero(Vector *v)
{
    for (int i = 0; i < v->size; i++)
        v->values[i] = 0;
}

/**
 * @brief Initialize a vector with random values sampled from a normal distribution;
 *
 * @param v vector to initialize.
 * @param std_dev standard deviation of normal distribution to sample.
 */
void vector_fill_normal(Vector *v, double std_dev)
{
    for (int i = 0; i < v->size; i++)
        v->values[i] = rnd_normal(std_dev);
}

/**
 * @brief Copy data from one vector to another.
 *
 * @param dest vector to copy data from.
 * @param src vector to copy data to.
 * @return vector result.
 */
Vector *vector_copy(Vector *dest, Vector *src)
{
    if (dest->size != src->size)
    {
        *dest = vector_error();
        return dest;
    }

    for (int i = 0; i < src->size; i++)
        dest->values[i] = src->values[i];

    return dest;
}

/**
 * @brief Copy data from one vector to another.
 *
 * @param dest vector to copy data from.
 * @param src vector to copy data to.
 * @param offset location to start copying to.
 * @return vector result.
 */
Vector *vector_ncopy(Vector *dest, Vector *src, int offset)
{
    if (dest->size - offset < src->size)
    {
        *dest = vector_error();
        return dest;
    }

    for (int i = 0; i < src->size; i++)
        dest->values[offset + i] = src->values[i];

    return dest;
}

/**
 * @brief Add to vectors together.
 *
 * @param dest first vector to add and vector to store result in.
 * @param v second vector to add.
 * @return vector result.
 */
Vector *vector_add(Vector *dest, Vector *v)
{
    if (dest->size != v->size)
    {
        *dest = vector_error();
        return dest;
    }

    for (int i = 0; i < dest->size; i++)
        dest->values[i] += v->values[i];

    return dest;
}

/**
 * @brief Calculate the magnitude of a vector.
 *
 * @param v vector to calculate the magnitude of.
 * @return magnitude of the vector.
 */
double vector_magnitude(Vector *v)
{
    double sum = 0;
    for (int i = 0; i < v->size; i++)
        sum += v->values[i] * v->values[i];

    return sqrt(sum);
}

/**
 * @brief Scales the values of a vector so they sum 1.
 *
 * @param v vector to scale.
 * @return scaled vector.
 */
Vector *vector_sum_1(Vector *v)
{
    double sum = 0;
    for (int i = 0; i < v->size; i++)
    {
        sum += v->values[i];
    }

    for (int i = 0; i < v->size; i++)
    {
        v->values[i] /= sum;
    }

    return v;
}

/**
 * @brief Calculate the sum of the squared difference between each entry in two vectors.
 *
 * @param a first vector.
 * @param b second vector.
 * @return squared difference between each entry in two vectors
 */
double vector_sq_diff_sum(Vector *a, Vector *b)
{
    if (a->size != a->size)
        return -1;

    double sum = 0;
    for (int i = 0; i < a->size; i++)
    {
        double diff = a->values[i] - b->values[i];
        sum += diff * diff;
    }

    return sum;
}

/**
 * @brief Find the index of the greatest value in the vector.
 *
 * @param v vector to search.
 * @return index of greatest value.
 */
int vector_max_index(Vector *v)
{
    int index = 0;
    double max = v->values[0];
    for (int i = 1; i < v->size; i++)
    {
        if (v->values[i] > max)
        {
            max = v->values[i];
            index = i;
        }
    }

    return index;
}

/**
 * @brief Multiply a vector by a matrix.
 *
 * @param dest vector to store result in.
 * @param v vector to multiply.
 * @param m matrix to multiply.
 * @return vector result.
 */
Vector *vector_matrix_mult(Vector *dest, Vector *v, Matrix *m)
{
    if (m->width != v->size || m->height != dest->size)
    {
        *dest = vector_error();
        return dest;
    }

    for (int i = 0; i < m->height; i++)
    {
        dest->values[i] = 0;
        for (int j = 0; j < m->width; j++)
        {
            dest->values[i] += m->values[i * m->width + j] * v->values[j];
        }
    }

    return dest;
}

/**
 * @brief Apply the sigmoid function to all the values in a vector.
 *
 * @param v vector to apply sigmoid function to.
 * @return vector result.
 */
Vector *vector_sigmoid(Vector *v)
{
    for (int i = 0; i < v->size; i++)
        v->values[i] = sigmoid(v->values[i]);
}