#include <image.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

/**
 * @brief Frees memory used by an image.
 *
 * @param i image to free memory of.
 */
void image_free(Image i)
{
    free(i.data);
}

/**
 * @brief Frees memory used by a image and frees the dataset itself.
 *
 * @param i image to free memory of.
 */
void image_free_p(Image *i)
{
    image_free(*i);
    free(i);
}

/**
 * @brief Frees memory used by a dataset.
 *
 * @param d dataset to free memory of.
 */
void dataset_free(Dataset d)
{
    for (int i = 0; i < d.count; i++)
        image_free(d.images[i]);
    free(d.images);
}

/**
 * @brief Frees memory used by a dataset and frees the dataset itself.
 *
 * @param d dataset to free memory of.
 */
void dataset_free_p(Dataset *d)
{
    dataset_free(*d);
    free(d);
}

/**
 * @brief Create a new dataset from files.
 *
 * @param image_file file that the image data is stored in.
 * @param label_file file that the label data is stored in.
 * @return a new dataset with the data from the specifed files.
 */
Dataset *image_load(const char *image_file, const char *label_file)
{
    FILE *img_f, *lbl_f;

    // Open files
    img_f = fopen(image_file, "r");
    lbl_f = fopen(label_file, "r");

    // Skip first 4 bytes
    fseek(img_f, 4, SEEK_SET);
    fseek(lbl_f, 4, SEEK_SET);

    // Read meta data
    int32_t count, count2, rows, cols, size;
    fread(&count, sizeof(int32_t), 1, img_f);
    fread(&count2, sizeof(int32_t), 1, lbl_f);
    fread(&rows, sizeof(int32_t), 1, img_f);
    fread(&cols, sizeof(int32_t), 1, img_f);
    count = htonl(count);
    count2 = htonl(count2);
    rows = htonl(rows);
    cols = htonl(cols);
    size = rows * cols;

    // Check counts match
    if (count != count2)
    {
        fclose(img_f);
        fclose(lbl_f);
        return 0;
    }

    // Load images data and lables
    Image *images = malloc(sizeof(Image) * count);
    uint8_t *buffer = malloc(sizeof(uint8_t) * size);
    for (int i = 0; i < count; i++)
    {
        images[i].size = size;
        fread(buffer, sizeof(uint8_t), size, img_f);
        images[i].data = malloc(sizeof(double) * size);
        for (int j = 0; j < size; j++)
            images[i].data[j] = buffer[j] / 255.0;

        images[i].label = 0;
        fread(&images[i].label, sizeof(uint8_t), 1, lbl_f);
    }
    free(buffer);

    fclose(img_f);
    fclose(lbl_f);

    Dataset *res = malloc(sizeof(Dataset));
    res->images = images;
    res->count = count;

    return res;
}

/**
 * @brief Randomize the order of images in a dataset.
 *
 * @param dataset dataset to randomize the order of.
 */
void image_randomize_order(Dataset *dataset)
{
    for (int i = dataset->count - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Image temp = dataset->images[i];
        dataset->images[i] = dataset->images[j];
        dataset->images[j] = temp;
    }
}

/**
 * @brief Create a new dataset that is a subset of another dataset.
 *
 * @param dataset dataset to take a subset of.
 * @param offset offset into the dataset to make the subset from.
 * @param count number of items to be in the subset.
 * @return a subset of the dataset.
 */
Dataset *dataset_subset(Dataset *dataset, int offset, int count)
{
    Dataset *res = malloc(sizeof(Dataset));
    res->count = count;
    res->images = (dataset->images) + offset;

    return res;
}