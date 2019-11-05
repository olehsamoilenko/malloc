#include <stdlib.h> // TODO: replace with libft
#include <stdio.h>

#define NEXT(block) (   \
    (struct metadata *)((char *)block + 2 * sizeof(struct metadata) + block->size) \
) // TODO: check last_valid_addr here

#define PREV(block) (   \
    (struct metadata *)((char *)block - sizeof(struct metadata)) \
)

#define START(block) (   \
	(struct metadata *)((char *)block - block->size - sizeof(struct metadata)) \
)

#define END(block) (   \
	(struct metadata *)((char *)block + sizeof(struct metadata) + block->size) \
)

struct metadata {
    unsigned int available;
    unsigned int size;
};

void *memory_start = NULL; // TODO: get rid of memory start
void *last_valid_address = NULL;

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

void dump_visual(void)
{
    int i = 0;
    struct metadata *block = (struct metadata *)memory_start;

    while (1)
    {
        int meta;
        
        meta = sizeof(struct metadata);
        while (meta--)
        {
            printf("       {{{ ");
            if (++i % 10 == 0) {
                printf("\n");
            }
        }
        int data = block->size;
        while (data--)
        {
            if (block->available)
                printf("         . ");
            else
                printf("         d ");

            if (++i % 10 == 0) {
                printf("\n");
            }
        }
        meta = sizeof(struct metadata);
        while (meta--)
        {
            printf("       }}} ");
            if (++i % 10 == 0) {
                printf("\n");
            }
        }

        if (block == last_valid_address)
            break ;
        else
        {
            void *new_block = NEXT(block);
            block = (struct metadata *)new_block;
        }
    }
    printf("\n");
}

void dump(void)
{
    struct metadata *block = (struct metadata *)memory_start;

    while (1)
    {
        void *end = (char *)block + sizeof(struct metadata) + block->size;
        struct metadata *end_meta = (struct metadata *)end;

        printf("%p=%lu (%u, %u)=(%u, %u)\n", block, (unsigned long)block, block->available, block->size, end_meta->available, end_meta->size);

        void *new_block = NEXT(block);

        if (block == last_valid_address)
            break ;
        else
            block = (struct metadata *)new_block;
    }

    printf("\n");
}

void mem_dump(void *buf, int len)
{
	char *mem = (char *)buf;
	int i = 0;
	while (i < len) {
		printf("%#10x ", mem[i]);
		i++;
		if (i % 10 == 0)
			printf("\n");
	}
}

void mem_clear(void *buf, int len)
{
	char *mem = (char *)buf;
	int i = 0;
	while (i++ < len) {
		mem[i] = 0;
	}
}

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

    if ((void *)b < last_valid_address) {
        struct metadata *next = NEXT(b);
        if (next->available) {

            b->size += next->size + 2 * sizeof(struct metadata);

			END(b)->size = b->size;

            if (next == last_valid_address) {
                last_valid_address = b; /* next block is eaten */
            }
        }
    }

	if ((void *)b > memory_start) {
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

void mysetup(void *buf, unsigned long size)
{
    memory_start = buf;
    last_valid_address = buf;

    struct metadata *b0 = (struct metadata *)buf;
    b0->available = 1;
    b0->size = size - 2 * sizeof(struct metadata);

    void *end = (char *)b0 + sizeof(struct metadata) + b0->size;
    struct metadata *meta_end = (struct metadata *)end;
    meta_end->available = 1;
    meta_end->size = b0->size;
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

            struct metadata *next_block_addr = NEXT(res);

            if ((void *)next_block_addr >= last_valid_address)
                return (NULL);
            else
                res = (struct metadata *)next_block_addr;
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

void *myalloc(unsigned long size) // TODO: return ptr to data
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
        void *reduced_block_end_addr = (char *)reduced_block + sizeof(struct metadata) + reduced_block->size;
        struct metadata *reduced_block_end = (struct metadata *)reduced_block_end_addr; /* initialized */
        reduced_block_end->available = reduced_block->available;
        reduced_block_end->size = reduced_block->size;

        new_block->available = 0;
        new_block->size = size;
        new_block_end->available = new_block->available;
        new_block_end->size = new_block->size;

        if (reduced_addr > last_valid_address)
            last_valid_address = reduced_addr;

        // printf("new: %p %lu %u\n", new_block, new_block, new_block->size);
        // printf("new end: %p %lu %u\n", new_block_end, new_block_end, new_block_end->size);
        // printf("reduced: %p %lu %u\n", reduced_block, reduced_block, reduced_block->size);
        // printf("reduced end: %p %lu %u\n", reduced_block_end, reduced_block_end, reduced_block_end->size);

        return ((char *)new_block + sizeof(struct metadata));
    }
    else if (new_block && new_block->size >= size)
    {
        printf("maybe not filled fully, but doesn't matter\n");
        new_block->available = 0; /* not filled fully, but doesn't matter */
        void *end = (char *)new_block + sizeof(struct metadata) + new_block->size;
        struct metadata *meta_end = (struct metadata *)end;
        meta_end->available = new_block->available;
        return ((char *)new_block + sizeof(struct metadata));
    }
    else
    {
        printf("no space\n");
        return (NULL);
    }
}

int main(void)
{
    size_t total_mem = 100;
	void *buf = malloc(total_mem);

    mem_clear(buf, total_mem);
    mysetup(buf, total_mem);

    myalloc(1000);
    void *a = myalloc(10);
    void *b = myalloc(10);
    void *c = myalloc(10);
    void *d = myalloc(6);
    myalloc(10);
    myalloc(10);
    myalloc(10);
    myalloc(6);
    myalloc(1000);

	myfree(c);
	myfree(b);
	myfree(d);
	myfree(a);

    dump();
    dump_visual();
	mem_dump(buf, total_mem);

    free(buf);
    return (0);
}

// BONUSES:
//          Manage the malloc debug environment variables. You can imitate those from malloc
//          system or invent your own.
//          • Create a show_alloc_mem_ex() function that displays more details, for example,
//          a history of allocations, or an hexa dump of the allocated zones.
// DONE     • “Defragment” the freed memory.
//          • Manage the use of your malloc in a multi-threaded program (so to be “thread safe”
//          using the pthread lib).
//          - During free, the project "defragments" free memory by grouping free blocks
//          concomitant in one
//          - Malloc has debug environment variables
//          - A function allows to dump hexa allocated zones
//          - A function makes it possible to display a history of the memory allocations made
