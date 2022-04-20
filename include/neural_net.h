#ifndef NEURAL_NET_INCLUDE
#define NEURAL_NET_INCLUDE

#include <matrix.h>
#include <vector.h>
#include <image.h>

typedef struct
{
    int layers;
    int total_values;
    Matrix *weights;
    Vector *biases;
} Neural_Net;

Neural_Net network_malloc(int layers, int *neurons_per_layer);

void network_free(Neural_Net n);

void network_free_p(Neural_Net *n);

void network_initialize(Neural_Net *Neural_Net, double std_dev);

void network_train(Neural_Net *Neural_Net, Dataset *dataset, int iterations, int num_groups, double step_size);

#endif