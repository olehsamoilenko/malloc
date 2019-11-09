#ifndef MALLOC_H
# include <stdio.h> 

# define MALLOC_H

# define NEXT(block) (   \
    (struct metadata *)((char *)block + 2 * sizeof(struct metadata) + block->size) \
) // TODO: check last_valid_addr here

# define PREV(block) (   \
    (struct metadata *)((char *)block - sizeof(struct metadata)) \
)

# define START(block) (   \
	(struct metadata *)((char *)block - block->size - sizeof(struct metadata)) \
)

# define END(block) (   \
	(struct metadata *)((char *)block + sizeof(struct metadata) + block->size) \
)

struct metadata {
    unsigned int available;
    unsigned int size;
};

void myfree(void *p);
void *myalloc(unsigned long size);

#endif