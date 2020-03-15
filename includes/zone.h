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
# ifdef __linux__
#  define __USE_GNU
# endif
# include <pthread.h>
# include "block.h"

extern pthread_mutex_t      g_mutex;
extern struct s_zone_meta   *g_first_zone;

enum                    e_zone_type
{
    TINY,
    SMALL,
    LARGE
};

struct                  s_zone_meta
{
    enum e_zone_type    type;
    size_t              size;
    struct s_zone_meta  *next;
};

# define ZONE_TO_BLOCK(zone)    ((void *)zone + sizeof(struct s_zone_meta))
# define BLOCK_TO_ZONE(block)   ((void *)block - sizeof(struct s_zone_meta))

struct s_zone_meta      *mmap_zone(size_t size);
enum e_zone_type        define_zone_type(size_t size);
struct s_zone_meta      *get_my_zone_meta(struct s_block_meta *block);
void                    unmap(struct s_block_meta *block);

# if DEBUG

struct s_zone_meta      *get_first_zone(void);

# endif

#endif
