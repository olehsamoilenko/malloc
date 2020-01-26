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
#include "test.h"

int main(void)
{
	void *n = malloc(1);
	void *n2 = malloc(2);
	void *n3 = malloc(5000); // large block
	void *n4 = malloc(4);
	CHECK(
		getZonesNumber() == 2
		&& getZoneTh(1)->type == TINY
		&& getBlocksNumber(getZoneTh(1)) == 3
		&& getZoneTh(2)->type == LARGE
		&& getBlocksNumber(getZoneTh(2)) == 1
	);
	// TINY : 0x10d179000
	// 0x10d179030 - 0x10d179031 : 1 bytes
	// 0x10d179049 - 0x10d17904b : 2 bytes
	// 0x10d179063 - 0x10d179067 : 4 bytes
	// LARGE : 0x10d166000
	// 0x10d166030 - 0x10d1673b8 : 5000 bytes
	// Total : 5007

	free(n4);
	free(n3);
	free(n2);
	free(n);
	free(n); // double free
	CHECK(
		getZonesNumber() == 0
	);
	// Total : 0
}
