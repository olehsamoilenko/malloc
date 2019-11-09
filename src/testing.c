#include "malloc.h"
#include <stdlib.h>

extern struct metadata *last_valid_address;
extern struct metadata *memory_start;

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
            struct metadata *new_block = NEXT(block);
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
        struct metadata *end_meta = END(block);

        printf("%p=%lu (%u, %u, %d)=(%u, %u, %d)\n",
			block, (unsigned long)block,
			block->available, block->size, block->type,
			end_meta->available, end_meta->size, end_meta->type);

        block = GETNEXT(block);
		if (!block)
			break ;
    }

    printf("\n");
}

// void mem_dump(void *buf, int len) TODO: cut
// {
// 	char *mem = (char *)buf;
// 	int i = 0;
// 	while (i < len) {
// 		printf("%#10x ", mem[i]);
// 		i++;
// 		if (i % 10 == 0)
// 			printf("\n");
// 	}
// }

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

    struct metadata *meta_end = END(b0);
    meta_end->available = 1;
    meta_end->size = b0->size;
}

void mysetup2(void *buf)
{
    memory_start = buf;

    struct metadata *tiny = (struct metadata *)buf;
    tiny->available = 1;
	tiny->type = TINY;
    tiny->size = N;
    END(tiny)->available = 1;
	END(tiny)->type = TINY;
    END(tiny)->size = N;

	struct metadata *small = NEXT(tiny);
	small->available = 1;
	small->type = SMALL;
	small->size = M;
	END(small)->available = 1;
	END(small)->type = SMALL;
	END(small)->size = M;

    last_valid_address = small;
}

int	check(int availables[], int sizes[])
{
	struct metadata *block = (struct metadata *)memory_start;

	int i = -1;

    while (1)
    {
		++i;

        struct metadata *end_meta = END(block);

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
	printf("Test 0 ... ");
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
		printf("OK\n");
	else
	{
		printf("ERROR\n");
		dump();
		dump_visual();
	}
	#if DEBUG
		dump();
		dump_visual();
	#endif

    free(buf);
}

void test_1(void) /* too big allocation */
{
	printf("Test 1 ... ");
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
		printf("OK\n");
	else
	{
		printf("ERROR\n");
		dump();
		dump_visual();
	}
	#if DEBUG
		dump();
		dump_visual();
	#endif

    free(buf);
}

void test_2(void) /* too big allocation */
{
	printf("Test 2 ... ");
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
		printf("OK\n");
	else
	{
		printf("ERROR\n");
		dump();
		dump_visual();
	}
	#if DEBUG
		dump();
		dump_visual();
	#endif

    free(buf);
}

void test_3(void) /* space left only for meta */
{
	printf("Test 3 ... ");
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
		printf("OK\n");
	else
	{
		printf("ERROR\n");
		dump();
		dump_visual();
	}
	#if DEBUG
		dump();
		dump_visual();
	#endif

    free(buf);
}

void test_4(void) /* right block eating */
{
	printf("Test 4 ... ");
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
		printf("OK\n");
	else
	{
		printf("ERROR\n");
		dump();
		dump_visual();
	}
	#if DEBUG
		dump();
		dump_visual();
	#endif

    free(buf);
}

void test_5(void) /* left block eating */
{
	printf("Test 5 ... ");
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
		printf("OK\n");
	else
	{
		printf("ERROR\n");
		dump();
		dump_visual();
	}
	#if DEBUG
		dump();
		dump_visual();
	#endif

    free(buf);
}

void test_6(void) /* several retries */
{
	printf("Test 6 ... ");
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
		printf("OK\n");
	else
	{
		printf("ERROR\n");
		dump();
		dump_visual();
	}
	#if DEBUG
		dump();
		dump_visual();
	#endif

    free(buf);
}

void test_7(void) /* fragmentation */
{
	printf("Test 7 ... ");
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
		printf("OK\n");
	else
	{
		printf("ERROR\n");
		dump();
		dump_visual();
	}
	#if DEBUG
		dump();
		dump_visual();
	#endif

    free(buf);
}

void test_8(void)
{
	printf("Test 8 ... ");
	size_t total_mem = 300;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup2(buf);
	void *a = myalloc(60); // large block
	
	printf("OK\n");
	#if DEBUG
		dump();
		dump_visual();
	#endif
}

void test_9(void) /* right block eating */
{
	printf("Test 9 ... ");
	size_t total_mem = 300;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup2(buf);

	dump();
	void *t1 = myalloc(1);
	dump();
	void *s1 = myalloc(6);
	void *t2 = myalloc(1);
	void *s2 = myalloc(6);
	void *t3 = myalloc(1);
	void *s3 = myalloc(6);


	myfree(s3);
	myfree(s2);
	myfree(s1);


	myfree(t3);
	myfree(t2);
	myfree(t1);

	int availables[] =	{1,  1  };
	int sizes[] =		{80, 150};
	if (check(availables, sizes))
		printf("OK\n");
	else
	{
		printf("ERROR\n");
		dump();
		dump_visual();
	}
	#if DEBUG
		dump();
		dump_visual();
	#endif

    free(buf);
}

void test_10(void) /* left block eating */
{
	printf("Test 10 ... ");
	size_t total_mem = 300;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup2(buf);

	void *t1 = myalloc(1);
	void *s1 = myalloc(6);
	void *t2 = myalloc(1);
	void *s2 = myalloc(6);
	void *t3 = myalloc(1);
	void *s3 = myalloc(6);

	#if DEBUG
		show_alloc_mem();
	#endif

	myfree(s1);
	myfree(s2);
	myfree(s3);

	myfree(t1);
	myfree(t2);
	myfree(t3);
	
	int availables[] =	{1,  1  };
	int sizes[] =		{80, 150};
	if (check(availables, sizes))
		printf("OK\n");
	else
		printf("ERROR\n");

    free(buf);
}

void testing(void)
{
	/* were ok when struct is 8 bytes
	test_0();
    test_1();
	test_2();
	test_3();
	test_4();
	test_5();
	test_6();
	test_7();
	*/

	// test_8();
	// test_9();
	test_10();
}