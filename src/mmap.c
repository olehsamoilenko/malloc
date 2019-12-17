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

#include "malloc.h"

struct page_meta *mmap_zone(unsigned long size)
{
	unsigned long bytes_to_request;
	enum block_type zone_type;

	if (size <= MAX_TINY_SIZE) {
		bytes_to_request = TINY_ZONE;
		zone_type = TINY;
	}
	else if (size <= MAX_SMALL_SIZE) {
		bytes_to_request = SMALL_ZONE;
		zone_type = SMALL;
	}
	else {
		bytes_to_request = LARGE_ZONE(size); // TODO: check overflowing, TODO: page + 1 ?
		printf("To request: %ld\n", bytes_to_request);
		zone_type = LARGE;
	}

	void *page = mmap(NULL, bytes_to_request, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	struct page_meta *zone = (struct page_meta *)page;
	zone->prev = NULL;
	zone->next = NULL;
	// TODO: zone type

	struct metadata *first_block = (struct metadata *)(page + sizeof(struct page_meta));
	first_block->available = true;
	first_block->type = zone_type;
	first_block->size = bytes_to_request - sizeof(struct page_meta) - sizeof(struct metadata);
	first_block->next = NULL;
	first_block->prev = NULL;

	#if DEBUG
		printf("[PAGING] Zone mapped, size = %lu, zone start = %p\n", bytes_to_request, zone); // TODO: type
	#endif

	return (zone);
}

struct page_meta *get_first_page()
{
	static void *first_page = NULL;

	if (first_page == NULL)
	{
		first_page = mmap_zone(MAX_TINY_SIZE);
	}
	return (first_page);
}