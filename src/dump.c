/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:13:20 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 16:13:22 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zone.h"

#define BGDEFAULT	"\e[97;49m"
#define BGRED		"\e[97;41m"
#define BGGREEN		"\e[30;42m"
#define BGYELLOW	"\e[30;43m"
#define BGCYAN		"\e[97;44m"

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

EXPORT_VOID		show_alloc_mem(void)
{
	struct s_zone_meta	*zone;
	size_t				sum;
	struct s_block_meta	*block;

	pthread_mutex_lock(&g_mutex);
	zone = first_zone;
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
	if (DEBUG)
		ft_putchar('\n');
	pthread_mutex_unlock(&g_mutex);
}

static void		print_symbol_impl(unsigned char sym, t_bool in_hex)
{
	if (in_hex)
	{
		if (sym <= 0x0f)
			ft_putchar(' ');
		ft_print_hex(sym, false);
	}
	else
	{
		ft_putchar(' ');
		if (sym == 0)
			ft_putchar('.');
		else if (ft_isprint(sym))
			ft_putchar(sym);
		else
			ft_putchar('?');
	}
}

static t_bool	print_symbol(unsigned char sym, t_bool in_hex, char *color)
{
	static int	counter = 0;
	static char	*g_color = BGDEFAULT;
	t_bool		line_completed;

	line_completed = true;
	if (!ft_strequ(color, g_color))
	{
		g_color = color;
		ft_putstr(color);
	}
	print_symbol_impl(sym, in_hex);
	ft_putchar(' ');
	counter += 3;
	line_completed = false;
	if (counter % 99 == 0)
	{
		ft_putstr(BGDEFAULT);
		ft_putchar('\n');
		ft_putstr(g_color);
		line_completed = true;
		counter = 0;
	}
	return (line_completed);
}

static t_bool	print_block(struct s_block_meta *block)
{
	unsigned char		*data;
	unsigned int		i;
	t_bool				line_completed;

	data = (unsigned char *)block;
	i = -1;
	while (++i < sizeof(struct s_block_meta))
		line_completed = print_symbol(data[i], true, BGCYAN);
	i = -1;
	data = (unsigned char *)META_TO_DATA(block);
	while (++i < block->size)
	{
		if (block->available)
			line_completed = print_symbol(data[i], false, BGGREEN);
		else
			line_completed = print_symbol(data[i], false, BGRED);
	}
	return (line_completed);
}

EXPORT_VOID		show_alloc_mem_ex(void)
{
	struct s_zone_meta	*zone;
	unsigned int		i;
	t_bool				line_completed;
	unsigned char		*data;
	struct s_block_meta *block;

	line_completed = true;
	zone = first_zone;
	while (zone)
	{
		i = -1;
		data = (unsigned char *)zone;
		while (++i < sizeof(struct s_zone_meta))
			line_completed = print_symbol(data[i], true, BGYELLOW);
		block = ZONE_TO_BLOCK(zone);
		while (block)
		{
			line_completed = print_block(block);
			block = block->next;
		}
		zone = zone->next;
	}
	while (line_completed == false)
		line_completed = print_symbol(' ', false, BGDEFAULT);
}
