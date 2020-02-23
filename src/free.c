/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 09:37:16 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 09:37:23 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zone.h"

struct zone_meta *get_my_zone_meta(struct block_meta *block)
{
	struct block_meta *first = block;
	while (first && first->prev)
	{
        first = first->prev;
	}
	struct zone_meta *cur_zone = (struct zone_meta *)((char *)first - sizeof(struct zone_meta));
	return (cur_zone);
}

void free_allocated_block(struct block_meta *block, t_bool try_eat_next, t_bool try_eat_prev, t_bool try_unmap)
{
	if (try_eat_next)
	{
		struct block_meta *next = block->next;
		if (next && next->available == true) {
			#if DEBUG
				ft_putstr("[FREE] Eating next block\n");
			#endif
			next->available = false;
			struct block_meta *nextnext = next->next;
			if (nextnext)
			{
				nextnext->prev = block;
			}
			block->next = nextnext;
			block->size += next->size + sizeof(struct block_meta);
		}
	}

	if (try_eat_prev == true)
	{
		struct block_meta *prev = block->prev;
		if (prev && prev->available) {

			#if DEBUG
				ft_putstr("[FREE] Eating previous block\n");
			#endif

			prev->size += block->size + sizeof(struct block_meta);
			prev->next = block->next;
			struct block_meta *next = block->next;
			if (next)
				next->prev = prev;
			block = prev;
		}
	}

	block->available = true;

    struct zone_meta *cur_zone = get_my_zone_meta(block);
	struct block_meta *tmp = ZONE_TO_BLOCK(cur_zone);
    t_bool all_available = true;
    while (tmp)
    {
        if (tmp->available == false)
        {
            all_available = false;
            break ;
        }
        tmp = tmp->next;
    }

    if (try_unmap == true && all_available == true)
    {
        #if DEBUG
            ft_putstr("[UNMAP] Zone available\n");
        #endif

        if (cur_zone == first_zone)
        {
            #if DEBUG
                ft_putstr("[UNMAP] First zone changed\n");
            #endif

            first_zone = cur_zone->next;
        }
		else
		{
			// delete zone from list
			struct zone_meta *tmp = first_zone;

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

        munmap(cur_zone, cur_zone->size);
    }
}

t_bool block_is_allocated(struct block_meta *block)
{
	struct zone_meta *zone = first_zone;

	while (zone)
    {
        struct block_meta *tmp = ZONE_TO_BLOCK(zone);

        while (tmp)
        {
			if (tmp == block)
			{
				#if DEBUG
					ft_putstr("[BLOCK] Block is allocated [Meta: "); // TD: zone info
					ft_print_hex((unsigned long)block, true);
					ft_putstr(", data: ");
					ft_print_hex((unsigned long)DATA_TO_META(block), true);
					ft_putendl("]");
				#endif
				return (true);
			}

            tmp = tmp->next;
        }

        zone = zone->next;
    }
	
	#if DEBUG
		ft_putstr("[BLOCK] Block was NOT allocated [Meta: ");
		ft_print_hex((unsigned long)block, true);
		ft_putstr(", data: ");
		ft_print_hex((unsigned long)DATA_TO_META(block), true);
		ft_putendl("]");
	#endif
	return (false);
}

void EXPORT free(void *p)
{
	#if DEBUG
        ft_putstr("[CALL] free: ");
        ft_print_hex((unsigned long)p, true);
        ft_putchar('\n');
    #endif

	struct block_meta *block = DATA_TO_META(p);
    
	if (p && block_is_allocated(block))
	{
		free_allocated_block(block, true, true, true);
	}
}