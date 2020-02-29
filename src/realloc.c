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

void		alloc_on_block(struct s_block_meta *new_block, size_t size);

static void	*malloc_and_cpy(struct s_block_meta *block,
	size_t size, size_t old_size, void *ptr)
{
	void	*ret;
	size_t	cpy_size;

	ret = malloc(size);
	cpy_size = old_size < size ? old_size : size;
	print_decimal_value("[REALLOC] Copying ", cpy_size);
	ft_memcpy(ret, ptr, cpy_size);
	free_allocated_block(block, false, true, true);
	return (ret);
}

static void	dump_info(struct s_block_meta *block,
	size_t size, enum e_zone_type new_type)
{
	if (DEBUG)
	{
		ft_putstr("[REALLOC] Block is not suitable: ");
		if (block->size < size)
			ft_putendl("size is too large");
		else if (new_type != define_zone_type(size))
			ft_putendl("zone type doesn't match");
	}
}

static void	*realloc_impl(struct s_block_meta *block, size_t size, void *ptr)
{
	struct s_zone_meta	*my_zone;
	const size_t		old_size = block->size;
	void				*ret;

	ret = NULL;
	my_zone = get_my_zone_meta(block);
	free_allocated_block(block, true, false, false);
	if (size == 0)
		free(ptr);
	else if (block->size >= size && my_zone->type == define_zone_type(size))
	{
		if (DEBUG)
			ft_putendl("[REALLOC] Block is suitable");
		alloc_on_block(block, size);
		ret = META_TO_DATA(block);
	}
	else
	{
		dump_info(block, size, my_zone->type);
		ret = malloc_and_cpy(block, size, old_size, ptr);
	}
	return (ret);
}

EXPORT_VOID	*realloc(void *ptr, size_t size)
{
	void				*ret;
	struct s_block_meta	*block;

	pthread_mutex_lock(&g_mutex);
	print_hex_value("[CALL] realloc: ", (unsigned long)ptr);
	ret = NULL;
	block = DATA_TO_META(ptr);
	if (block_is_allocated(block))
	{
		ret = realloc_impl(block, size, ptr);
	}
	else if (ptr == NULL)
		ret = malloc(size);
	print_hex_value("[RETURN] realloc: ", (unsigned long)ret);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}
