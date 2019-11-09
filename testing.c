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

int	check(int availables[], int sizes[])
{
	struct metadata *block = (struct metadata *)memory_start;

	int i = -1;

    while (1)
    {
		++i;

        void *end = (char *)block + sizeof(struct metadata) + block->size;
		// TODO: use END
        struct metadata *end_meta = (struct metadata *)end;

		if (block->available != end_meta->available
			|| block->size != end_meta->size
			|| block->size != sizes[i]
			|| block->available != availables[i])
		return (0);

        void *new_block = NEXT(block);

        if (block == last_valid_address)
            break ;
        else
            block = (struct metadata *)new_block;
    }
	return (1);
}

void test_0(void) /* simple allocation */
{
    size_t total_mem = 100;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup(buf, total_mem);

    myalloc(10);
	myalloc(11);
	myalloc(12);
	myalloc(13);
	myalloc(3);

	int availables[] =	{0,  0,  0,  0};
	int sizes[] =		{10, 11, 12, 3};
	if (check(availables, sizes))
		printf("Test 0 OK\n");
	else
	{
		printf("Test 0 ERROR\n");
		dump();
		dump_visual();
		mem_dump(buf, total_mem);
	}

    free(buf);
}

void test_1(void) /* too big allocation */
{
    size_t total_mem = 100;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup(buf, total_mem);

    void *a = myalloc(100); // too much
	myalloc(1);

	int availables[] =	{0, 1};
	int sizes[] =		{1, 67};
	if (check(availables, sizes)
		&& a == NULL)
		printf("Test 1 OK\n");
	else
	{
		printf("Test 1 ERROR\n");
		dump();
		dump_visual();
		mem_dump(buf, total_mem);
	}

    free(buf);
}

void test_2(void) /* too big allocation */
{
    size_t total_mem = 100;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup(buf, total_mem);

	myalloc(1);
    void *a = myalloc(100); // too much
	myalloc(2);

	int availables[] =	{0, 0, 1};
	int sizes[] =		{1, 2, 49};
	if (check(availables, sizes)
		&& a == NULL)
		printf("Test 2 OK\n");
	else
	{
		printf("Test 2 ERROR\n");
		dump();
		dump_visual();
		mem_dump(buf, total_mem);
	}

    free(buf);
}

void test_3(void) /* space left only for meta */
{
    size_t total_mem = 100;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup(buf, total_mem);

	myalloc(84);
	void *a = myalloc(0); // no space for meta
	void *b = myalloc(1); // no space

	int availables[] =	{0};
	int sizes[] =		{84};
	if (check(availables, sizes)
		&& a == NULL
		&& b == NULL)
		printf("Test 3 OK\n");
	else
	{
		printf("Test 3 ERROR\n");
		dump();
		dump_visual();
		mem_dump(buf, total_mem);
	}

    free(buf);
}

void test_4(void) /* right block eating */
{
    size_t total_mem = 100;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup(buf, total_mem);

	void *a = myalloc(20);
	void *b = myalloc(20);
	myfree(b);
	myfree(a);

	int availables[] =	{1};
	int sizes[] =		{84};
	if (check(availables, sizes))
		printf("Test 4 OK\n");
	else
	{
		printf("Test 4 ERROR\n");
		dump();
		dump_visual();
		mem_dump(buf, total_mem);
	}

    free(buf);
}

void test_5(void) /* left block eating */
{
    size_t total_mem = 100;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup(buf, total_mem);

	void *a = myalloc(20);
	void *b = myalloc(20);
	void *c = myalloc(12);
	myfree(a);
	myfree(b);
	myfree(c);

	int availables[] =	{1};
	int sizes[] =		{84};
	if (check(availables, sizes))
		printf("Test 5 OK\n");
	else
	{
		printf("Test 5 ERROR\n");
		dump();
		dump_visual();
		mem_dump(buf, total_mem);
	}

    free(buf);
}

void test_6(void) /* several retries */
{
    size_t total_mem = 100;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup(buf, total_mem);

	void *a;
	a = myalloc(20);
	myfree(a);
	a = myalloc(20);
	myfree(a);
	a = myalloc(20);
	myfree(a);
	a = myalloc(20);
	myfree(a);
	a = myalloc(20);
	myfree(a);

	int availables[] =	{1};
	int sizes[] =		{84};
	if (check(availables, sizes))
		printf("Test 6 OK\n");
	else
	{
		printf("Test 6 ERROR\n");
		dump();
		dump_visual();
		mem_dump(buf, total_mem);
	}

    free(buf);
}

void test_7(void) /* fragmentation */
{
    size_t total_mem = 100;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup(buf, total_mem);

	myalloc(1);
	myalloc(1);
	myalloc(1);
	myalloc(1);
	myalloc(1);

	int availables[] =	{0, 0, 0, 0, 0 };
	int sizes[] =		{1, 1, 1, 1, 16};
	if (check(availables, sizes))
		printf("Test 7 OK\n");
	else
	{
		printf("Test 7 ERROR\n");
		dump();
		dump_visual();
		mem_dump(buf, total_mem);
	}

    free(buf);
}

void testing(void)
{
	test_0();
    test_1();
	test_2();
	test_3();
	test_4();
	test_5();
	test_6();
	test_7();
}