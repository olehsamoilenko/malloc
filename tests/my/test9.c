/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test9.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/22 10:32:26 by osamoile          #+#    #+#             */
/*   Updated: 2019/12/22 10:32:35 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"

int main()
{
	// ft_memalloc(3);
	show_alloc_mem();
	show_alloc_mem_ex();
}

// void test_many_small()
// {
// 	for (int i = 0; i < 100; i++)
// 		malloc(9); // small

// 	malloc(60);

// 	for (int i = 0; i < 100; i++)
// 		malloc(9); // small

// 	malloc(60);
// 	malloc(4000);
// 	malloc(8000);
// 	// malloc(getpagesize());
// }

// void test_unmap()
// {
//     int i = 0;
//     while (++i < 1000)
//     {
//         void *a = malloc(100);
//         free(a);
//     }
// }

// void test_paging()
// {
//     void *n = malloc(1);
//     void *n2 = malloc(2);
//     void *n3 = malloc(50);
// 	void *n4 = malloc(4);

//     free(n4);
//     free(n3);
//     free(n2);
//     free(n);

//     void *n5 = malloc(10);
//     // free(n5);

//     show_alloc_mem();
//     show_alloc_mem_ex();
// }
