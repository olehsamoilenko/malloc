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

#include "zone.h"

struct zone_meta *first_zone = NULL;
pthread_mutex_t	g_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;

enum zone_type define_zone_type(size_t size)
{
	enum zone_type type;

	if (size <= MAX_TINY_BLOCK_SIZE) {
		type = TINY;
		#if DEBUG // TD: refactor
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
        // TD: type
	#endif

    return (NULL);
}

void *alloc_on_block(struct block_meta *new_block, size_t size) // TD: refactor, no return
{
	// TD: scheme of new and reduced

	if (new_block && new_block->size >= size + sizeof(struct block_meta))
	{
		struct block_meta *reduced_block = (struct block_meta *)((char *)new_block + sizeof(struct block_meta) + size);
		reduced_block->available = true;
		reduced_block->size = new_block->size - sizeof(struct block_meta) - size;
		reduced_block->next = new_block->next;
		reduced_block->prev = new_block;

		struct block_meta *nextnext = reduced_block->next;
		if (nextnext)
			nextnext->prev = reduced_block;

		new_block->available = false;
		new_block->size = size;
		new_block->next = reduced_block;

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

	}
	else if (new_block && new_block->size >= size)
	{
		#if DEBUG
			ft_putstr("[ALLOC] Take last block in zone\n");
		#endif
		new_block->available = false; /* not filled fully, but doesn't matter */
	}

	return (new_block);
}

// TODO tests: https://github.com/Haradric/ft_malloc/tree/master/tests
// TODO tests: https://github.com/mtupikov42/malloc/tree/master/test
void EXPORT *malloc(size_t size)
{
	pthread_mutex_lock(&g_mutex);
	#if DEBUG
		ft_putstr("[CALL] malloc: ");
		ft_putnbr(size);
		ft_putchar('\n');
	#endif

    void *ret;

    struct block_meta *new_block = get_suitable_block(size);
	if (!new_block)
	{
		insert_zone_to_list(mmap_zone(size));
		new_block = get_suitable_block(size);
	}

    if (new_block && new_block->size >= size)
    {
        ret = (char *)alloc_on_block(new_block, size) + sizeof(struct block_meta);
    }
    else
    {
        #if DEBUG
        	ft_putstr("[FAIL] No suitable block\n");
		#endif
        ret = NULL;
    }

    #if DEBUG
        ft_putstr("[RETURN] malloc: ");
        ft_print_hex((unsigned long)ret, true);
        ft_putchar('\n');
    #endif

	pthread_mutex_unlock(&g_mutex);

    return (ret);
}

//          multi thread

//          own signal when metadata corrupted

//          calloc

//          The UNIX 98 standard requires malloc(), calloc(), and realloc() to set errno to ENOMEM upon failure
