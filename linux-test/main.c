#include <stdlib.h>
#include <stdio.h>

extern void *malloc(size_t s);

int main(void)
{
    void *ptr = malloc(20);
    printf("%p\n", ptr);
    return 0;
}
