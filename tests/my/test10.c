/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test10.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/22 10:32:26 by osamoile          #+#    #+#             */
/*   Updated: 2019/12/22 10:32:35 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

// realloc initial

void change_zone(void)
{
	void *m = malloc(6);
	void *m2 = malloc(6);
	show_alloc_mem();
	void *r = realloc(m2, 10);
	show_alloc_mem();
	void *r2 = realloc(r, 20);
	show_alloc_mem();

	// SMALL : 0x106de1000
	// 0x106de1030 - 0x106de1036 : 6 bytes
	// [AVAILABLE] 0x106de104e - 0x106de2000 : 4018 bytes
	// LARGE : 0x106de2000
	// 0x106de2030 - 0x106de2044 : 20 bytes
	// Total : 26
}

void eat_prev(void)
{
	void *m = malloc(6);
	void *m2 = malloc(6);
	void *m3 = malloc(6);
	free(m2);
	show_alloc_mem();
	void *r = realloc(m3, 10);
	show_alloc_mem();

	// SMALL : 0x10b802000
	// 0x10b802030 - 0x10b802036 : 6 bytes
	// [AVAILABLE] 0x10b80204e - 0x10b802054 : 6 bytes
	// 0x10b80206c - 0x10b802076 : 10 bytes
	// [AVAILABLE] 0x10b80208e - 0x10b803000 : 3954 bytes
	// Total : 16
}

void unmap(void)
{
	void *m = malloc(6);
	void *r = realloc(m, 10);
	show_alloc_mem();

	// SMALL : 0x102b5f000
	// 0x102b5f030 - 0x102b5f03a : 10 bytes
	// [AVAILABLE] 0x102b5f052 - 0x102b60000 : 4014 bytes
	// Total : 10
}

void malloc_free_prev(void)
{
	void *m = malloc(6);
	void *m2 = malloc(6);
	free(m);
	void *r = realloc(m2, 30);
	show_alloc_mem();

	// LARGE : 0x10e942000
	// 0x10e942030 - 0x10e94204e : 30 bytes
	// Total : 30
}

void null(void)
{
	void *r = realloc(NULL, 5);
	show_alloc_mem();

	// TINY : 0x10c057000
	// 0x10c057030 - 0x10c057035 : 5 bytes
	// [AVAILABLE] 0x10c05704d - 0x10c058000 : 4019 bytes
	// Total : 5
}

void zero(void) // TODO
{
	void *m = malloc(5);
	void *r = realloc(m, 0);
	show_alloc_mem();
}

int main()
{
	// change_zone();
	// null();
	zero();
}
