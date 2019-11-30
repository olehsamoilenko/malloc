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

# define NEXT(block) (														\
    (struct metadata *)((char *)block + 2 * sizeof(struct metadata) + block->size)	\
) /* from start */

# define GETNEXT(block) ({					\
    struct metadata *next = NEXT(block);	\
	if (next > last_valid_address)			\
		next = NULL;						\
	next;									\
}) /* from start */

# define PREV(block) (   \
    (struct metadata *)((char *)block - sizeof(struct metadata)) \
) /* from start */

# define GETPREV(block) ({					\
    struct metadata *prev = PREV(block);	\
	if (prev < get_memory_start())				\
		prev = NULL;						\
	prev;									\
}) /* from start */

# define START(block) (   \
	(struct metadata *)((char *)block - block->size - sizeof(struct metadata)) \
) /* from end */

# define END(block) (   \
	(struct metadata *)((char *)block + sizeof(struct metadata) + block->size) \
) /* from start */

# define MAX_TINY_SIZE 5 /* TODO: at least 100 allocations */
# define MAX_SMALL_SIZE 10
# define TINY_ZONE (getpagesize()) /* in subject: N */
# define SMALL_ZONE (getpagesize()) /* in subject: M */
# define LARGE_ZONE(n) ({							\
	unsigned long res = getpagesize();				\
	while (res < n + 2 * sizeof(struct metadata))	\
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
    unsigned int size; // TODO: long ?
	enum block_type type; // TODO: 2 bits
	struct metadata *next;
	struct metadata *prev;
};

struct metadata *get_memory_start(); // TODO: replace from header

void myfree(void *p);
void *myalloc(unsigned long size);
void show_alloc_mem(void);
void show_alloc_mem_ex(void);

#endif
