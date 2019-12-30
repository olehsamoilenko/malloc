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

// TODO: realloc

// TODO: If “ptr” is a NULL pointer, no operation is performed.
void EXPORT free(void *p)
{
	// void *caller = __builtin_return_address(0);
	// ft_putnbr(caller);
	// ft_putstr(" ");

    #if DEBUG
        ft_putstr("[CALL] free: ");
        ft_print_hex((unsigned long)p);
        ft_putchar('\n');
    #endif

    struct block_meta *block = (struct block_meta *)((char *)p - sizeof(struct block_meta));
    block->available = true;

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

    // searching for zone meta
    while (block->prev)
        block = block->prev;
    // block is first block now
    struct zone_meta *cur_zone = (struct zone_meta *)((char *)block - sizeof(struct zone_meta));
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

    if (all_available)
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