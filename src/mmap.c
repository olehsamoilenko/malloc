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

#include "zone.h"

#if DEBUG
struct zone_meta EXPORT *get_first_zone() 
{
	return (first_zone);
}
#endif

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
	unsigned long bytes_need;
	enum zone_type zone_type;

	if (size <= MAX_TINY_BLOCK_SIZE) {
		bytes_need = BLOCKS_IN_ZONE * (MAX_TINY_BLOCK_SIZE + sizeof(struct block_meta)) + sizeof(struct zone_meta);
		zone_type = TINY;
	}
	else if (size <= MAX_SMALL_BLOCK_SIZE) {
		bytes_need = BLOCKS_IN_ZONE * (MAX_SMALL_BLOCK_SIZE + sizeof(struct block_meta)) + sizeof(struct zone_meta);
		zone_type = SMALL;
	}
	else {
		bytes_need = size + sizeof(struct block_meta) + sizeof(struct zone_meta);
		zone_type = LARGE;
	}

	unsigned int pages = (bytes_need - 1) / getpagesize() + 1;
	unsigned long bytes_to_request = pages * getpagesize();

	#if DEBUG
		ft_putstr("[PAGING] Bytes need: ");
		ft_putnbr(bytes_need);
		ft_putstr(", bytes to request: ");
		ft_putnbr(bytes_to_request);
		ft_putstr(" (");
		ft_putnbr(pages);
		ft_putendl(" pages)");
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
        ft_print_hex((unsigned long)zone, true);
        ft_putchar('\n');
        // TD: type
	#endif

	return (zone);
}
