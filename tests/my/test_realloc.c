/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_realloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/22 10:32:26 by osamoile          #+#    #+#             */
/*   Updated: 2019/12/22 10:32:35 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "libft.h"
#include "test.h"

void change_zone(void)
{
    void *m = malloc(6);
    void *m2 = malloc(6);

    CHECK(
        getZonesNumber() == 1
        && getZoneTh(1)->type == TINY
        && getBlocksNumber(getZoneTh(1)) == 2
    );
    /*
     * TINY : 0x105989000
     * 0x105989030 - 0x105989036 : 6 bytes
     * 0x10598904e - 0x105989054 : 6 bytes
     * Total : 12
     */

    void *r = realloc(m2, 200);

    CHECK(
        getZonesNumber() == 2
        && getZoneTh(1)->type == TINY
        && getBlocksNumber(getZoneTh(1)) == 1
        && getZoneTh(2)->type == SMALL
        && getBlocksNumber(getZoneTh(2)) == 1
    );
    /*
     * TINY : 0x105989000
     * 0x105989030 - 0x105989036 : 6 bytes
     * SMALL : 0x10598d000
     * 0x10598d030 - 0x10598d0f8 : 200 bytes
     * Total : 206
     */

    void *r2 = realloc(r, 2000);
    CHECK(
        getZonesNumber() == 2
        && getZoneTh(1)->type == TINY
        && getBlocksNumber(getZoneTh(1)) == 1
        && getZoneTh(2)->type == LARGE
        && getBlocksNumber(getZoneTh(2)) == 1
    );
    /*
     * TINY : 0x105989000
     * 0x105989030 - 0x105989036 : 6 bytes
     * LARGE : 0x1059a7000
     * 0x1059a7030 - 0x1059a7800 : 2000 bytes
     * Total : 2006
     */

    free(m);
    free(r2);
}

void eat_prev(void)
{
    void *m = malloc(6);
    void *m2 = malloc(6);
    void *m3 = malloc(6);
    free(m2);

    CHECK(
        getZonesNumber() == 1
        && getZoneTh(1)->type == TINY
        && getBlocksNumber(getZoneTh(1)) == 2
    );
    /*
     * TINY : 0x103db1000
     * 0x103db1030 - 0x103db1036 : 6 bytes
     * 0x103db106c - 0x103db1072 : 6 bytes
     * Total : 12
     */

    void *r = realloc(m3, 10);
    CHECK(
        getZonesNumber() == 1
        && getZoneTh(1)->type == TINY
        && getBlocksNumber(getZoneTh(1)) == 2
    );
    /*
     * TINY : 0x103db1000
     * 0x103db1030 - 0x103db1036 : 6 bytes
     * 0x103db106c - 0x103db1076 : 10 bytes
     * Total : 16
     */

    free(r);
    free(m);
}

void unmap_vulnerability(void)
{
    void *m = malloc(6);
    void *r = realloc(m, 10);
    CHECK(
        getZonesNumber() == 1
        && getZoneTh(1)->type == TINY
        && getBlocksNumber(getZoneTh(1)) == 1
    );
    /*
     * TINY : 0x105686000
     * 0x105686030 - 0x10568603a : 10 bytes
     * Total : 10
     */

    free(r);
}

void malloc_free_prev(void)
{
    /* data may be corrupted
    when prev block is eaten
    and new is allocated on
    his place */
    const char *str = "hello";

    void *m = malloc(6);
    void *m2 = malloc(6);
    ft_strcpy(m2, str);
    void *m3 = malloc(6);
    free(m);
    CHECK(
        getZonesNumber() == 1
        && getZoneTh(1)->type == TINY
        && getBlocksNumber(getZoneTh(1)) == 2
    );
    /*
     * TINY : 0x10d3c3000
     * 0x10d3c304e - 0x10d3c3054 : 6 bytes
     * 0x10d3c306c - 0x10d3c3072 : 6 bytes
     * Total : 12
     */

    void *r = realloc(m2, 11);
    CHECK(
        getZonesNumber() == 1
        && getZoneTh(1)->type == TINY
        && getBlocksNumber(getZoneTh(1)) == 2
        && cmpContent(r, str)
    );
    /*
     * TINY : 0x10d3c3000
     * 0x10d3c306c - 0x10d3c3072 : 6 bytes
     * 0x10d3c308a - 0x10d3c3095 : 11 bytes
     * Total : 17
     */

    free(r);
    free(m3);
}

void cpy_to_tiny(void)
{
    /* copying to small block */
    const char *str = "hellohellohello";

    void *m = malloc(200); /* small */
    ft_strcpy(m, str);
    CHECK(
        getZonesNumber() == 1
        && getZoneTh(1)->type == SMALL
        && getBlocksNumber(getZoneTh(1)) == 1
        && cmpContent(m, str)
    );
    /*
     * SMALL : 0x10f9e6000
     * 0x10f9e6030 - 0x10f9e60f8 : 200 bytes
     * Total : 200
     */

    void *r = realloc(m, 5); // tiny
    CHECK(
        getZonesNumber() == 1
        && getZoneTh(1)->type == TINY
        && getBlocksNumber(getZoneTh(1)) == 1
        && cmpContent(r, str)
    );
    /*
     * TINY : 0x10fa00000
     * 0x10fa00030 - 0x10fa00035 : 5 bytes
     * Total : 5
     */

    free(r);
}

void cpy_to_large(void)
{
    /* copying to large block */
    const char *str = "hell";

    void *m = malloc(5); /* tiny */
    ft_strcpy(m, str);
    void *r = realloc(m, 2000); /* large */
    CHECK(
        getZonesNumber() == 1
        && getZoneTh(1)->type == LARGE
        && getBlocksNumber(getZoneTh(1)) == 1
        && cmpContent(r, str)
    );
    /*
     * LARGE : 0x1080c8000
     * 0x1080c8030 - 0x1080c8800 : 2000 bytes
     * Total : 2000
     */

    free(r);
}

void null(void)
{
    /* MAN: If ptr is NULL, then the call is
    equivalent to malloc(size), for all values of size */
    const char *str = "hell";
    void *r = realloc(NULL, 5);
    ft_strcpy(r, str);
    CHECK(
        getZonesNumber() == 1
        && getZoneTh(1)->type == TINY
        && getBlocksNumber(getZoneTh(1)) == 1
        && cmpContent(r, str)
    );
    /*
     * TINY : 0x100e0a000
     * 0x100e0a030 - 0x100e0a035 : 5 bytes
     * Total : 5
     */

    free(r);
}

void zero(void)
{
    /* MAN: if size is equal to zero, and ptr is not NULL,
    then the call is equivalent to free(ptr) */
    void *m = malloc(5);
    void *r = realloc(m, 0);
    CHECK(
        getZonesNumber() == 0
    );
    /*
     * Total : 0
     */

    /* MAN: If size is 0, then malloc() returns either NULL, or a unique
    pointer value that can later be successfully passed to free() */
    void *r2 = realloc(NULL, 0);
    CHECK(
        getZonesNumber() == 1
        && getZoneTh(1)->type == TINY
    );
    /*
     * TINY : 0x1085a2000
     * 0x1085a2030 - 0x1085a2030 : 0 bytes
     * Total : 0
     */

    free(r2);
    free(r);
}

void check_cpy(void)
{
    const char *str = "hello";

    void *m1 = malloc(10);
    ft_strcpy(m1, str);
    void *m2 = malloc(5);
    void *r = realloc(m1, 20);
    CHECK(
        getZonesNumber() == 1
        && getZoneTh(1)->type == TINY
        && getBlocksNumber(getZoneTh(1)) == 2
        && cmpContent(r, str)
    );
    /*
     * TINY : 0x103437000
     * 0x103437052 - 0x103437057 : 5 bytes
     * 0x10343706f - 0x103437083 : 20 bytes
     * Total : 25
     */
    free(m2);
    free(r);
}

int main()
{
    change_zone();          TEAR_DOWN();
    eat_prev();             TEAR_DOWN();
    unmap_vulnerability();  TEAR_DOWN();
    malloc_free_prev();     TEAR_DOWN();
    cpy_to_tiny();          TEAR_DOWN();
    cpy_to_large();         TEAR_DOWN();
    null();                 TEAR_DOWN();
    zero();                 TEAR_DOWN();
    check_cpy();            TEAR_DOWN();
}
