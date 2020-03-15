/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 14:14:54 by osamoile          #+#    #+#             */
/*   Updated: 2020/01/12 14:14:57 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "test.h"

int main()
{
	void *ptr;
	ptr = malloc(1024);
	ptr = malloc(1024 * 32);
	ptr = malloc(1024 * 1024);
	ptr = malloc(1024 * 1024 * 16);
	ptr = malloc(1024 * 1024 * 128);
	CHECK(
		getZonesNumber() == 5
		&& getZoneTh(1)->type == SMALL
		&& getBlocksNumber(getZoneTh(1)) == 1
		&& getZoneTh(2)->type == LARGE
		&& getBlocksNumber(getZoneTh(1)) == 1
		&& getZoneTh(3)->type == LARGE
		&& getBlocksNumber(getZoneTh(1)) == 1
		&& getZoneTh(4)->type == LARGE
		&& getBlocksNumber(getZoneTh(1)) == 1
		&& getZoneTh(5)->type == LARGE
		&& getBlocksNumber(getZoneTh(1)) == 1
	);

	(void)ptr;
	return (0);
}
