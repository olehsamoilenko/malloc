#include "malloc.h"
#include <stdlib.h>

extern void *last_valid_address;
extern void *memory_start;

void dump_visual(void)
{
    int i = 0;
    struct metadata *block = (struct metadata *)memory_start;

    while (1)
    {
        int meta;
        
        meta = sizeof(struct metadata);
        while (meta--)
        {
            printf("       {{{ ");
            if (++i % 10 == 0) {
                printf("\n");
            }
        }
        int data = block->size;
        while (data--)
        {
            if (block->available)
                printf("         . ");
            else
                printf("         d ");

            if (++i % 10 == 0) {
                printf("\n");
            }
        }
        meta = sizeof(struct metadata);
        while (meta--)
        {
            printf("       }}} ");
            if (++i % 10 == 0) {
                printf("\n");
            }
        }

        if (block == last_valid_address)
            break ;
        else
        {
            void *new_block = NEXT(block);
            block = (struct metadata *)new_block;
        }
    }
    printf("\n");
}

void dump(void)
{
    struct metadata *block = (struct metadata *)memory_start;

    while (1)
    {
        void *end = (char *)block + sizeof(struct metadata) + block->size;
        struct metadata *end_meta = (struct metadata *)end;

        printf("%p=%lu (%u, %u)=(%u, %u)\n", block, (unsigned long)block, block->available, block->size, end_meta->available, end_meta->size);

        void *new_block = NEXT(block);

        if (block == last_valid_address)
            break ;
        else
            block = (struct metadata *)new_block;
    }

    printf("\n");
}

void mem_dump(void *buf, int len)
{
	char *mem = (char *)buf;
	int i = 0;
	while (i < len) {
		printf("%#10x ", mem[i]);
		i++;
		if (i % 10 == 0)
			printf("\n");
	}
}

void mem_clear(void *buf, int len)
{
	char *mem = (char *)buf;
	int i = 0;
	while (i++ < len) {
		mem[i] = 0;
	}
}

void mysetup(void *buf, unsigned long size)
{
    memory_start = buf;
    last_valid_address = buf;

    struct metadata *b0 = (struct metadata *)buf;
    b0->available = 1;
    b0->size = size - 2 * sizeof(struct metadata);

    void *end = (char *)b0 + sizeof(struct metadata) + b0->size;
    struct metadata *meta_end = (struct metadata *)end;
    meta_end->available = 1;
    meta_end->size = b0->size;
}

void test_1(void)
{
    size_t total_mem = 100;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup(buf, total_mem);

    myalloc(100);
    
    dump();
    dump_visual();
	mem_dump(buf, total_mem);

    free(buf);
}

void test_N(void)
{
    size_t total_mem = 100;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup(buf, total_mem);

    myalloc(1000);
    void *a = myalloc(10);
    void *b = myalloc(10);
    void *c = myalloc(10);
    void *d = myalloc(6);
    myalloc(10);
    myalloc(10);
    myalloc(10);
    myalloc(6);
    myalloc(1000);

	myfree(c);
	myfree(b);
	myfree(d);
	myfree(a);

    dump();
    dump_visual();
	mem_dump(buf, total_mem);

    free(buf);
}

// Failed. Runtime error
// myalloc returned out-of-range block
// Aborted (core dumped)

// Failed. Runtime error
// Segmentation fault (core dumped)

// Failed. Runtime error
// For heap size of 524288 bytes, max allocated size 262153 bytes is too small
// Aborted (core dumped)

// Failed. Runtime error
// Heap fragmentation (max) (alloc size = 16)
// Aborted (core dumped)

void testing(void)
{
    test_1();
    test_N();
}