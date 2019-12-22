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

#include "block.h"

struct zone_meta *first_zone = NULL;

// TODO: realloc

// TODO: If “ptr” is a NULL pointer, no operation is performed.
void EXPORT free(void *p)
{
    struct block_meta *block = (struct block_meta *)((char *)p - sizeof(struct block_meta));
    block->available = true;

    struct block_meta *next = block->next;
	if (next && next->available) {

        #if DEBUG
            printf("[FREE] Eating next block\n");
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
            printf("[FREE] Eating previous block\n");
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
            printf("[UNMAP] Zone available\n");
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
                printf("[UNMAP] First zone changed\n");
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
            printf("[UNMAP] munmap result: %d\n", res);
        #endif
    }
}

struct block_meta *get_suitable_block(size_t size)
{
    if (!size)
        return (NULL);

	enum zone_type type;
	if (size <= MAX_TINY_SIZE) {
		type = TINY;
		#if DEBUG // TODO: refactor
			printf("[BLOCK] Block type: TINY\n");
		#endif
	}
	else if (size <= MAX_SMALL_SIZE) {
		type = SMALL;
		#if DEBUG
			printf("[BLOCK] Block type: SMALL\n");
		#endif
	}
	else {
		type = LARGE;
		#if DEBUG
			printf("[BLOCK] Block type: LARGE\n");
		#endif
	}

    struct zone_meta *zone = first_zone;
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
		printf("[BLOCK] No suitable space: size = %lu\n", size); // TODO: type
	#endif

    return (NULL);
}

// TODO tests: https://github.com/Haradric/ft_malloc/tree/master/tests
// TODO tests: https://github.com/mtupikov42/malloc/tree/master/test
void EXPORT *malloc(size_t size)
{
	printf("HELLO\n");

	// TODO: scheme of new and reduced

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

		#if DEBUG
			printf("[ALLOC] New: %p %u; Reduced: %p %u\n", new_block, new_block->size, reduced_block, reduced_block->size);
		#endif

        return ((char *)new_block + sizeof(struct block_meta));
    }
    else if (new_block && new_block->size >= size)
    {
		#if DEBUG
        	printf("[ALLOC] Take last block in zone\n");
		#endif
        new_block->available = false; /* not filled fully, but doesn't matter */

        return ((char *)new_block + sizeof(struct block_meta));
    }
    else
    {
        #if DEBUG
        	printf("[ALLOC] No suitable block\n");
		#endif
        return (NULL);
    }
}

void testing(void);

// BONUSES:
// 2. DONE  Manage the malloc debug environment variables. You can imitate those from malloc
//          system or invent your own.
//          - Malloc has debug environment variables

// 3. DONE  • Create a show_alloc_mem_ex() function that displays more details, for example,
//          a history of allocations, or an hexa dump of the allocated zones.
//          - A function allows to dump hexa allocated zones

// 1. DONE  • “Defragment” the freed memory.
//          - During free, the project "defragments" free memory by grouping free blocks
//          concomitant in one

//          • Manage the use of your malloc in a multi-threaded program (so to be “thread safe”
//          using the pthread lib).

//          - A function makes it possible to display a history of the memory allocations made

//          own segfault
