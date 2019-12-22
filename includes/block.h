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
}) // TODO: check macros // TODO: need it? refactor
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

struct zone_meta
{
	// TODOMB :busy_blocks : int
	enum zone_type type; // TODO: 2 bits
    unsigned int size;
	struct zone_meta *prev;
	struct zone_meta *next;
};

extern struct zone_meta *first_zone;

// TODO: remove
struct zone_meta *mmap_zone(unsigned long size);
void insert_zone_to_list(struct zone_meta *zone);

#endif
