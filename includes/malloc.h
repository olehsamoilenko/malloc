/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 09:38:18 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 09:38:27 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

void myfree(void *p);
void *myalloc(unsigned long size); // TODO check: unsigned long ?
void show_alloc_mem(void);
void show_alloc_mem_ex(void);

#endif
