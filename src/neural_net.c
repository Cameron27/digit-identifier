#include <neural_net.h>
#include <stdlib.h>
#include <stdio.h>
#include <backpropagation.h>

/**
 * @brief Creates a network that represents an error.
 *
 * @return a network that represents an error.
 */
Neural_Net network_error()
{
    Neural_Net error;
    error.layers = -1;
    error.total_values = -1;
    error.weights = 0;
    error.biases = 0;

    return error;
}

/**
 * @brief Allocates memory for a new network.
 *
 * @param layers number of layers in the network.
 * @param neurons_per_layer an array with the number of neurons that are in each layer.
 * @return a new network with memory allocated for the weights and biases.
 */
Neural_Net network_malloc(int layers, int *neurons_per_layer)
{
    if (layers <= 0)
        return network_error();

    Neural_Net new;
    new.layers = layers - 1;
    new.total_values = 0;
    new.weights = malloc(sizeof(Matrix) * (layers - 1));
    new.biases = malloc(sizeof(Vector) * (layers - 1));

    // Create weight matrices and bias vectors
    for (int i = 0; i < layers - 1; i++)
    {
        new.weights[i] = matrix_malloc(neurons_per_layer[i], neurons_per_layer[i + 1]);
        new.total_values += neurons_per_layer[i] * neurons_per_layer[i + 1];
        new.biases[i] = vector_malloc(neurons_per_layer[i + 1]);
        new.total_values += neurons_per_layer[i + 1];
    }

    return new;
}

/**
 * @brief Frees memory used by a network.
 *
 * @param n network to free memory of.
 */
void network_free(Neural_Net n)
{
    for (int i = 0; i < n.layers; i++)
    {
        matrix_free(n.weights[i]);
        vector_free(n.biases[i]);
    }
    free(n.weights);
    free(n.biases);
}

/**
 * @brief Frees memory used by a network and frees the network itself.
 *
 * @param n network to free memory of.
 */
void network_free_p(Neural_Net *n)
{
    network_free(*n);
    free(n);
}

/**
 * @brief Initialize a network with random values sampled from a normal distribution;
 *
 * @param network network to initialize.
 * @param std_dev standard deviation of normal distribution to sample.
 */
void network_initialize(Neural_Net *network, double std_dev)
{
    for (int i = 0; i < network->layers; i++)
    {
        matrix_fill_normal((network->weights) + i, std_dev);
        vector_fill_normal((network->biases) + i, std_dev);
    }
}

/**
 * @brief Run the network in some input and store the values of all the nodes.
 *
 * @param raw_node_values place to store all raw node values (i.e. value before sigmoid is applied).
 * @param node_values place to store all node values.
 * @param network network to run.
 * @param input input to the network.
 */
void network_run(Vector *raw_node_values, Vector *node_values, Neural_Net *network, Vector *input)
{
    Vector *active_layer = input;

    for (int i = 0; i < network->layers; i++)
    {
        vector_matrix_mult(raw_node_values + i, active_layer, (network->weights) + i);
        vector_add(raw_node_values + i, (network->biases) + i);

        vector_copy(node_values + i, raw_node_values + i);
        vector_sigmoid(node_values + i);

        active_layer = node_values + i;
    }

    // printf("%i : %s\n\n", input->size, vector_to_string(*input));
    // for (int i = 0; i < network->layers; i++)
    // {
    //     printf("%i : %s\n\n", raw_node_values[i].size, vector_to_string(raw_node_values[i]));
    //     printf("%i : %s\n\n", node_values[i].size, vector_to_string(node_values[i]));
    // }
}

/**
 * @brief Adjust all the values in a network by moving down the gradient.
 *
 * @param network network to adjust.
 * @param gradient gradient to move along.
 * @param step_size value to multiple gradient by when moving.
 */
void network_adjust(Neural_Net *network, Vector *gradient, double step_size)
{
    int index = 0;
    for (int i = network->layers - 1; i >= 0; i--)
    {
        for (int j = 0; j < network->weights[i].width * network->weights[i].height; j++)
            network->weights[i].values[j] -= gradient->values[index + j] * step_size;

        index += network->weights[i].width * network->weights[i].height;

        for (int j = 0; j < network->biases[i].size; j++)
            network->biases[i].values[j] -= gradient->values[index + j] * step_size;

        index += network->biases[i].size;
    }
}

/**
 * @brief Perform an optimization step on a network with a dataset.
 *
 * @param network network to optimize.
 * @param dataset dataset to optimize for.
 * @param step_size value to multiple gradient by when moving.
 * @return total cost from when the network was run.
 */
double network_optimize(Neural_Net *network, Dataset *dataset, double step_size)
{
    // Create network input vector
    Vector *input = malloc(sizeof(Vector));
    *input = vector_malloc(dataset->images->size);

    // Create network output vectors
    Vector *raw_node_values = malloc(sizeof(Vector) * network->layers);
    Vector *node_values = malloc(sizeof(Vector) * (network->layers + 1));
    node_values = node_values + 1; // Offset so that it lines up with raw node values (as node values needs to contain
                                   // the input while there is no raw node values equivalent)
    for (int i = 0; i < network->layers; i++)
    {
        raw_node_values[i] = vector_malloc(network->biases[i].size);
        node_values[i] = vector_malloc(network->biases[i].size);
        i = i;
    }

    // Create backpropagation input vector
    Vector *expected_result = malloc(sizeof(Vector));
    *expected_result = vector_malloc(network->biases[network->layers - 1].size);

    // Create backpropagation output vetors
    Vector *single_gradient = malloc(sizeof(Vector));
    Vector *sum_gradient = malloc(sizeof(Vector));
    *single_gradient = vector_malloc(network->total_values);
    *sum_gradient = vector_calloc(network->total_values);

    double cost = 0;
    int correct_guesses = 0;
    // Run network and perform back propagation on each item
    for (int i = 0; i < dataset->count; i++)
    {
        // TODO: determain which alternative is better
        vector_fill(input, dataset->images[i].data);
        // input->values = dataset->images[i].data;

        node_values[-1] = *input;
        network_run(raw_node_values, node_values, network, input);

        vector_fill_zero(expected_result);
        expected_result->values[dataset->images[i].label] = 1;

        correct_guesses += dataset->images[i].label == vector_max_index(node_values + (network->layers - 1));

        cost += vector_sq_diff_sum(expected_result, node_values + (network->layers - 1));

        backprop_calc_grad(single_gradient, network, raw_node_values, node_values, expected_result);

        vector_add(sum_gradient, single_gradient);
    }

    printf("\nGradient magnitude: %f\n", vector_magnitude(sum_gradient));
    printf("Total cost: %f\n", cost);
    printf("Corrent guesses: %i out of %i\n", correct_guesses, dataset->count);
    network_adjust(network, sum_gradient, step_size / dataset->count);

    // Free values
    vector_free_p(input);
    for (int i = 0; i < network->layers; i++)
    {
        vector_free(raw_node_values[i]);
        vector_free(node_values[i]);
    }
    free(raw_node_values);
    free(node_values - 1);
    vector_free_p(expected_result);
    vector_free_p(single_gradient);
    vector_free_p(sum_gradient);

    return cost;
}

/**
 * @brief Train a neural network on a set of training data for a single iteration.
 *
 * @param network network to train.
 * @param dataset dataset to train with.
 * @param num_groups number of groups to split the training set into each iteration.
 * @param step_size value to multiple gradient by when moving.
 * @return sum of all cost for the iteration.
 */
double network_train_iteration(Neural_Net *network, Dataset *dataset, int num_groups, double step_size)
{
    const int SEGMENT_SIZE = dataset->count / num_groups;
    image_randomize_order(dataset);

    double cost = 0;
    for (int i = 0; i < dataset->count / SEGMENT_SIZE; i++)
    {
        Dataset *segment = dataset_subset(dataset, SEGMENT_SIZE * i, SEGMENT_SIZE);
        cost += network_optimize(network, segment, step_size);
        free(segment);
    }
}

/**
 * @brief Train a neural network on a set of training data.
 *
 * @param neural_net network to train.
 * @param dataset dataset to train with.
 * @param iterations number of iterations to perform for training.
 * @param num_groups number of groups to split the training set into each iteration.
 * @param step_size value to multiple gradient by when moving.
 */
void network_train(Neural_Net *network, Dataset *dataset, int iterations, int num_groups, double step_size)
{
    double prev_cost = 1.0 / 0.0;
    for (int i = 0; i < iterations; i++)
    {
        printf("\n--- Starting iteration %i of %i ---\n", i + 1, iterations);
        double cost = network_train_iteration(network, dataset, num_groups, step_size);
        if (cost > prev_cost * 0.9)
        {
            printf("\nHalving step size\n");
            step_size *= 0.5;
        }

        prev_cost = cost;
    }
}