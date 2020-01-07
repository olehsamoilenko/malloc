/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test8.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/22 10:32:26 by osamoile          #+#    #+#             */
/*   Updated: 2019/12/22 10:32:35 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "malloc.h"

// large block

int main()
{
	void *m1 = malloc(1);
	void *m2 = malloc(100);
	void *m3 = malloc(10);
	void *m22 = malloc(100);
    show_alloc_mem();

	free(m2);
	show_alloc_mem();
}
