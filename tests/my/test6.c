#include "malloc.h"

int main(void)
{
    void *n = malloc(1);
    void *n2 = malloc(2);
    void *n3 = malloc(50);
	void *n4 = malloc(4);

    show_alloc_mem();
    show_alloc_mem_ex();

    free(n4);
    free(n3);
    free(n2);
    free(n);
	free(n);

    void *n5 = malloc(10);
    // free(n5);

	show_alloc_mem();
	show_alloc_mem_ex();

}