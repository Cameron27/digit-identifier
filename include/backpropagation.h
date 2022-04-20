#ifndef BACKPROPAGATION_INCLUDE
#define BACKPROPAGATION_INCLUDE

#include <neural_net.h>
#include <vector.h>

Vector *backprop_calc_grad(Vector *single_gradient, Neural_Net *network, Vector *raw_node_values, Vector *node_values, Vector *expected_result);

#endif