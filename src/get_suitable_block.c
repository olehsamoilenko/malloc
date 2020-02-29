/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_suitable_block.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 09:37:16 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 09:37:23 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zone.h"

static void					insert_zone_to_list(struct s_zone_meta *zone)
{
	struct s_zone_meta *lst;

	lst = g_first_zone;
	while (lst && lst->next)
		lst = lst->next;
	if (lst)
		lst->next = zone;
	else
		g_first_zone = zone;
}

static struct s_block_meta	*search_for_block(size_t size)
{
	const enum e_zone_type	type = define_zone_type(size);
	struct s_zone_meta		*zone;
	struct s_block_meta		*block;

	zone = g_first_zone;
	while (zone)
	{
		if (zone->type == type)
		{
			block = ZONE_TO_BLOCK(zone);
			while (block)
			{
				if (block->available && block->size >= size)
					return (block);
				block = block->next;
			}
		}
		zone = zone->next;
	}
	print_decimal_value("[BLOCK] No suitable space: size = ", size);
	return (NULL);
}

struct s_block_meta			*get_suitable_block(size_t size)
{
	struct s_block_meta	*new_block;
	void				*new_zone;

	new_block = search_for_block(size);
	if (new_block == NULL)
	{
		new_zone = mmap_zone(size);
		if (new_zone)
			insert_zone_to_list(new_zone);
		new_block = search_for_block(size);
	}
	return (new_block);
}