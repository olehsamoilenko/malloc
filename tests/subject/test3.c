/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/22 17:35:43 by osamoile          #+#    #+#             */
/*   Updated: 2019/12/22 17:35:46 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/malloc.h"
#include <unistd.h>
#include <string.h>

#define M (1024 * 1024)

void print(char *s)
{
    write(1, s, strlen(s));
}

int main()
{
    char *addr1;
    char *addr3;
    addr1 = (char*)malloc(16*M);
    strcpy(addr1, "Bonjours\n");
    print(addr1);
    addr3 = (char*)realloc(addr1, 128*M);
    addr3[127*M] = 42;
    print(addr3);
    return (0);
}

// int main()
// {
//  char *addr1;
//  char *addr2;
//  char *addr3;
//  addr1 = (char*)malloc(16*M);
//  strcpy(addr1, "Bonjours\n");
//  print(addr1);
//  addr2 = (char*)malloc(16*M);
//  addr3 = (char*)realloc(addr1, 128*M);
//  addr3[127*M] = 42;
//  print(addr3);
//  return (0);
// }
