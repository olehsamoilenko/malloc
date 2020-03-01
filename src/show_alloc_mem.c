/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:13:20 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 16:13:22 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zone.h"

static size_t	print_block_info(struct s_block_meta *block)
{
	size_t res;

	res = 0;
	if (block->available == false)
	{
		ft_print_hex((size_t)(char *)block
							+ sizeof(struct s_block_meta), true);
		ft_putstr(" - ");
		ft_print_hex((size_t)(char *)block
							+ sizeof(struct s_block_meta) + block->size, true);
		ft_putstr(" : ");
		ft_putnbr(block->size);
		ft_putstr(" bytes\n");
		res = block->size;
	}
	return (res);
}

static void		print_zone_info(struct s_zone_meta *zone)
{
	char *label;

	if (zone->type == TINY)
		label = "TINY";
	else if (zone->type == SMALL)
		label = "SMALL";
	else
		label = "LARGE";
	ft_putstr(label);
	ft_putstr(" : ");
	ft_print_hex((unsigned long)zone, true);
	ft_putchar('\n');
}

void			print_decimal_value(char *str, int value)
{
	if (DEBUG)
	{
		ft_putstr(str);
		ft_putnbr(value);
		ft_putchar('\n');
	}
}

void			print_hex_value(char *str, unsigned long value)
{
	if (DEBUG)
	{
		ft_putstr(str);
		ft_print_hex(value, true);
		ft_putchar('\n');
	}
}

EXPORT_VOID		show_alloc_mem(void)
{
	struct s_zone_meta	*zone;
	size_t				sum;
	struct s_block_meta	*block;

	pthread_mutex_lock(&g_mutex);
	zone = g_first_zone;
	sum = 0;
	while (zone)
	{
		print_zone_info(zone);
		block = ZONE_TO_BLOCK(zone);
		while (block)
		{
			sum += print_block_info(block);
			block = block->next;
		}
		zone = zone->next;
	}
	ft_putstr("Total : ");
	ft_putnbr(sum);
	ft_putchar('\n');
	pthread_mutex_unlock(&g_mutex);
}
