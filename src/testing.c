/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 09:37:54 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 09:38:01 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <stdlib.h>

extern struct metadata *last_valid_address;
extern struct metadata *memory_start;

void mem_clear(void *buf, int len)
{
	char *mem = (char *)buf;
	unsigned int i = 0;
	while (i++ < len) {
		mem[i] = 0;
	}
}

void mysetup(void)
{
	if (memory_start == NULL)
    	memory_start = mmap(NULL, TINY_ZONE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

	printf("Memory start: %p\n", memory_start);

    struct metadata *tiny = (struct metadata *)memory_start;
    tiny->available = 1;
	tiny->type = TINY;
    tiny->size = TINY_ZONE - 2 * sizeof(struct metadata);
    END(tiny)->available = 1;
	END(tiny)->type = TINY;
    END(tiny)->size = TINY_ZONE - 2 * sizeof(struct metadata);

	// struct metadata *small = mmap(NULL, SMALL_ZONE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	// printf("Small zone: %p\n", small);

	// small->available = 1;
	// small->type = SMALL;
	// small->size = SMALL_ZONE - 2 * sizeof(struct metadata);
	// END(small)->available = 1;
	// END(small)->type = SMALL;
	// END(small)->size = SMALL_ZONE - 2 * sizeof(struct metadata);

    last_valid_address = tiny;
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

#include <errno.h>

void test_8(void)
{
	printf("Test 8 ... ");
    mysetup();
	void *a = myalloc(60); // large block
	myfree(a);
	void *b = myalloc(60); // large block
	myfree(a);

	#if DEBUG
		show_alloc_mem_ex();
		show_alloc_mem();
	#endif
}

/*
void test_9(void)
{
	printf("Test 9 ... ");
	size_t total_mem = 300;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup(buf);

	void *t1 = myalloc(1);
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

void test_10(void)
{
	printf("Test 10 ... ");
	size_t total_mem = 300;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup(buf);

	void *t1 = myalloc(1);
	void *s1 = myalloc(6);
	void *t2 = myalloc(1);
	void *s2 = myalloc(6);
	void *t3 = myalloc(1);
	void *s3 = myalloc(6);
	ft_strcpy(s3, "hello");

	#if DEBUG
		show_alloc_mem();
		show_alloc_mem_ex();
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
}*/

void test_mmap(void)
{
	printf("Test mmap ... \n");
	
	mysetup();
	show_alloc_mem();
	show_alloc_mem_ex();
}

void testing(void)
{
	test_8();
	// test_9();
	// test_10();

	// test_mmap();
}
