#include "malloc.h"

int main(void)
{
    void *n = malloc(1);
    void *n2 = malloc(2);

	// large block
    void *n3 = malloc(50);

	void *n4 = malloc(4);
    show_alloc_mem();
    free(n4);
    free(n3);
    free(n2);

	// double free
    free(n);
	free(n);

	show_alloc_mem();
}