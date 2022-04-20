#include <stdlib.h>
#include <math.h>

double rnd_double()
{
    double res = (double)rand() / RAND_MAX;
    return res;
}

double rnd_normal(double std_dev)
{
    static int res2_set = 0;
    static double res2;

    if (res2_set)
    {
        res2_set = 0;
        return res2 * std_dev;
    }

    double a = rnd_double();
    double b = rnd_double();

    double r = sqrt(-2 * log(a));
    double t = 2 * M_PI * b;
    double res1 = r * cos(t);
    res2 = r * sin(t);

    res2_set = 1;

    return res1 * std_dev;
}
