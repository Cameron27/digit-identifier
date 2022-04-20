#include <image.h>
#include <neural_net.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (argc < 2)
    {
        printf("usage: num-identifier <train | test> <images> <labels>\n");
        return 1;
    }

    Dataset *dataset = image_load(argv[1], argv[2]);

    Neural_Net *network;
    int arr[4] = {dataset->images[0].size, 16, 16, 10};
    network = malloc(sizeof(Neural_Net));
    *network = network_malloc(4, (int *)&arr);

    network_initialize(network, 1);

    network_train(network, dataset, 5, 20, 0.1);

    // Free values
    dataset_free_p(dataset);
    network_free_p(network);

    return 0;
}
