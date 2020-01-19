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

#include "block.h"
#include "zone.h"

struct zone_meta *get_my_zone_meta(struct block_meta *block)
{
	while (block->prev)
        block = block->prev;

	struct zone_meta *cur_zone = (struct zone_meta *)((char *)block - sizeof(struct zone_meta));

	return (cur_zone);
}

void free_allocated_block(struct block_meta *block, t_bool try_eat_next, t_bool try_eat_prev, t_bool try_unmap)
{
    block->available = true;

	if (try_eat_next) // TODO: check
	{
		struct block_meta *next = block->next;
		if (next && next->available) {

			#if DEBUG
				ft_putstr("[FREE] Eating next block\n");
			#endif

			block->size += next->size + sizeof(struct block_meta);
			block->next = next->next;

			struct block_meta *nextnext = next->next;
			if (nextnext) // TODO: find test for check
				nextnext->prev = block;

		}
	}

	if (try_eat_prev)
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
		}
	}

    struct zone_meta *cur_zone = get_my_zone_meta(block);
	block = ZONE_TO_BLOCK(cur_zone);

    t_bool all_available = true;
    while (block)
    {
        if (!block->available)
        {
            all_available = false;
            break ;
        }
        block = block->next;
    }

    if (try_unmap && all_available) // TODO: refactor to zone->av_blocks
    {
        #if DEBUG
            ft_putstr("[UNMAP] Zone available\n");
        #endif
        // remove zone from list
        struct zone_meta *next = cur_zone->next;
        struct zone_meta *prev = cur_zone->prev;

        if (prev)
        {
            prev->next = next;
        }

        if (next)
        {
            next->prev = prev;
        }

        if (cur_zone == first_zone)
        {
            #if DEBUG
                ft_putstr("[UNMAP] First zone changed\n");
            #endif

            first_zone = next;
        }

        /*
            To check page reclaims:
            Mac:    /usr/bin/time -l ./test
            Ubuntu: /usr/bin/time --verbose ./test
        */

        // TODO: test unmap with LARGE blocks
        int res = munmap(cur_zone, cur_zone->size);
        #if DEBUG
            ft_putstr("[UNMAP] munmap result: ");
            ft_putnbr(res);
            ft_putchar('\n');
        #endif
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
			if (tmp == block) // TODO: avail ?
			{
				#if DEBUG
					ft_putstr("[BLOCK] Block is allocated [Meta: "); // TODO: zone info
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
	else
	{
		// abort
	}
}