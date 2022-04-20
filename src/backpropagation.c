#include <backpropagation.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

/*
Notation used:
    y_i = expected value index i
    c_i = cost index i
    a_i = node value index i
    z_i = raw node value index i
    w_ij = weight from index j to i
    b_i = bias index i
*/

/**
 * @brief Calculate derivative of cost with respect to the last node values.
 *
 * @param dc_da vector to store results in.
 * @param a actual result.
 * @param y expected result.
 */
void backprop_calc_init_dc_da(Vector *dc_da, Vector *a, Vector *y)
{
    // Vector *normalized = malloc(sizeof(Vector));
    // *normalized = vector_malloc(a->size);
    // vector_copy(normalized, a);
    // vector_sum_1(normalized);

    for (int i = 0; i < dc_da->size; i++)
    {
        float temp = (a->values[i] - y->values[i]);
        dc_da->values[i] = 2 * temp * temp;
        // dc_da->values[i] = -y->values[i] / normalized->values[i];
    }

    // vector_free_p(normalized);
}

/**
 * @brief Calculate derivative of the node values with respect to the raw node values.
 *
 * @param da_dz vector to store result in.
 * @param z raw node value.
 */
void backprop_calc_da_dz(Vector *da_dz, Vector *z)
{
    for (int i = 0; i < da_dz->size; i++)
    {
        double temp = exp(-z->values[i]);
        double temp2 = 1 + temp;
        da_dz->values[i] = temp / (temp2 * temp2);
    }
}

/**
 * @brief Calculate derivative of the costs with respect to the biases.
 *
 * @param dc_db vector to store the results in.
 * @param da_dz derivative of the node values with respect to the raw node values the weights go to,
 * @param dc_da derivative of cost with respect to the node values the weights go to.
 */
void backprop_calc_dc_db(Vector *dc_db, Vector *da_dz, Vector *dc_da)
{
    for (int i = 0; i < dc_db->size; i++)
        dc_db->values[i] = da_dz->values[i] * dc_da->values[i];
}

/**
 * @brief Calculate derivative of the costs with respect to the weights.
 *
 * @param dc_dw vector to store the results in.
 * @param a node values the weights come from.
 * @param da_dz derivative of the node values with respect to the raw node values the weights go to,
 * @param dc_da derivative of cost with respect to the node values the weights go to.
 */
void backprop_calc_dc_dw(Vector *dc_dw, Vector *a, Vector *da_dz, Vector *dc_da)
{
    for (int k = 0; k < dc_dw->size; k++)
    {
        int i = k / a->size; // Index of node weight goes to
        int j = k % a->size; // Index of node weight comes from

        dc_dw->values[k] = a->values[j] * da_dz->values[i] * dc_da->values[i];
    }
}

/**
 * @brief Calculate derivative of the costs with respect to the previous node values.
 *
 * @param dc_da_prev vector to store the results in.
 * @param w weights from previous node values to current node values.
 * @param da_dz derivative of the current node values with respect to the current raw node values,
 * @param dc_da derivative of cost with respect to the current node values.
 */
void backprop_calc_dc_da(Vector *dc_da_prev, Matrix *w, Vector *da_dz, Vector *dc_da)
{
    for (int j = 0; j < dc_da_prev->size; j++)
    {
        dc_da_prev->values[j] = 0;
        for (int i = 0; i < dc_da->size; i++)
            dc_da_prev->values[j] += w->values[i * w->width + j] * da_dz->values[i] * dc_da->values[i];
    }
}

/**
 * @brief Perform backpropagation to calculate the gradient of the network for an input.
 *
 * @param gradient vector to store the result in.
 * @param network network that backpropagation is being performed on.
 * @param raw_node_values raw node values for an input.
 * @param node_values node values for an input.
 * @return vector result. The vector starts with the gradient for the final set of weights, then the final set of biases
 * and continues alternating weight and biases from the end of the network to the start.
 */
Vector *backprop_calc_grad(Vector *gradient, Neural_Net *network, Vector *raw_node_values, Vector *node_values, Vector *expected_result)
{
    // Create all needed vectors
    Vector *dc_da = malloc(sizeof(Vector));
    *dc_da = vector_malloc(expected_result->size);
    Vector *da_dz = malloc(sizeof(Vector));
    Vector *dc_dw = malloc(sizeof(Vector));
    Vector *dc_db = malloc(sizeof(Vector));
    Vector *dc_da_prev = malloc(sizeof(Vector));

    int l = network->layers - 1;
    backprop_calc_init_dc_da(dc_da, node_values + l, expected_result);

    int index = 0;
    for (; l >= 0; l--)
    {
        *da_dz = vector_malloc(raw_node_values[l].size);
        backprop_calc_da_dz(da_dz, raw_node_values + l);

        *dc_db = vector_malloc(network->biases[l].size);
        backprop_calc_dc_db(dc_db, da_dz, dc_da);

        // TODO: optimize to use bias derivatives
        *dc_dw = vector_malloc(network->weights[l].width * network->weights[l].height);
        backprop_calc_dc_dw(dc_dw, node_values + (l - 1), da_dz, dc_da);

        // Do not calculate the next dc_da if on the first layer
        if (l != 0)
        {
            *dc_da_prev = vector_malloc(node_values[l - 1].size);
            backprop_calc_dc_da(dc_da_prev, network->weights + l, da_dz, dc_da);
            // Update dc_da
            vector_free(*dc_da);
            *dc_da = *dc_da_prev;
        }

        // Copy values to gradient
        vector_ncopy(gradient, dc_dw, index);
        index += dc_dw->size;
        vector_ncopy(gradient, dc_db, index);
        index += dc_db->size;

        // Free values
        vector_free(*da_dz);
        vector_free(*dc_db);
        vector_free(*dc_dw);
    }

    // Free values
    vector_free_p(dc_da);
    free(da_dz);
    free(dc_dw);
    free(dc_db);
    free(dc_da_prev);

    return gradient;
}