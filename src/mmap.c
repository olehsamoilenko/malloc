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
# define EXPORT_META struct s_zone_meta __attribute__((visibility("default")))
EXPORT_META *get_first_zone()
{
	return (first_zone);
}
#endif

void insert_zone_to_list(struct s_zone_meta *zone)
{
	struct s_zone_meta *lst;

	lst = first_zone;
	while (lst && lst->next)
		lst = lst->next;

	if (lst)
	{
		lst->next = zone;
	}
	else
	{
		first_zone = zone;
	}
}

struct s_zone_meta *mmap_zone(size_t size)
{
	size_t bytes_need;
	enum e_zone_type zone_type;

	if (size <= MAX_TINY_BLOCK_SIZE) {
		bytes_need = BLOCKS_IN_ZONE * (MAX_TINY_BLOCK_SIZE + sizeof(struct s_block_meta)) + sizeof(struct s_zone_meta);
		zone_type = TINY;
	}
	else if (size <= MAX_SMALL_BLOCK_SIZE) {
		bytes_need = BLOCKS_IN_ZONE * (MAX_SMALL_BLOCK_SIZE + sizeof(struct s_block_meta)) + sizeof(struct s_zone_meta);
		zone_type = SMALL;
	}
	else {
		bytes_need = size + sizeof(struct s_block_meta) + sizeof(struct s_zone_meta);
		zone_type = LARGE;
	}

	size_t pages = (bytes_need - 1) / getpagesize() + 1;
	size_t bytes_to_request = pages * getpagesize();

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
	if (page == MAP_FAILED)
	{
		#if DEBUG
			ft_putendl("[PAGING] mmap failed");
		#endif
		return (NULL);
	}
	struct s_zone_meta *zone = (struct s_zone_meta *)page;
	zone->type = zone_type;
	zone->size = bytes_to_request;
	zone->next = NULL;

	struct s_block_meta *first_block = (struct s_block_meta *)(page + sizeof(struct s_zone_meta));
	first_block->available = true;
	first_block->size = bytes_to_request - sizeof(struct s_zone_meta) - sizeof(struct s_block_meta);
	first_block->next = NULL;
	first_block->prev = NULL;

	#if DEBUG
		ft_putstr("[PAGING] Zone mapped, size = ");
		ft_putnbr(bytes_to_request);
		ft_putstr(", zone start = ");
		ft_print_hex((unsigned long)zone, true);
		ft_putchar('\n');
	#endif

	return (zone);
}
