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
#include "zone.h"

// TODO: If size is 0, then malloc() returns either NULL, or a unique pointer value that can later be successfully passed to free().
// TODO: NULL may also be returned by a successful call to malloc() with a size of zero

struct zone_meta *first_zone = NULL;

enum zone_type define_zone_type(size_t size)
{
	enum zone_type type;

	if (size <= MAX_TINY_BLOCK_SIZE) {
		type = TINY;
		#if DEBUG // TODO: refactor
			ft_putstr("[BLOCK] Block type: TINY\n");
		#endif
	}
	else if (size <= MAX_SMALL_BLOCK_SIZE) {
		type = SMALL;
		#if DEBUG
			ft_putstr("[BLOCK] Block type: SMALL\n");
		#endif
	}
	else {
		type = LARGE;
		#if DEBUG
			ft_putstr("[BLOCK] Block type: LARGE\n");
		#endif
	}

	return (type);
}

struct block_meta *get_suitable_block(size_t size)
{
    if (!size) // TODO: refactor, 1 return
        return (NULL);

	enum zone_type type = define_zone_type(size);

    struct zone_meta *zone = first_zone;
    while (zone)
    {
        if (zone->type == type)
        {
            struct block_meta *block = ZONE_TO_BLOCK(zone);

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
        ft_putstr("[BLOCK] No suitable space: size = ");
        ft_putnbr(size);
        ft_putchar('\n');
        // TODO: type
	#endif

    return (NULL);
}

void *alloc_on_block(struct block_meta *new_block, size_t size) // TODO: refactor, no return
{
	// TODO: scheme of new and reduced

	struct block_meta *reduced_block = (struct block_meta *)((char *)new_block + sizeof(struct block_meta) + size);
	reduced_block->available = true;
	reduced_block->size = new_block->size - sizeof(struct block_meta) - size;

	reduced_block->next = new_block->next;
	reduced_block->prev = new_block;
	new_block->next = reduced_block;

	new_block->available = false;
	new_block->size = size;

	#if DEBUG
		ft_putstr("[ALLOC] New: ");
		ft_print_hex((unsigned long)new_block, true);
		ft_putstr(" (");
		ft_putnbr(new_block->size);
		ft_putstr(" bytes), reduced: ");
		ft_print_hex((unsigned long)reduced_block, true);
		ft_putstr(" (");
		ft_putnbr(reduced_block->size);
		ft_putendl(" bytes)");
	#endif

	return (new_block);
}

// TODO tests: https://github.com/Haradric/ft_malloc/tree/master/tests
// TODO tests: https://github.com/mtupikov42/malloc/tree/master/test
void EXPORT *malloc(size_t size)
{
    void *ret;

    struct block_meta *new_block = get_suitable_block(size);
	if (!new_block)
	{
		insert_zone_to_list(mmap_zone(size));
		new_block = get_suitable_block(size);
	}

    if (new_block && new_block->size >= size + sizeof(struct block_meta))
    {
        ret = (char *)alloc_on_block(new_block, size) + sizeof(struct block_meta);
    }
    else if (new_block && new_block->size >= size)
    {
		#if DEBUG
        	ft_putstr("[ALLOC] Take last block in zone\n");
		#endif
        new_block->available = false; /* not filled fully, but doesn't matter */

        ret = (char *)new_block + sizeof(struct block_meta);
    }
    else
    {
        #if DEBUG
        	ft_putstr("[ALLOC] No suitable block\n");
		#endif
        ret = NULL;
    }

    #if DEBUG
        ft_putstr("[CALL] malloc: ");
        ft_print_hex((unsigned long)ret, true);
        ft_putchar('\n');
    #endif

    return (ret);
}

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

//          multi thread

//          own signal when metadata corrupted

//          calloc

//          The UNIX 98 standard requires malloc(), calloc(), and realloc() to set errno to ENOMEM upon failure
