/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 09:37:16 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 09:37:23 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block.h"

// TODO: If ptr is NULL, then the call is equivalent to malloc(size)
// TODO: if size is equal to zero, and ptr is not NULL, then the call is equivalent to free(ptr)
// TODO: If the area pointed to was moved, a free(ptr) is done.
// TODO: The UNIX 98 standard requires malloc(), calloc(), and realloc() to set errno to ENOMEM upon failure
// TODO: If realloc() fails the original block is left untouched; it is not freed or moved.

void EXPORT *realloc(void *ptr, size_t size)
{
	#if DEBUG
        ft_putstr("[CALL] realloc: ");
        ft_print_hex((unsigned long)ptr);
        ft_putchar('\n');
    #endif

	void *ret = NULL;

	struct block_meta *b = START_OF_BLOCK(ptr);
	// TODO: check if block is allocated

	// define zone type
	enum zone_type new_type = define_zone_type(size);

	// define zone of my position
	struct zone_meta *my_zone = get_my_zone_meta(b);

	free(ptr); // TODO test 10 SF // TODO: may eat prev // TODO: may unmap zone

	// check if block is suitable
	if (b->size >= size && my_zone->type == new_type)
	{
		#if DEBUG
			ft_putendl("[REALLOC] Checking... Block is suitable");
		#endif
	
		// TODO: check with old size > new size
		alloc_on_block(b, size);
		ret = b;
	}
	else
	{
		#if DEBUG
			ft_putstr("[REALLOC] Checking... Block is not suitable: ");
			if (b->size < size)
				ft_putendl("size is too large");
			else if (my_zone->type != new_type)
				ft_putendl("zone type doesn't match");
		#endif
		
		ret = malloc(size);
		// TODO: cpy data
	}

	return (ret);
}