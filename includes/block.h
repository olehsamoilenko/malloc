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

// TODO: split to several files

#ifndef BLOCK_H
# define BLOCK_H

# include <sys/mman.h>

// TODO: $> nm libft_malloc.so

# include "libft.h"

# define ZONE_TO_BLOCK(zone) ((struct block_meta *)((char *)zone + sizeof(struct zone_meta))) // TODO: need it?
# define DATA_TO_META(block) ((struct block_meta *)((char *)block - sizeof(struct block_meta))) // TODO: need it?
# define META_TO_DATA(block) ((struct block_meta *)((char *)block + sizeof(struct block_meta))) // TODO: need it?
# define MAX_TINY_SIZE 5 /* TODO: at least 100 allocations */
# define MAX_SMALL_SIZE 10
# define TINY_ZONE (getpagesize()) /* in subject: N */
# define SMALL_ZONE (getpagesize()) /* in subject: M */
# define LARGE_ZONE(n) (n + sizeof(struct block_meta) + sizeof(struct zone_meta))
# define EXPORT __attribute__((visibility("default")))

enum zone_type
{
	TINY,
	SMALL,
	LARGE
};

struct block_meta
{
    unsigned int available; // TODO: 1 bit
    unsigned int size;
	struct block_meta *prev;
	struct block_meta *next;
};

struct zone_meta // TODO: zone.h
{
	enum zone_type type; // TODO: 2 bits
    unsigned int size;
	struct zone_meta *prev;
	struct zone_meta *next;
};

extern struct zone_meta *first_zone;

// TODO: need it ?
struct zone_meta *mmap_zone(unsigned long size);
void insert_zone_to_list(struct zone_meta *zone);
enum zone_type define_zone_type(size_t size);
struct zone_meta *get_my_zone_meta(struct block_meta *block);

void *alloc_on_block(struct block_meta *new_block, size_t size);
t_bool block_is_allocated(struct block_meta *block);

#endif
