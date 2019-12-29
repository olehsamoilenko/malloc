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

int main()
{
	ft_putstr("TEST\n");
	void *caller = __builtin_return_address(0);
	ft_putnbr(caller);
	ft_putstr("\n");

	void *tmp = malloc(1);
	free(tmp);
}
