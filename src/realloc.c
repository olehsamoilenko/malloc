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

// TODO: if size is equal to zero, and ptr is not NULL, then the call is equivalent to free(ptr)
// TODO: The UNIX 98 standard requires malloc(), calloc(), and realloc() to set errno to ENOMEM upon failure
// TODO: If realloc() fails the original block is left untouched; it is not freed or moved.

void EXPORT *realloc(void *ptr, size_t size)
{
	#if DEBUG
        ft_putstr("[CALL] realloc: ");
        ft_print_hex((unsigned long)ptr, true);
        ft_putchar('\n');
    #endif

	void *ret;

	struct block_meta *b = DATA_TO_META(ptr);
	if (block_is_allocated(b))
	{
		enum zone_type new_type = define_zone_type(size);
		struct zone_meta *my_zone = get_my_zone_meta(b);

		free_allocated_block(b, true, false, false);

		if (b->size >= size && my_zone->type == new_type)
		{
			#if DEBUG
				ft_putendl("[REALLOC] Checking... Block is suitable");
			#endif
		
			// TODO: check with old size > new size
			alloc_on_block(b, size);
			ret = META_TO_DATA(b);
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
			
			// TODO: cpy data
			free_allocated_block(b, false, true, true);
			ret = malloc(size);
		}
	}
	else if (ptr == NULL)
	{
		ret = malloc(size);
	}
	else
	{
		ret = NULL;
		// TODO: check man
	}

	return (ret);
}