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

struct block_meta *get_suitable_block(size_t size)
{
    if (!size)
        return (NULL);

	enum zone_type type;
	if (size <= MAX_TINY_SIZE) {
		type = TINY;
		#if DEBUG // TODO: refactor
			ft_putstr("[BLOCK] Block type: TINY\n");
		#endif
	}
	else if (size <= MAX_SMALL_SIZE) {
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
        ft_putstr("[BLOCK] No suitable space: size = ");
        ft_putnbr(size);
        ft_putchar('\n');
        // TODO: type
	#endif

    return (NULL);
}

// TODO tests: https://github.com/Haradric/ft_malloc/tree/master/tests
// TODO tests: https://github.com/mtupikov42/malloc/tree/master/test
void EXPORT *malloc(size_t size)
{
    void *ret;

	// void *caller = __builtin_return_address(0);
	// ft_putnbr(caller);
	// ft_putstr(" ");

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
            ft_putstr("[ALLOC] New: ");
            ft_print_hex((unsigned long)new_block);
            ft_putchar(' ');
            ft_putnbr(new_block->size);
            ft_putstr("; Reduced: ");
            ft_print_hex((unsigned long)reduced_block);
            ft_putchar(' ');
            ft_putnbr(reduced_block->size);
            ft_putchar('\n');
		#endif

        ret = (char *)new_block + sizeof(struct block_meta);
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
        ft_print_hex((unsigned long)ret);
        ft_putchar('\n');
    #endif

    return (ret);
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
