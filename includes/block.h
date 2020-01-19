/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/22 10:40:43 by osamoile          #+#    #+#             */
/*   Updated: 2019/12/22 10:40:45 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_H
# define BLOCK_H

# include <sys/mman.h>
# include "libft.h"

// TODO: $> nm libft_malloc.so

# define MAX_TINY_BLOCK_SIZE 128 // TODO: 100 allocations
# define MAX_SMALL_BLOCK_SIZE 1024

# define DATA_TO_META(block) ((struct block_meta *)((char *)block - sizeof(struct block_meta))) // TODO: need it?
# define META_TO_DATA(block) ((struct block_meta *)((char *)block + sizeof(struct block_meta))) // TODO: need it?

# define EXPORT __attribute__((visibility("default")))

struct block_meta
{
    unsigned int available; // TODO: 1 bit
    unsigned int size;
	struct block_meta *prev;
	struct block_meta *next;
};

// TODO: need it ?
void *alloc_on_block(struct block_meta *new_block, size_t size);
t_bool block_is_allocated(struct block_meta *block);

void free_allocated_block(struct block_meta *block, t_bool try_eat_next, t_bool try_eat_prev, t_bool try_unmap);

#endif
