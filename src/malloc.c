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

struct s_zone_meta	*g_first_zone = NULL;
# ifdef __linux__
pthread_mutex_t		g_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
# else
pthread_mutex_t		g_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
# endif

enum e_zone_type	define_zone_type(size_t size)
{
	enum e_zone_type type;

	if (size <= MAX_TINY_BLOCK_SIZE)
	{
		type = TINY;
		if (DEBUG)
			ft_putstr("[BLOCK] Block type: TINY\n");
	}
	else if (size <= MAX_SMALL_BLOCK_SIZE)
	{
		type = SMALL;
		if (DEBUG)
			ft_putstr("[BLOCK] Block type: SMALL\n");
	}
	else
	{
		type = LARGE;
		if (DEBUG)
			ft_putstr("[BLOCK] Block type: LARGE\n");
	}
	return (type);
}

static void			drop_info(struct s_block_meta *nb,
					struct s_block_meta *rb)
{
	if (DEBUG)
	{
		ft_putstr("[ALLOC] New: ");
		ft_print_hex((unsigned long)nb, true);
		ft_putstr(" (");
		ft_putnbr(nb->size);
		ft_putstr(" bytes), reduced: ");
		ft_print_hex((unsigned long)rb, true);
		ft_putstr(" (");
		ft_putnbr(rb->size);
		ft_putendl(" bytes)");
	}
}

void				alloc_on_block(struct s_block_meta *new_blk, size_t size)
{
	struct s_block_meta *reduced_blk;
	struct s_block_meta *nextnext;

	if (new_blk && new_blk->size >= size + sizeof(struct s_block_meta))
	{
		reduced_blk = (void *)new_blk + sizeof(struct s_block_meta) + size;
		reduced_blk->available = true;
		reduced_blk->size = new_blk->size - sizeof(struct s_block_meta) - size;
		reduced_blk->next = new_blk->next;
		reduced_blk->prev = new_blk;
		nextnext = reduced_blk->next;
		if (nextnext)
			nextnext->prev = reduced_blk;
		new_blk->available = false;
		new_blk->size = size;
		new_blk->next = reduced_blk;
		drop_info(new_blk, reduced_blk);
	}
	else if (new_blk && new_blk->size >= size)
	{
		if (DEBUG)
			ft_putstr("[ALLOC] Take last block in zone\n");
		new_blk->available = false;
	}
}

EXPORT_VOID			*malloc(size_t size)
{
	void				*ret;
	struct s_block_meta	*new_block;

	pthread_mutex_lock(&g_mutex);
	print_decimal_value("[CALL] malloc: ", size);
	new_block = get_suitable_block(size);
	if (new_block && new_block->size >= size)
	{
		alloc_on_block(new_block, size);
		ret = META_TO_DATA(new_block);
	}
	else
	{
		if (DEBUG)
			ft_putstr("[FAIL] No suitable block\n");
		ret = NULL;
	}
	print_hex_value("[RETURN] malloc: ", (unsigned long)ret);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}
