/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/22 17:08:15 by osamoile          #+#    #+#             */
/*   Updated: 2019/12/22 17:08:23 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/malloc.h"

// TDCHECK: between 255 and 272 pages
// res: 266

int main()
{
	int i;
	char *addr;
	i = 0;
	while (i < 1024)
	{
		addr = (char*)malloc(1024);
		addr[0] = 42;
		i++;
	}
	show_alloc_mem();
	return (0);
}
