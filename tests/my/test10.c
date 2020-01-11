/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test10.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/22 10:32:26 by osamoile          #+#    #+#             */
/*   Updated: 2019/12/22 10:32:35 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

// realloc initial

int main()
{
	void *m = malloc(6);
	void *m2 = malloc(6);
	show_alloc_mem();
	void *r = realloc(m2, 10);
	ft_putendl("");
	show_alloc_mem();
	void *r2 = realloc(r, 20);
	ft_putendl("");
	show_alloc_mem();
}
