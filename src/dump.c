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

#define BGDEFAULT	"\e[97;49m"
#define BGRED		"\e[97;41m"
#define BGGREEN		"\e[30;42m"
#define BGCYAN		"\e[97;44m"
// TODO: new color for zone_meta

const char *labels[] = {"TINY", "SMALL", "LARGE"}; // TODO: to macro

void EXPORT show_alloc_mem() // TODO: shit with debug
{
    struct zone_meta *zone = first_zone;

	unsigned long sum = 0;
    while (zone)
    {
        ft_putstr(labels[zone->type]);
        ft_putstr(" : ");
        ft_print_hex((unsigned long)zone);
        ft_putchar('\n');

        struct block_meta *block = FIRST_BLOCK(zone);

        while (block)
        {
            #if DEBUG // TODO: refactor
                if (block->available)
                    ft_putstr("[AVAILABLE] ");
            #endif

            #if !DEBUG
                if (!block->available)
                {
            #endif
                ft_print_hex((unsigned long)(char *)block + sizeof(struct block_meta));
                ft_putstr(" - ");
                ft_print_hex((unsigned long)(char *)block + sizeof(struct block_meta) + block->size);
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
}

void print_symbol(char sym, char *color)
{
    static int counter = 0;
    static char *g_color = BGDEFAULT;

    if (!ft_strequ(color, g_color))
    {
        g_color = color;
        ft_putstr(color);
    }
    ft_putchar(sym);
    counter++;
    if (counter % 100 == 0)
    {
        ft_putstr(BGDEFAULT);
        ft_putchar('\n');
        ft_putstr(g_color);
    }
}

// TODO: real data text = (unsigned char *)block;
void EXPORT show_alloc_mem_ex(void)
{
	struct zone_meta *zone = first_zone;
	unsigned int i = 0;

    while (zone)
    {
        // zone meta
        i = -1;
        while (++i < sizeof(struct zone_meta))
            print_symbol('Z', BGCYAN);

        struct block_meta *block = FIRST_BLOCK(zone);
        while (block)
        {
            // block meta
            i = -1;
            while (++i < sizeof(struct block_meta))
                print_symbol('B', BGCYAN);

            // block data
            i = -1;
            while (++i < block->size)
            {
                if (block->available)
                    print_symbol('.', BGGREEN);
                else
                    print_symbol('x', BGRED);
            }

            block = block->next;
        }

        zone = zone->next;
    }

    ft_putstr(BGDEFAULT);
    ft_putchar('\n');
}