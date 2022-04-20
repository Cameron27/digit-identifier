#ifndef IMAGE_INCLUDE
#define IMAGE_INCLUDE

#include <stdint.h>

typedef struct
{
    int size;
    double *data;
    int label;
} Image;

typedef struct
{
    int count;
    Image *images;
} Dataset;

void image_free(Image i);

void image_free_p(Image *i);

void dataset_free(Dataset d);

void dataset_free_p(Dataset *d);

Dataset *image_load(const char *image_file, const char *label_file);

void image_randomize_order(Dataset *dataset);

Dataset *dataset_subset(Dataset *dataset, int offset, int count);

#endif