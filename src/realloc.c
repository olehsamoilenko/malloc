/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 09:37:16 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 09:37:23 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block.h"

// TODO: new size > old size
// TODO: old size > new size
// TODO: If ptr is NULL, then the call is equivalent to malloc(size)
// TODO: if size is equal to zero, and ptr is not NULL, then the call is equivalent to free(ptr)
// TODO: If the area pointed to was moved, a free(ptr) is done.
// TODO: The UNIX 98 standard requires malloc(), calloc(), and realloc() to set errno to ENOMEM upon failure
// TODO: If realloc() fails the original block is left untouched; it is not freed or moved.

void *realloc(void *ptr, size_t size)
{
	return (NULL);
}