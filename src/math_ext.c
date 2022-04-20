#include <math.h>

/**
 * @brief Apply sigmoid function to a value.
 *
 * @param x value to apply function to.
 * @return result of function.
 */
double sigmoid(double x)
{
    double e = exp(-x);

    return 1 / (1 + e);
}