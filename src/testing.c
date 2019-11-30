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

void mem_clear(void *buf, int len)
{
	char *mem = (char *)buf;
	unsigned int i = 0;
	while (i++ < len) {
		mem[i] = 0;
	}
}

int	check(int availables[], int sizes[])
{
	struct metadata *block = (struct metadata *)get_memory_start();

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

	void *a = myalloc(1); // tiny block
	void *b = myalloc(60); // large block
	myfree(b);
	myfree(a);

	#if DEBUG
		show_alloc_mem_ex();
		show_alloc_mem();
	#endif
}

void test_many_small()
{
	for (int i = 0; i < 100; i++)
		myalloc(9); // small

	myalloc(60);

	for (int i = 0; i < 100; i++)
		myalloc(9); // small

	myalloc(60);
	myalloc(4000);
	myalloc(8000);
}

void testing(void)
{
	// test_8();
	test_many_small();
	show_alloc_mem_ex();
	show_alloc_mem();
}
