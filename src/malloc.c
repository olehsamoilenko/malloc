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

struct metadata *memory_start = NULL; // TODO: get rid of memory start (mb static, mmap(0))
struct metadata *last_valid_address = NULL;

// TODO: realloc

// TODO: If “ptr” is a NULL pointer, no operation is performed.

// TODO: We will compare the number of "reclaims" with the number of test0 and test1. If there is so much
// of "page reclaims" or more than test1, the free does not work.
// $> ./ run.sh / usr / bin / time -l ./test2
void myfree(void *p)
{
    struct metadata *b = (struct metadata *)((char *)p - sizeof(struct metadata));
    b->available = 1;

    END(b)->available = b->available; // TODO: duplication ?
    END(b)->size = b->size;

    struct metadata *next = GETNEXT(b);
	if (next && next->available && next->type == b->type) {

		b->size += next->size + 2 * sizeof(struct metadata);

		END(b)->size = b->size;

		if (next == last_valid_address) {
			last_valid_address = b; /* next block is eaten */
		}
	}

	struct metadata *prev = GETPREV(b);
	if (prev && prev->available && prev->type == b->type) {
		
		struct metadata *big_block_end = END(b);
		big_block_end->size += prev->size + 2 * sizeof(struct metadata);

		START(prev)->size = big_block_end->size; 
		
		if (b == last_valid_address) {
			last_valid_address = START(prev); /* prev block eaten */
			// TODO: unmap several pages ?
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
			printf("Block type: TINY\n");
		#endif
	}
	else if (size <= MAX_SMALL_SIZE) {
		type = SMALL;
		#if DEBUG
			printf("Block type: SMALL\n");
		#endif
	}
	else {
		type = LARGE;
		#if DEBUG
			printf("Block type: LARGE\n");
		#endif
	}

	struct metadata *res = (struct metadata *)memory_start;
	while (1)
	{
		if (res->available && res->type == type && res->size >= size)
			return (res);

		res = GETNEXT(res);
		if (!res)
			break ;
	}

	#if DEBUG
		printf("No suitable space: size = %lu\n", size);
	#endif
    return (NULL);
}

void show_alloc_mem()
{
	struct metadata *block = (struct metadata *)memory_start;

	const char *labels[] = {"TINY", "SMALL", "LARGE"};

	int current_type = -1;
	unsigned long sum = 0;

    while (1)
    {
		if (current_type != block->type)
		{
			printf("%s : 0x%lX\n", labels[block->type], (unsigned long)block);
			current_type = block->type;
		}

		if (!block->available)
		{
			printf("0x%lX - 0x%lX : %u bytes\n",
				(unsigned long)(char *)block + sizeof(struct metadata),
				(unsigned long)END(block), block->size);
			sum += block->size;
		}

        block = GETNEXT(block);
		if (!block)
		{
			printf("Total : %lu", sum);
			break ;
		}
    }

    printf("\n");
}

// TODO Count the number of pages used and adjust the score as follows:
// - less than 255 pages, the reserved memory is insufficient: 0
// - 1023 pages and more, the malloc works but consumes a minimum page each allocation: 1
// - between 513 pages and 1022 pages, the malloc works but the overhead is too important: 2
// - between 313 pages and 512 pages, the malloc works but the overhead is very important: 3
// - between 273 pages and 312 pages, the malloc works but the overhead is important: 4
// - between 255 and 272 pages, the malloc works and the overhead is reasonable: 5

void *myalloc(unsigned long size)
{
    struct metadata *new_block = get_suitable_block(size);

    if (new_block && new_block->size >= size + 2 * sizeof(struct metadata))
    {
        void *new_block_end_addr = (char *)new_block + sizeof(struct metadata) + size;
        struct metadata *new_block_end = (struct metadata *)new_block_end_addr;

        void *reduced_addr = (char *)new_block_end_addr + sizeof(struct metadata);
        struct metadata *reduced_block = (struct metadata *)reduced_addr;
        reduced_block->available = new_block->available; // 1
        reduced_block->size = new_block->size - 2 * sizeof(struct metadata) - size;
		reduced_block->type = new_block->type;

        struct metadata *reduced_block_end = END(reduced_block);
        reduced_block_end->available = reduced_block->available;
        reduced_block_end->size = reduced_block->size;

        new_block->available = 0;
        new_block->size = size;
        new_block_end->available = new_block->available;
        new_block_end->size = new_block->size;
		new_block_end->type = new_block->type;

        if (reduced_block > last_valid_address)
            last_valid_address = reduced_block;

		#if DEBUG
			printf("new: %p %lu %u\n", new_block, (unsigned long)new_block, new_block->size);
			printf("new end: %p %lu %u\n", new_block_end, (unsigned long)new_block_end, new_block_end->size);
			printf("reduced: %p %lu %u\n", reduced_block, (unsigned long)reduced_block, reduced_block->size);
			printf("reduced end: %p %lu %u\n", reduced_block_end, (unsigned long)reduced_block_end, reduced_block_end->size);
		#endif

        return ((char *)new_block + sizeof(struct metadata));
    }
    else if (new_block && new_block->size >= size)
    {
		#if DEBUG
        	printf("maybe not filled fully, but doesn't matter\n");
		#endif
        new_block->available = 0; /* not filled fully, but doesn't matter */

        END(new_block)->available = new_block->available;
        return ((char *)new_block + sizeof(struct metadata));
    }
    else
    {
        /* there is no suitable block */
		
        return (NULL);
    }
}

void testing(void);

int main(void)
{
    testing();
	// getpagesize() - 4096
    return (0);
}

// BONUSES:
// DONE     Manage the malloc debug environment variables. You can imitate those from malloc
//          system or invent your own.
//          - Malloc has debug environment variables

//          • Create a show_alloc_mem_ex() function that displays more details, for example,
//          a history of allocations, or an hexa dump of the allocated zones.
//          - A function allows to dump hexa allocated zones

// DONE     • “Defragment” the freed memory.
//          - During free, the project "defragments" free memory by grouping free blocks
//          concomitant in one

//          • Manage the use of your malloc in a multi-threaded program (so to be “thread safe”
//          using the pthread lib).
//          - A function makes it possible to display a history of the memory allocations made
