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

# define FIRST_BLOCK(zone) ((struct block_meta *)((char *)zone + sizeof(struct zone_meta)))
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
	// enum zone_type type; // TODO: move to zone_meta
	struct block_meta *next;
	struct block_meta *prev;
};

struct zone_meta {
	// busy_blocks : int
	// size : int
	enum zone_type type; // TODO: 2 bits
	struct zone_meta *prev;
	struct zone_meta *next;
};

void myfree(void *p);
void *myalloc(unsigned long size);
void show_alloc_mem(void);
void show_alloc_mem_ex(void);

// TODO: remove
struct zone_meta *mmap_zone(unsigned long size);
struct zone_meta *get_first_zone();
void insert_zone_to_list(struct zone_meta *zone);

#endif
