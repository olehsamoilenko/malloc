#include <stdlib.h>
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

void *memory_start = NULL;
void *last_valid_address = NULL;

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
