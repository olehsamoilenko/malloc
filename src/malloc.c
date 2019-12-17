/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osamoile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 09:37:16 by osamoile          #+#    #+#             */
/*   Updated: 2019/11/16 09:37:23 by osamoile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

struct metadata *last_valid_address = NULL;

struct metadata *get_memory_start() {
	static void *mem_start = NULL;

	if (mem_start == NULL) {
		mem_start = mmap(NULL, 1, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0); // TODO: 1 => TINY_SIZE
		struct metadata *tiny = (struct metadata *)mem_start;
		tiny->available = 1;
		tiny->type = TINY;
		tiny->size = TINY_ZONE - 2 * sizeof(struct metadata);
		tiny->next = NULL;
		tiny->prev = NULL;
		last_valid_address = mem_start;
	}
	return (mem_start);
}
// TODO: realloc

// TODO: If “ptr” is a NULL pointer, no operation is performed.

// TODO: We will compare the number of "reclaims" with the number of test0 and test1. If there is so much
// of "page reclaims" or more than test1, the free does not work.
// $> ./ run.sh / usr / bin / time -l ./test2
void myfree(void *p)
{
    struct metadata *block = (struct metadata *)((char *)p - sizeof(struct metadata));
    block->available = true;

    struct metadata *next = block->next;
	if (next && next->available && next->type == block->type) {

		block->size += next->size + sizeof(struct metadata);
		block->next = next->next;

		if (next == last_valid_address) {
			last_valid_address = block; /* next block is eaten */
		}
	}

	struct metadata *prev = block->prev;
	if (prev && prev->available) {
		
		if (prev->type == block->type) {

			prev->size += block->size + sizeof(struct metadata);
			prev->next = block->next;
			
			if (block == last_valid_address) {
				last_valid_address = prev; /* prev block eaten */
			}
		}
		else {
			if (block == last_valid_address) {
				printf("Yes, it's last, need to unmap\n");
				int res = munmap(block, block->size); // TODO: if unmap several pages, don't unmap first page!
				printf("Unmap result: %d\n", res);
				last_valid_address = prev; // TODO: duplication
			}
		}
	}
}

struct metadata *get_suitable_block(unsigned long size) /* last valid address oriented */
{
    if (!size)
        return (NULL);

	int type;
	if (size <= MAX_TINY_SIZE) {
		type = TINY;
		#if DEBUG
			// printf("Block type: TINY\n");
		#endif
	}
	else if (size <= MAX_SMALL_SIZE) {
		type = SMALL;
		#if DEBUG
			// printf("Block type: SMALL\n");
		#endif
	}
	else {
		type = LARGE;
		#if DEBUG
			// printf("Block type: LARGE\n");
		#endif
	}

    // struct page_meta *page = get_first_page();
    // while (page)
    // {
        
    //     page = page->next;
    // }

	struct metadata *res = (struct metadata *)get_memory_start();
	while (1)
	{
		if (res->available && res->type == type && res->size >= size)
			return (res);

		res = res->next;
		if (!res)
			break ;
	}

	#if DEBUG
		printf("No suitable space: size = %lu\n", size);
	#endif
    return (NULL);
}

// void *mmap_zone(unsigned long size)
// {
// 	unsigned long to_request;
// 	char type;

// 	if (size <= MAX_TINY_SIZE) {
// 		to_request = TINY_ZONE;
// 		type = TINY;
// 	}
// 	else if (size <= MAX_SMALL_SIZE) {
// 		to_request = SMALL_ZONE;
// 		type = SMALL;
// 	}
// 	else {
// 		to_request = LARGE_ZONE(size); // TODO: check overflowing, TODO: page + 1 ?
// 		printf("To request: %d\n", to_request);
// 		type = LARGE;
// 	}

// 	struct metadata *zone = mmap(NULL, to_request, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);// TODO: POLNOE GAVNO: new zone not in the end
// 	printf("\tMapping new zone, size = %lu, zone start = %p\n", to_request, zone);
// 	zone->available = true;
// 	zone->type = type;
//     zone->size = to_request - sizeof(struct metadata);
// 	// TODO: free after mmap to avoid fragmentation

// 	last_valid_address = zone;

// 	return (zone);
// }

// TODO Count the number of pages used and adjust the score as follows:
// - less than 255 pages, the reserved memory is insufficient: 0
// - 1023 pages and more, the malloc works but consumes a minimum page each allocation: 1
// - between 513 pages and 1022 pages, the malloc works but the overhead is too important: 2
// - between 313 pages and 512 pages, the malloc works but the overhead is very important: 3
// - between 273 pages and 312 pages, the malloc works but the overhead is important: 4
// - between 255 and 272 pages, the malloc works and the overhead is reasonable: 5
void *myalloc(unsigned long size)
{
	// TODO: get rid of last valid addr
	// TODO: rename new and reduced blocks

    struct metadata *new_block = get_suitable_block(size);
	if (!new_block)
	{
		mmap_zone(size);
		new_block = get_suitable_block(size);
	}

    if (new_block && new_block->size >= size + sizeof(struct metadata))
    {
        struct metadata *reduced_block = (struct metadata *)((char *)new_block + sizeof(struct metadata) + size);
        reduced_block->available = true;
        reduced_block->size = new_block->size - sizeof(struct metadata) - size;
		reduced_block->type = new_block->type;

		reduced_block->next = new_block->next;
		reduced_block->prev = new_block;
		new_block->next = reduced_block;
	
        new_block->available = false;
        new_block->size = size;

        if (reduced_block > last_valid_address)
            last_valid_address = reduced_block;

		#if DEBUG
			printf("new: %p %lu %u\n", new_block, (unsigned long)new_block, new_block->size);
			printf("reduced: %p %lu %u\n", reduced_block, (unsigned long)reduced_block, reduced_block->size);
		#endif

		printf("me: %p   next: %p   prev: %p\n",
			new_block, new_block->next, new_block->prev);
		if (new_block->next) {
			struct metadata *nxt = new_block->next;
			printf("my next: %p   next: %p   prev: %p\n",
				nxt, nxt->next, nxt->prev);
		}
		if (new_block->prev) {
			struct metadata *prv = new_block->prev;
			printf("my prev: %p   next: %p   prev: %p\n",
				prv, prv->next, prv->prev);
		}

        return ((char *)new_block + sizeof(struct metadata));
    }
    else if (new_block && new_block->size >= size)
    {
		#if DEBUG
        	printf("maybe not filled fully, but doesn't matter\n");
		#endif
        new_block->available = false; /* not filled fully, but doesn't matter */

        return ((char *)new_block + sizeof(struct metadata));
    }
    else
    {
        return (NULL); /* there is no suitable block */
    }
}

void testing(void);

int main(void)
{
    testing();
    return (0);
}

// BONUSES:
// DONE     Manage the malloc debug environment variables. You can imitate those from malloc
//          system or invent your own.
//          - Malloc has debug environment variables

// DONE     • Create a show_alloc_mem_ex() function that displays more details, for example,
//          a history of allocations, or an hexa dump of the allocated zones.
//          - A function allows to dump hexa allocated zones

// DONE     • “Defragment” the freed memory.
//          - During free, the project "defragments" free memory by grouping free blocks
//          concomitant in one

//          • Manage the use of your malloc in a multi-threaded program (so to be “thread safe”
//          using the pthread lib).
//          - A function makes it possible to display a history of the memory allocations made

// void *zone = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
// void *zone2 = mmap(zone, getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
// void *zone3 = mmap(zone2 + 1, getpagesize(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
// munmap(zone2, getpagesize());
// // myfree(a);
// printf("%p %p %p\n", zone, zone2, zone3);
// zone2 unmaped, map it before use
