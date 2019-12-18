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

extern struct block_meta *last_valid_address;

#include <errno.h>

void test_8(void)
{
	ft_printf("Test 8 ... ");

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
	myalloc(getpagesize());
}

void test_unmap() // TODO: try
{
	void *a = myalloc(4072);
	void *b = myalloc(4072);
	myfree(b);
}

void test_paging()
{
    void *n = myalloc(1);
    void *n2 = myalloc(2);
    void *n3 = myalloc(50);
	void *n4 = myalloc(4);

    myfree(n4);
    myfree(n3);
    myfree(n2);
    myfree(n);

    void *n5 = myalloc(10);
    myfree(n5);

    show_alloc_mem();
    show_alloc_mem_ex();
}

void testing(void)
{
    test_paging();
}
