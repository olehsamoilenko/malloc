/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 09:37:16 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 09:37:23 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zone.h"

#if DEBUG

# define EXPORT_META struct s_zone_meta __attribute__((visibility("default")))

EXPORT_META         *get_first_zone(void)
{
    return (g_first_zone);
}

#endif

static size_t       calculate_zone_size(int blocks, int max_size)
{
    return (blocks * (max_size + sizeof(struct s_block_meta))
            + sizeof(struct s_zone_meta));
}

static size_t       bytes_to_request(size_t size)
{
    const enum e_zone_type  zone_type = define_zone_type(size);
    size_t                  bytes_need;
    size_t                  pages;
    size_t                  bytes_to_request;

    if (zone_type == TINY)
        bytes_need = calculate_zone_size(BLOCKS_IN_ZONE, MAX_TINY_BLOCK_SIZE);
    else if (zone_type == SMALL)
        bytes_need = calculate_zone_size(BLOCKS_IN_ZONE, MAX_SMALL_BLOCK_SIZE);
    else
        bytes_need = calculate_zone_size(1, size);
    pages = (bytes_need - 1) / getpagesize() + 1;
    bytes_to_request = pages * getpagesize();
    if (DEBUG)
    {
        ft_putstr("[PAGING] Bytes need: ");
        ft_putnbr(bytes_need);
        ft_putstr(", bytes to request: ");
        ft_putnbr(bytes_to_request);
        ft_putstr(" (");
        ft_putnbr(pages);
        ft_putendl(" pages)");
    }
    return (bytes_to_request);
}

static void         create_first_block(struct s_zone_meta *zone, size_t z_size)
{
    struct s_block_meta *first_block;

    first_block = ZONE_TO_BLOCK(zone);
    first_block->available = true;
    first_block->size = z_size
                        - sizeof(struct s_zone_meta)
                        - sizeof(struct s_block_meta);
    first_block->next = NULL;
    first_block->prev = NULL;
}

struct s_zone_meta  *mmap_zone(size_t size)
{
    const size_t        to_request = bytes_to_request(size);
    struct s_zone_meta  *zone;

    zone = mmap(NULL, to_request, PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANON, -1, 0);
    if (zone == MAP_FAILED)
    {
        if (DEBUG)
            ft_putendl("[PAGING] mmap failed");
        return (NULL);
    }
    zone->type = define_zone_type(size);
    zone->size = to_request;
    zone->next = NULL;
    create_first_block(zone, to_request);
    if (DEBUG)
    {
        ft_putstr("[PAGING] Zone mapped, size = ");
        ft_putnbr(to_request);
        ft_putstr(", zone start = ");
        ft_print_hex((unsigned long)zone, true);
        ft_putchar('\n');
    }
    return (zone);
}
