/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/22 10:40:43 by osamoile          #+#    #+#             */
/*   Updated: 2019/12/22 10:40:45 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZONE_H
# define ZONE_H

# define ZONE_TO_BLOCK(zone) ((struct block_meta *)((char *)zone + sizeof(struct zone_meta)))
# define BLOCKS_IN_ZONE 100

# include <stdlib.h>
# include "block.h"

enum zone_type
{
	TINY,
	SMALL,
	LARGE
};

struct zone_meta
{
	enum zone_type type; // TODO: 2 bits
	unsigned int size;
	struct zone_meta *next;
};

extern struct zone_meta *first_zone;

// TDCHECK: need it ?
struct zone_meta *mmap_zone(unsigned long size);
void insert_zone_to_list(struct zone_meta *zone);
enum zone_type define_zone_type(size_t size);
struct zone_meta *get_my_zone_meta(struct block_meta *block);
struct zone_meta *get_first_zone(void);

#endif
