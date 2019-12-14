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

extern struct metadata *last_valid_address;

const char *labels[] = {"TINY", "SMALL", "LARGE"};

void show_alloc_mem()
{
	struct metadata *block = (struct metadata *)get_memory_start();


	int current_type = -1;
	unsigned long sum = 0;

    while (1)
    {
		if (current_type != block->type)
		{
			printf("%s : 0x%lX\n", labels[block->type], (unsigned long)block);
			current_type = block->type;
		}

		if (!block->available)
		{
			printf("0x%lX - 0x%lX : %u bytes\n",
				(unsigned long)(char *)block + sizeof(struct metadata),
				(unsigned long)(char *)block + sizeof(struct metadata) + block->size,
				block->size);
			sum += block->size;
		}
		else // TODO: remove
		{
			printf("Available : %u bytes\n", block->size);
		}

        block = block->next;
		if (!block)
		{
			printf("Total : %lu", sum);
			break ;
		}
    }

    printf("\n");
}

void print_meta(struct metadata *block, unsigned int *counter, unsigned char *text)
{
	unsigned int len;
	
	len = sizeof(block->available);
	char *cur_color;

	while (len--)
	{
		if (!len)
		{
			printf("%s AV: %4d %s ", BGCYAN, block->available, BGDEFAULT);
			cur_color = BGDEFAULT;
		}
		else
		{
			printf("%s%10c ", BGCYAN, ' ');
			cur_color = BGCYAN;
		}
		*counter += 1;
		if (*counter % 10 == 0) {
			printf("%s\n%s", BGDEFAULT, cur_color);
		}
	}

	len = sizeof(block->size);
	while (len--)
	{
		if (!len)
		{
			printf("%s S: %5d %s ", BGCYAN, block->size, BGDEFAULT);
			cur_color = BGDEFAULT;
		}
		else
		{
			printf("%s%10c ", BGCYAN, ' ');
			cur_color = BGCYAN;
		}
		*counter += 1;
		if (*counter % 10 == 0) {
			printf("%s\n%s", BGDEFAULT, cur_color);
		}
	}

	len = sizeof(block->type);
	while (len--)
	{
		if (!len)
		{
			printf("%s%9s %s ", BGCYAN, labels[block->type], BGDEFAULT);
			cur_color = BGDEFAULT;
		}
		else
		{
			printf("%s%10c ", BGCYAN, ' ');
			cur_color = BGCYAN;
		}
		*counter += 1;
		if (*counter % 10 == 0) {
			printf("%s\n%s", BGDEFAULT, cur_color);
		}
	}
}

void show_alloc_mem_ex(void)
{
	printf("\n");
	struct metadata *block = (struct metadata *)get_memory_start();
	unsigned int i = 0;
	unsigned char *text;

    while (1)
    {
		text = (unsigned char *)block;

		print_meta(block, &i, text);

        int data = block->size;
		text = (unsigned char *)block + sizeof(struct metadata);
        while (data--)
        {
            if (block->available)
				printf("%s", BGGREEN);
			else
				printf("%s", BGRED);

			if (ft_isprint(*text))
            	printf("%10c", *text);
			else
				printf("%10c", '?');

			printf("%s ", BGDEFAULT);
			text++;

            if (++i % 10 == 0) {
                printf("\n");
            }
        }

        print_meta(block, &i, text);

        block = block->next;
		if (!block)
			break ;
    }

    printf("\n");
}