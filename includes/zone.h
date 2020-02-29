/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/22 10:40:43 by osamoile          #+#    #+#             */
/*   Updated: 2019/12/22 10:40:45 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZONE_H
# define ZONE_H

# define BLOCKS_IN_ZONE 100

# include <stdlib.h>
# include <pthread.h>
# include "block.h"

extern pthread_mutex_t		g_mutex;
extern struct s_zone_meta	*first_zone;

enum					e_zone_type
{
	TINY,
	SMALL,
	LARGE
};

struct					s_zone_meta
{
	enum e_zone_type	type;
	size_t				size;
	struct s_zone_meta	*next;
};

typedef struct s_zone_meta	t_zone;
# define ZONE_TO_BLOCK(zone)	((t_block *)((char *)zone + sizeof(t_zone)))
# define BLOCK_TO_ZONE(block)	((t_zone *)((char *)block - sizeof(t_zone)))

struct s_zone_meta		*mmap_zone(size_t size);
void					insert_zone_to_list(struct s_zone_meta *zone);
enum e_zone_type		define_zone_type(size_t size);
struct s_zone_meta		*get_my_zone_meta(struct s_block_meta *block);

# if DEBUG

struct s_zone_meta		*get_first_zone(void);
# endif

#endif
