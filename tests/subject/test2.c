/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/22 17:28:22 by osamoile          #+#    #+#             */
/*   Updated: 2019/12/22 17:28:25 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/malloc.h"

// TODO: less pages than test1
// TODO: pages = test0 + 3

int main()
{
	int i;
	char *addr;
	i = 0;
	while (i < 1024)
	{
		addr = (char*)malloc(1024);
		addr[0] = 42;
		free(addr);
		i++;
	}
	return (0);
}
