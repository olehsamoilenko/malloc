/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 09:38:18 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 09:38:27 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <stdio.h> // TODO: remove
# include <sys/mman.h>
# include "libft.h"

# define MAX_TINY_SIZE 5 /* TODO: at least 100 allocations */
# define MAX_SMALL_SIZE 10
# define TINY_ZONE (getpagesize()) /* in subject: N */
# define SMALL_ZONE (getpagesize()) /* in subject: M */
# define LARGE_ZONE(n) ({							\
	unsigned long res = getpagesize();				\
	while (res < n + sizeof(struct metadata))		\
		res += getpagesize();						\
	res;											\
}) // TODO: check macros

enum block_type
{
	TINY,
	SMALL,
	LARGE
};

struct metadata {
    unsigned int available; // TODO: 1 bit
    unsigned int size;
	enum block_type type; // TODO: 2 bits TODO: move to page_meta
	struct metadata *next;
	struct metadata *prev;
};

struct metadata *get_memory_start(); // TODO: replace from header

void myfree(void *p);
void *myalloc(unsigned long size);
void show_alloc_mem(void);
void show_alloc_mem_ex(void);

#endif
