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
# include <stdio.h> // TODO: remove
# include <unistd.h> // TODO: swap with libft
# include "libft.h"

# define MALLOC_H

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
	if (prev < memory_start)				\
		prev = NULL;						\
	prev;									\
}) /* from start */

# define START(block) (   \
	(struct metadata *)((char *)block - block->size - sizeof(struct metadata)) \
) /* from end */

# define END(block) (   \
	(struct metadata *)((char *)block + sizeof(struct metadata) + block->size) \
) /* from start */

// TODO: (n + 2 * struct) * 100
# define N 80 /* tiny zone size: meta + blocks TODO: // Each zone must contain at least 100 allocations. */ 
# define M 150 /* small zone size: meta + blocks  TODO: // • The size of these zones must be a multiple of getpagesize().*/
# define MAX_TINY_SIZE 5 // TODO: change to normal values
# define MAX_SMALL_SIZE 10



enum block_type
{
	// ZERO, // good for debug
	TINY,
	SMALL,
	LARGE
};

struct metadata {
    unsigned int available; // TODO: 1 bit
    unsigned int size;
	enum block_type type; // TODO: 2 bits
};

void myfree(void *p);
void *myalloc(unsigned long size);
void show_alloc_mem(void);
void show_alloc_mem_ex(void);

#endif
