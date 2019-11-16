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





void mem_clear(void *buf, int len)
{
	char *mem = (char *)buf;
	unsigned int i = 0;
	while (i++ < len) {
		mem[i] = 0;
	}
}

void mysetup(void *buf)
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

void test_8(void)
{
	printf("Test 8 ... ");
	size_t total_mem = 300;
	void *buf = malloc(total_mem);
    mem_clear(buf, total_mem);
    mysetup(buf);
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

void test_10(void) /* left block eating */
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
	#endif

	show_alloc_mem_ex();

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
	test_8();
	test_9();
	test_10();
}
