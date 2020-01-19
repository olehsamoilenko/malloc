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

#include "block.h"
#include "zone.h"

#define BGDEFAULT	"\e[97;49m"
#define BGRED		"\e[97;41m"
#define BGGREEN		"\e[30;42m"
#define BGYELLOW	"\e[30;43m"
#define BGCYAN		"\e[97;44m"
// TD: check vmmap

const char *labels[] = {"TINY", "SMALL", "LARGE"}; // TD: to macro

void EXPORT show_alloc_mem()
{
	#if DEBUG
		ft_putchar('\n');
	#endif

    struct zone_meta *zone = first_zone;

	unsigned long sum = 0;
    while (zone)
    {
        ft_putstr(labels[zone->type]);
        ft_putstr(" : ");
        ft_print_hex((unsigned long)zone, true);
        ft_putchar('\n');

        struct block_meta *block = ZONE_TO_BLOCK(zone);

        while (block)
        {
            #if DEBUG // TD: refactor
                if (block->available)
                    ft_putstr("[AVAILABLE] ");
            #endif

            #if !DEBUG
                if (!block->available)
                {
            #endif
                ft_print_hex((unsigned long)(char *)block + sizeof(struct block_meta), true);
                ft_putstr(" - ");
                ft_print_hex((unsigned long)(char *)block + sizeof(struct block_meta) + block->size, true);
                ft_putstr(" : ");
                ft_putnbr(block->size);
                ft_putstr(" bytes\n");
            #if !DEBUG
                }
            #endif

            if (!block->available)
                sum += block->size;

            block = block->next;
        }

        zone = zone->next;
    }

    ft_putstr("Total : ");
    ft_putnbr(sum);
    ft_putchar('\n');

	#if DEBUG
		ft_putchar('\n');
	#endif
}

t_bool print_symbol(unsigned char sym, t_bool in_hex, char *color)
{
    static int counter = 0;
    static char *g_color = BGDEFAULT;
	t_bool line_completed = true;

    if (!ft_strequ(color, g_color))
    {
        g_color = color;
        ft_putstr(color);
    }

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

void EXPORT show_alloc_mem_ex(void)
{
	struct zone_meta *zone = first_zone;
	unsigned int i;
	t_bool line_completed = true;
	unsigned char *data;

    while (zone)
    {
        i = -1;
		data = (unsigned char *)zone;
        while (++i < sizeof(struct zone_meta))
            line_completed = print_symbol(data[i], true, BGYELLOW);

        struct block_meta *block = ZONE_TO_BLOCK(zone);
        while (block)
        {
			data = (unsigned char *)block;

            i = -1;
            while (++i < sizeof(struct block_meta))
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

            block = block->next;
        }

        zone = zone->next;
    }

	while (!line_completed)
		line_completed = print_symbol(' ', false, BGDEFAULT);
}