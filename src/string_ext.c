#include <string.h>
#include <string_ext.h>

char *strjoin(char *dest, char **strings, int count, char *join)
{
    for (int i = 0; i < count; i++)
    {
        strcat(dest, strings[i]);
        if (i < count - 1)
            strcat(dest, join);
    }

    return dest;
}