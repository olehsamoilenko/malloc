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

# include <sys/mman.h>
# include "libft.h"

# define FIRST_BLOCK(page) ((struct block_meta *)((char *)page + sizeof(struct page_meta)))
# define MAX_TINY_SIZE 5 /* TODO: at least 100 allocations */
# define MAX_SMALL_SIZE 10
# define TINY_ZONE (getpagesize()) /* in subject: N */
# define SMALL_ZONE (getpagesize()) /* in subject: M */
# define LARGE_ZONE(n) ({							\
	unsigned long res = getpagesize();				\
	while (res < n + sizeof(struct block_meta))		\
		res += getpagesize();						\
	res;											\
}) // TODO: check macros

enum zone_type // TODO: zone_type
{
	TINY,
	SMALL,
	LARGE
};

struct block_meta { // TODO: block_meta
    unsigned int available; // TODO: 1 bit
    unsigned int size;
	// enum zone_type type; // TODO: move to page_meta
	struct block_meta *next;
	struct block_meta *prev;
};

struct page_meta {
	// busy_blocks : int
	// size : int
	enum zone_type type; // TODO: 2 bits
	struct page_meta *prev;
	struct page_meta *next;
};

void myfree(void *p);
void *myalloc(unsigned long size);
void show_alloc_mem(void);
void show_alloc_mem_ex(void);

// TODO: remove
struct page_meta *mmap_zone(unsigned long size);
struct page_meta *get_first_zone();
void insert_zone_to_list(struct page_meta *zone);

#endif
