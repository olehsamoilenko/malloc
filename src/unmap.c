/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unmap.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 09:37:16 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 09:37:23 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zone.h"

struct s_zone_meta		*get_my_zone_meta(struct s_block_meta *block)
{
	struct s_block_meta *first;

	first = block;
	while (first && first->prev)
		first = first->prev;
	return (BLOCK_TO_ZONE(first));
}

static t_bool			all_blocks_available(struct s_zone_meta *cur_zone)
{
	t_bool				res;
	struct s_block_meta	*tmp;

	res = true;
	tmp = ZONE_TO_BLOCK(cur_zone);
	while (tmp)
	{
		if (tmp->available == false)
		{
			res = false;
			break ;
		}
		tmp = tmp->next;
	}
	return (res);
}

static void				delete_zone_from_list(struct s_zone_meta *cur_zone)
{
	struct s_zone_meta *tmp;

	tmp = g_first_zone;
	while (tmp && tmp->next)
	{
		if (tmp->next == cur_zone)
		{
			tmp->next = cur_zone->next;
			break ;
		}
		tmp = tmp->next;
	}
}

void					unmap(struct s_block_meta *block)
{
	struct s_zone_meta *cur_zone;

	cur_zone = get_my_zone_meta(block);
	if (all_blocks_available(cur_zone) == true)
	{
		if (DEBUG)
			ft_putstr("[UNMAP] Zone available\n");
		if (cur_zone == g_first_zone)
		{
			if (DEBUG)
				ft_putstr("[UNMAP] First zone changed\n");
			g_first_zone = cur_zone->next;
		}
		else
			delete_zone_from_list(cur_zone);
		munmap(cur_zone, cur_zone->size);
	}
}