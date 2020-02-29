/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/22 10:40:43 by osamoile          #+#    #+#             */
/*   Updated: 2019/12/22 10:40:45 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BLOCK_H
# define BLOCK_H

# include <sys/mman.h>
# include "utils.h"
# include "libft.h"

# define MAX_TINY_BLOCK_SIZE 128
# define MAX_SMALL_BLOCK_SIZE 1024
# define EXPORT_VOID void __attribute__((visibility("default")))

// TDCHECK: static functions

struct	s_block_meta
{
	unsigned int		available;
	size_t				size;
	struct s_block_meta	*prev;
	struct s_block_meta	*next;
};

typedef struct s_block_meta	t_block;
# define DATA_TO_META(block) ((t_block *)((void *)block - sizeof(t_block)))
# define META_TO_DATA(block) ((t_block *)((void *)block + sizeof(t_block)))

void	*alloc_on_block(struct s_block_meta *new_block, size_t size);
t_bool	block_is_allocated(struct s_block_meta *block);
void	free_allocated_block(struct s_block_meta *block, t_bool try_eat_next,
		t_bool try_eat_prev, t_bool try_unmap);

#endif
