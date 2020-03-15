/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 09:37:16 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 09:37:23 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zone.h"

static struct s_block_meta  *eat_prev(struct s_block_meta *block)
{
    void                *res;
    struct s_block_meta *prev;
    struct s_block_meta *next;

    res = block;
    prev = block->prev;
    if (prev && prev->available)
    {
        if (DEBUG)
            ft_putstr("[FREE] Eating previous block\n");
        prev->size += block->size + sizeof(struct s_block_meta);
        prev->next = block->next;
        next = block->next;
        if (next)
            next->prev = prev;
        res = prev;
    }
    return (res);
}

static void                 eat_next(struct s_block_meta *block)
{
    struct s_block_meta *next;
    struct s_block_meta *nextnext;

    next = block->next;
    if (next && next->available == true)
    {
        if (DEBUG)
            ft_putstr("[FREE] Eating next block\n");
        next->available = false;
        nextnext = next->next;
        if (nextnext)
            nextnext->prev = block;
        block->next = nextnext;
        block->size += next->size + sizeof(struct s_block_meta);
    }
}

void                        free_allocated_block(struct s_block_meta *block,
                            t_bool try_eat_next,
                            t_bool try_eat_prev,
                            t_bool try_unmap)
{
    if (try_eat_next)
        eat_next(block);
    if (try_eat_prev == true)
        block = eat_prev(block);
    block->available = true;
    if (try_unmap == true)
        unmap(block);
}

EXPORT_VOID                 free(void *p)
{
    struct s_block_meta *block;

    pthread_mutex_lock(&g_mutex);
    print_hex_value("[CALL] free: ", (unsigned long)p);
    block = DATA_TO_META(p);
    if (p && block_is_allocated(block))
        free_allocated_block(block, true, true, true);
    pthread_mutex_unlock(&g_mutex);
}
