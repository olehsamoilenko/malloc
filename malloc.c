#include <cstdlib>
#include <cstdio>

#define NEXT(block) (   \
    (struct metadata *)((char *)block + 2 * sizeof(struct metadata) + block->size) \
)

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

void myfree(void *p)
{
    struct metadata *b = (struct metadata *)((char *)p - sizeof(struct metadata));
    b->available = 1;

    END(b)->available = b->available;
    END(b)->size = b->size;

    if (b < last_valid_address) {
        struct metadata *next = NEXT(b);
        if (next->available) {

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

void mysetup(void *buf, std::size_t size)
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

struct metadata *get_suitable_block(std::size_t size) /* last valid address oriented */
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

            if (next_block_addr >= last_valid_address)
                return (NULL);
            else
                res = (struct metadata *)next_block_addr;
        }
    }

    return (NULL);
}

void *myalloc(std::size_t size) // TODO: return ptr to data
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

        return ((char *)new_block + sizeof(struct metadata));
    }
    else if (new_block && new_block->size >= size)
    {
        new_block->available = 0; /* not filled fully, but doesn't matter */
        void *end = (char *)new_block + sizeof(struct metadata) + new_block->size;
        struct metadata *meta_end = (struct metadata *)end;
        meta_end->available = new_block->available;
        return ((char *)new_block + sizeof(struct metadata));
    }
    else
    {
        return (NULL);
    }
}

int main(void)
{
    size_t total_mem = 100;
	void *buf = malloc(total_mem);

    mysetup(buf, total_mem);

    void *a = myalloc(10);
    void *b = myalloc(10);
    void *c = myalloc(10);
    void *d = myalloc(6);
    myalloc(10);
    myalloc(10);
    myalloc(10);
    myalloc(6);

	myfree(c);
	myfree(b);
	myfree(d);
	myfree(a);

    free(buf);
    return (0);
}
