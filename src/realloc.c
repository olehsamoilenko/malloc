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

#include "zone.h"

EXPORT_VOID *realloc(void *ptr, size_t size)
{
	pthread_mutex_lock(&g_mutex);

	#if DEBUG
		ft_putstr("[CALL] realloc: ");
		ft_print_hex((unsigned long)ptr, true);
		ft_putchar('\n');
	#endif

	void *ret = NULL;

	struct s_block_meta *b = DATA_TO_META(ptr);
	if (block_is_allocated(b))
	{
		enum e_zone_type new_type = define_zone_type(size);
		struct s_zone_meta *my_zone = get_my_zone_meta(b);

		size_t old_size = b->size;
		free_allocated_block(b, true, false, false);

		if (size == 0)
		{
			/* MAN: if size is equal to zero, and ptr is not NULL,
			then the call is equivalent to free(ptr) */
			free(ptr);
		}
		else if (b->size >= size && my_zone->type == new_type)
		{
			#if DEBUG
				ft_putendl("[REALLOC] Block is suitable");
			#endif
		
			alloc_on_block(b, size);
			ret = META_TO_DATA(b);
		}
		else
		{
			#if DEBUG
				ft_putstr("[REALLOC] Block is not suitable: ");
				if (b->size < size)
					ft_putendl("size is too large");
				else if (my_zone->type != new_type)
					ft_putendl("zone type doesn't match");
			#endif

			ret = malloc(size);
			size_t cpy_size = old_size < size ? old_size : size;
			#if DEBUG
				ft_putstr("[REALLOC] Copying ");
				ft_putnbr(cpy_size);
				ft_putendl(" bytes");
			#endif
			ft_memcpy(ret, ptr, cpy_size);
			free_allocated_block(b, false, true, true);
		}
	}
	else if (ptr == NULL)
	{
		ret = malloc(size);
	}

	#if DEBUG
		ft_putstr("[RETURN] realloc: ");
		ft_print_hex((unsigned long)ret, true);
		ft_putchar('\n');
	#endif

	pthread_mutex_unlock(&g_mutex);
	return (ret);
}