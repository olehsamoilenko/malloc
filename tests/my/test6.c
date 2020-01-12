/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 14:50:22 by osamoile          #+#    #+#             */
/*   Updated: 2020/01/12 14:50:24 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

int main(void)
{
    void *n = malloc(1);
    void *n2 = malloc(2);

	// large block
    void *n3 = malloc(50);
	ft_strcpy(n3, "hello");

	void *n4 = malloc(4);
    show_alloc_mem_ex();

    free(n4);
    free(n3);
    free(n2);

	// double free
    free(n);
	free(n);

	show_alloc_mem_ex();
}
