/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/26 16:36:52 by osamoile          #+#    #+#             */
/*   Updated: 2020/01/26 16:36:54 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# if DEBUG

# include "zone.h"

# define OFF "\033[0m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"

# define CHECK(cond) ({						\
	t_bool res = false;						\
	if (cond) {								\
		res = true;							\
	}										\
	ft_putstr("[TEST] ");					\
	ft_putstr(__func__);					\
	ft_putstr(": ");						\
	if (res) {								\
		ft_putstr(GREEN);					\
		ft_putstr("OK");					\
	}										\
	else {									\
		ft_putstr(RED);						\
		ft_putstr("KO");					\
	}										\
	ft_putendl(OFF);						\
})

# define TEAR_DOWN() ({\
	CHECK(get_first_zone() == NULL); \
	struct s_zone_meta *zone;\
	while ((zone = get_first_zone()))\
	{\
		struct s_block_meta *block = ZONE_TO_BLOCK(zone);\
		while (block)\
		{\
			if (!block->available)\
			{\
				free(META_TO_DATA(block));\
				break ;\
			}\
			block = block->next;\
		}\
	}\
})

int getZonesNumber(void)
{
	int cnt = 0;

	struct s_zone_meta *zone = get_first_zone();
	
	while (zone)
	{
		cnt += 1;
		zone = zone->next;
	}

	return cnt;
}

struct s_zone_meta *getZoneTh(int th)
{
	struct s_zone_meta *tmp = get_first_zone();
	struct s_zone_meta *res = NULL;

	if (getZonesNumber() >= th)
	{
		int i = 0;
		while (tmp && ++i < th)
		{
			tmp = tmp->next;
		}
		res = tmp;
	}

	return res;
}

int getBlocksNumber(struct s_zone_meta *zone)
{
	struct s_block_meta *block = ZONE_TO_BLOCK(zone);
	int cnt = 0;

	while (block)
	{
		if (block->available == false)
			cnt += 1;
		block = block->next;

	}

	ft_putstr("[TEST] getBlocksNumber: ");
	ft_putnbr(cnt);
	ft_putchar('\n');

	return cnt;
}

t_bool cmpContent(void *content, const char *data)
{
	struct s_block_meta *block = DATA_TO_META(content);
	int len = block->size < ft_strlen(data) ? block->size : ft_strlen(data);
	if (ft_strnequ(content, data, len))
		return true;
	else
		return false;
}

# else

# define CHECK(cond) ({						\
	show_alloc_mem();						\
})

# define TEAR_DOWN() ({})

# endif

#endif
