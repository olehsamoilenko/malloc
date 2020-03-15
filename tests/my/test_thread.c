/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 14:14:54 by osamoile          #+#    #+#             */
/*   Updated: 2020/01/12 14:14:57 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <pthread.h>

void *thread_func()
{
    char *str;
    for (int i = 0; i < 500; i++)
    {
        str = malloc(8);
        str = realloc(str, 16);
        free(str);
    }
    return (NULL);
}


int main()
{
    pthread_t thread_id[42];

    for (int i = 0; i < 42; i++)
    {
        pthread_create(&thread_id[i], NULL, thread_func, NULL);
    }
    for (int i = 0; i < 42; i++)
    {
        pthread_join(thread_id[i], NULL);
    }

    return (0);
}
