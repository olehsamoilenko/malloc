#include "malloc.h"

struct metadata *memory_start = NULL; // TODO: get rid of memory start
struct metadata *last_valid_address = NULL;

// TODO
// Each zone must contain at least 100 allocations.
// ◦ “TINY” mallocs, from 1 to n bytes, will be stored in N bytes big zones.
// ◦ “SMALL” mallocs, from (n+1) to m bytes, will be stored in M bytes big zones.
// ◦ “LARGE” mallocs, fron (m+1) bytes and more, will be stored out of zone,
// which simply means with mmap(), they will be in a zone on their own.
// • It’s up to you to define the size of n, m, N and M so that you find a good compromise
// between speed (saving on system recall) and saving memory.
// void show_alloc_mem();
// The visual will be formatted by increasing addresses such as:
// TINY : 0xA0000
// 0xA0020 - 0xA004A : 42 bytes
// 0xA006A - 0xA00BE : 84 bytes
// SMALL : 0xAD000
// 0xAD020 - 0xADEAD : 3725 bytes
// LARGE : 0xB0000
// 0xB0020 - 0xBBEEF : 48847 bytes
// Total : 52698 bytes

// TODO: realloc

// TODO: La free() function deallocates the memory allocation pointed to by “ptr”. If “ptr”is
// a NULL pointer, no operation is performed.

// TODO: We will compare the number of "reclaims" with the number of test0 and test1. If there is so much
// of "page reclaims" or more than test1, the free does not work.
// $> ./ run.sh / usr / bin / time -l ./test2
void myfree(void *p)
{
    struct metadata *b = (struct metadata *)((char *)p - sizeof(struct metadata));
    b->available = 1;

    END(b)->available = b->available; // TODO: duplication ?
    END(b)->size = b->size;

    if (b < last_valid_address) {
        struct metadata *next = NEXT(b);

        if (next->available) { // TODO: bug?

            b->size += next->size + 2 * sizeof(struct metadata);

			END(b)->size = b->size;

            if (next == last_valid_address) {
                last_valid_address = b; /* next block is eaten */
            }
        }
    }

	if (b > memory_start) {
		struct metadata *prev = PREV(b);
		if (prev->available) {
			
			struct metadata *big_block_end = END(b);
			big_block_end->size += prev->size + 2 * sizeof(struct metadata);

			START(prev)->size = big_block_end->size; 
			
			if (b == last_valid_address) {
				last_valid_address = START(prev); /* prev block eaten */
			}
		}
	}
}

struct metadata *get_suitable_block(unsigned long size) /* last valid address oriented */
{
    if (!size)
        return (NULL);
    struct metadata *res = ((struct metadata *)last_valid_address);
    if (res->available && res->size >= size)
        return (res);
    else
    {
        res = (struct metadata *)memory_start;
        while (1)
        {
            if (res->available && res->size >= size)
                return (res);

            res = NEXT(res);
            if (!res)
                return (NULL);
        }
    }

    return (NULL);
}


// TODO: With performance in mind, you must limit the number of calls to mmap(), but also
// to munmap(). You have to “pre-allocate” some memory zones to store your “small”
// and “medium” malloc.
// • The size of these zones must be a multiple of getpagesize().
// Pre-allocated zones
// Check in the source code that pre-allocated areas according to different sizes
// malloc can store at least 100 times the maximum size for this type of zone.
// Also check that the size of the fields is a multiple of getpagesize ().
// If any of these points are missing, leave No.

// TODO: We see in this example that this malloc used 1024 pages or 4MBytes to store
// 1Mbyte.
// Count the number of pages used and adjust the score as follows:
// - less than 255 pages, the reserved memory is insufficient: 0
// - 1023 pages and more, the malloc works but consumes a minimum page each
// allocation: 1
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
        reduced_block->available = new_block->available;
        reduced_block->size = new_block->size - 2 * sizeof(struct metadata) - size;

        struct metadata *reduced_block_end = END(reduced_block); /* initialized */
        reduced_block_end->available = reduced_block->available;
        reduced_block_end->size = reduced_block->size;

        new_block->available = 0;
        new_block->size = size;
        new_block_end->available = new_block->available;
        new_block_end->size = new_block->size;

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
		#if DEBUG
			printf("no suitable block\n");
		#endif
        return (NULL);
    }
}

void testing(void);

int main(void)
{
    testing();
    return (0);
}

// BONUSES:
// PROGRESS Manage the malloc debug environment variables. You can imitate those from malloc
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
