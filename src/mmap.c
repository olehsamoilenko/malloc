/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 09:37:16 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 09:37:23 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block.h"

void insert_zone_to_list(struct zone_meta *zone)
{
    struct zone_meta *lst = first_zone;

    while (lst && lst->next)
        lst = lst->next;

    if (lst)
    {
        lst->next = zone;
        zone->prev = lst;
    }
    else
    {
        first_zone = zone;
    }
}

struct zone_meta *mmap_zone(unsigned long size)
{
	unsigned long bytes_to_request;
	enum zone_type zone_type;

	if (size <= MAX_TINY_SIZE) {
		bytes_to_request = TINY_ZONE;
		zone_type = TINY;
	}
	else if (size <= MAX_SMALL_SIZE) {
		bytes_to_request = SMALL_ZONE;
		zone_type = SMALL;
	}
	else {
		bytes_to_request = LARGE_ZONE(size);
		zone_type = LARGE;
	}

	#if DEBUG
		ft_putstr("[PAGING] Bytes to request: ");
		ft_putnbr(bytes_to_request);
		ft_putchar('\n');
	#endif

	void *page = mmap(NULL, bytes_to_request, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	struct zone_meta *zone = (struct zone_meta *)page;
    zone->type = zone_type;
    zone->size = bytes_to_request;
	zone->prev = NULL;
	zone->next = NULL;

	struct block_meta *first_block = (struct block_meta *)(page + sizeof(struct zone_meta));
	first_block->available = true;
	first_block->size = bytes_to_request - sizeof(struct zone_meta) - sizeof(struct block_meta);
	first_block->next = NULL;
	first_block->prev = NULL;

	#if DEBUG
        ft_putstr("[PAGING] Zone mapped, size = ");
        ft_putnbr(bytes_to_request);
        ft_putstr(", zone start = ");
        ft_print_hex((unsigned long)zone);
        ft_putchar('\n');
        // TODO: type
	#endif

	return (zone);
}
