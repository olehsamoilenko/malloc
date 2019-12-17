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
#define BGRED		"\e[41m" //"\e[91m"
#define BGGREEN		"\e[30;42m" //"\e[92m"
#define BGCYAN		"\e[44m"

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

// TODO: colors
// TODO: real data
// TODO: aligning
void show_alloc_mem_ex(void)
{
	struct zone_meta *zone = get_first_zone();
	unsigned int i = 0;

    while (zone)
    {
        // zone meta
        i = -1;
        while (++i < sizeof(struct zone_meta))
            ft_printf("Z");
        ft_printf("\n");

        struct block_meta *block = FIRST_BLOCK(zone);
        while (block)
        {
            // block meta
            i = -1;
            while (++i < sizeof(struct block_meta))
                ft_printf("B");
            ft_printf("\n");
            
            // block data
            i = -1;
            while (++i < block->size)
            {
                if (block->available)
                    ft_printf(".");
                else
                    ft_printf("x");
            }
            ft_printf("\n");

            block = block->next;
        }
        

        zone = zone->next;
    }

	// 	text = (unsigned char *)block;
	// 	print_meta(block, &i, text);
    //     int data = block->size;
	// 	text = (unsigned char *)block + sizeof(struct block_meta);
    //     while (data--)
    //     {
    //         if (block->available)
	// 			ft_printf("%s", BGGREEN);
	// 		else
	// 			ft_printf("%s", BGRED);
	// 		if (ft_isprint(*text))
    //         	ft_printf("%10c", *text);
	// 		else
	// 			ft_printf("%10c", '?');
	// 		ft_printf("%s ", BGDEFAULT);
	// 		text++;
    //         if (++i % 10 == 0) {
    //             ft_printf("\n");
    //         }
    //     }
    //     // print_meta(block, &i, text);

}