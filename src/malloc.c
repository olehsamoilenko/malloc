/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 09:37:16 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 09:37:23 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

struct block_meta *last_valid_address = NULL;

// TODO: realloc

// TODO: If “ptr” is a NULL pointer, no operation is performed.

// TODO: We will compare the number of "reclaims" with the number of test0 and test1. If there is so much
// of "page reclaims" or more than test1, the free does not work.
// $> ./ run.sh / usr / bin / time -l ./test2
void myfree(void *p)
{
    // struct block_meta *block = (struct block_meta *)((char *)p - sizeof(struct block_meta));
    // block->available = true;

    // struct block_meta *next = block->next;
	// if (next && next->available && next->type == block->type) {

	// 	block->size += next->size + sizeof(struct block_meta);
	// 	block->next = next->next;

	// 	if (next == last_valid_address) {
	// 		last_valid_address = block; /* next block is eaten */
	// 	}
	// }

	// struct block_meta *prev = block->prev;
	// if (prev && prev->available) {
		
	// 	if (prev->type == block->type) {

	// 		prev->size += block->size + sizeof(struct block_meta);
	// 		prev->next = block->next;
			
	// 		if (block == last_valid_address) {
	// 			last_valid_address = prev; /* prev block eaten */
	// 		}
	// 	}
	// 	else {
	// 		if (block == last_valid_address) {
	// 			ft_printf("Yes, it's last, need to unmap\n");
	// 			int res = munmap(block, block->size); // TODO: if unmap several pages, don't unmap first page!
	// 			ft_printf("Unmap result: %d\n", res);
	// 			last_valid_address = prev; // TODO: duplication
	// 		}
	// 	}
	// }
}

struct block_meta *get_suitable_block(unsigned long size)
{
    if (!size)
        return (NULL);

	enum zone_type type;
	if (size <= MAX_TINY_SIZE) {
		type = TINY;
		#if DEBUG
			ft_printf("[BLOCK] Block type: TINY\n");
		#endif
	}
	else if (size <= MAX_SMALL_SIZE) {
		type = SMALL;
		#if DEBUG
			ft_printf("[BLOCK] Block type: SMALL\n");
		#endif
	}
	else {
		type = LARGE;
		#if DEBUG
			ft_printf("[BLOCK] Block type: LARGE\n");
		#endif
	}

    struct page_meta *zone = get_first_zone();
    while (zone)
    {
        if (zone->type == type)
        {
            struct block_meta *block = FIRST_BLOCK(zone);

            while (block)
            {
                if (block->available && block->size >= size)
                    return (block);

                block = block->next;
            }
        }
        
        zone = zone->next;
    }

	#if DEBUG
		ft_printf("[BLOCK] No suitable space: size = %lu\n", size); // TODO: type
	#endif

    return (NULL);
}

// TODO Count the number of pages used and adjust the score as follows:
// - less than 255 pages, the reserved memory is insufficient: 0
// - 1023 pages and more, the malloc works but consumes a minimum page each allocation: 1
// - between 513 pages and 1022 pages, the malloc works but the overhead is too important: 2
// - between 313 pages and 512 pages, the malloc works but the overhead is very important: 3
// - between 273 pages and 312 pages, the malloc works but the overhead is important: 4
// - between 255 and 272 pages, the malloc works and the overhead is reasonable: 5
void *myalloc(unsigned long size)
{
	// TODO: get rid of last valid addr
	// TODO: rename new and reduced blocks

    struct block_meta *new_block = get_suitable_block(size);
	if (!new_block)
	{
		insert_zone_to_list(mmap_zone(size));
		new_block = get_suitable_block(size);
	}

    if (new_block && new_block->size >= size + sizeof(struct block_meta))
    {
        struct block_meta *reduced_block = (struct block_meta *)((char *)new_block + sizeof(struct block_meta) + size);
        reduced_block->available = true;
        reduced_block->size = new_block->size - sizeof(struct block_meta) - size;

		reduced_block->next = new_block->next;
		reduced_block->prev = new_block;
		new_block->next = reduced_block;
	
        new_block->available = false;
        new_block->size = size;

        if (reduced_block > last_valid_address)
            last_valid_address = reduced_block;

		#if DEBUG
			ft_printf("[ALLOC] New: %p %u; Reduced: %p %u\n", new_block, new_block->size, reduced_block, reduced_block->size);
		#endif

		// ft_printf("me: %p   next: %p   prev: %p\n",
		// 	new_block, new_block->next, new_block->prev);
		// if (new_block->next) {
		// 	struct block_meta *nxt = new_block->next;
		// 	ft_printf("my next: %p   next: %p   prev: %p\n",
		// 		nxt, nxt->next, nxt->prev);
		// }
		// if (new_block->prev) {
		// 	struct block_meta *prv = new_block->prev;
		// 	ft_printf("my prev: %p   next: %p   prev: %p\n",
		// 		prv, prv->next, prv->prev);
		// }

        return ((char *)new_block + sizeof(struct block_meta));
    }
    else if (new_block && new_block->size >= size)
    {
		#if DEBUG
        	ft_printf("maybe not filled fully, but doesn't matter\n");
		#endif
        new_block->available = false; /* not filled fully, but doesn't matter */

        return ((char *)new_block + sizeof(struct block_meta));
    }
    else
    {
        return (NULL); /* there is no suitable block */
    }
}

void testing(void);

int main(void)
{
    testing();
    return (0);
}

// BONUSES:
// DONE     Manage the malloc debug environment variables. You can imitate those from malloc
//          system or invent your own.
//          - Malloc has debug environment variables

// DONE     • Create a show_alloc_mem_ex() function that displays more details, for example,
//          a history of allocations, or an hexa dump of the allocated zones.
//          - A function allows to dump hexa allocated zones

// DONE     • “Defragment” the freed memory.
//          - During free, the project "defragments" free memory by grouping free blocks
//          concomitant in one

//          • Manage the use of your malloc in a multi-threaded program (so to be “thread safe”
//          using the pthread lib).
//          - A function makes it possible to display a history of the memory allocations made

// void *zone = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
// void *zone2 = mmap(zone, getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
// void *zone3 = mmap(zone2 + 1, getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
// munmap(zone2, getpagesize());
// // myfree(a);
// ft_printf("%p %p %p\n", zone, zone2, zone3);
// zone2 unmaped, map it before use
