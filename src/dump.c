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

#include "malloc.h"

#define BGDEFAULT	"\e[97;49m"
#define BGRED		"\e[97;41m"
#define BGGREEN		"\e[30;42m"
#define BGCYAN		"\e[97;44m"
// TODO: new color for zone_meta

extern struct block_meta *last_valid_address;

const char *labels[] = {"TINY", "SMALL", "LARGE"}; // TODO: to macro

void show_alloc_mem()
{
    struct zone_meta *zone = get_first_zone();

	unsigned long sum = 0;
    while (zone)
    {
        ft_printf("%s : 0x%lX\n", labels[zone->type], (unsigned long)zone); // TODO: check output
        struct block_meta *block = FIRST_BLOCK(zone);

        while (block)
        {
            #if DEBUG // TODO: refactor
                if (block->available)
                    ft_printf("[AVAILABLE] ");
            #endif

            #if !DEBUG
                if (!block->available)
                {
            #endif
                ft_printf("0x%lX - 0x%lX : %u bytes\n",
                        (unsigned long)(char *)block + sizeof(struct block_meta),
                        (unsigned long)(char *)block + sizeof(struct block_meta) + block->size,
                        block->size);
            #if !DEBUG
                }
            #endif

            if (!block->available)
                sum += block->size;

            block = block->next;
        }

        zone = zone->next;
    }


    ft_printf("Total : %lu\n", sum);

}

void print_meta(struct block_meta *block, unsigned int *counter, unsigned char *text)
{
	unsigned int len;
	
	len = sizeof(struct block_meta);
	char *cur_color;

	while (len--)
	{
		if (!len)
		{
			ft_printf("%s AV: %4d %s ", BGCYAN, block->available, BGDEFAULT);
			cur_color = BGDEFAULT;
		}
		else
		{
			ft_printf("%s%10c ", BGCYAN, ' ');
			cur_color = BGCYAN;
		}
		*counter += 1;
		if (*counter % 10 == 0) {
			ft_printf("%s\n%s", BGDEFAULT, cur_color);
		}
	}
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

// TODO: real data
void show_alloc_mem_ex(void)
{
	struct zone_meta *zone = get_first_zone();
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

	// text = (unsigned char *)block;
    // if (ft_isprint(*text))
    //     ft_printf("%10c", *text);
    // else
    //     ft_printf("%10c", '?');
    // text++;

}