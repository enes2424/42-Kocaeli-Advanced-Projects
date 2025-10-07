#ifndef ALLOC_H
#define ALLOC_H

#define TINY 0
#define SMALL 1
#define LARGE 2

#include "libft/libft.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct s_block
{
    size_t          size;
    bool            free;
    struct s_block  *next;
}   t_block;

typedef struct s_zone
{
    size_t          size; /* unused size for tiny and small, one payload size like in t_block size for large */
    struct s_zone   *next;
    void            *ptr; /* first block pointer for tiny and small, payload pointer for large */
}   t_zone;

typedef struct s_memory_management
{
    t_zone  *tiny_zones;
    t_zone  *small_zones;
    t_zone  *large_zones;
    size_t  total_used_size;
}   t_memory_management;

extern t_memory_management g_mm;
extern pthread_mutex_t  g_memory_lock;

size_t  get_page_size(void);
size_t  get_zone_size_tiny(void);
size_t  get_zone_size_small(void);
size_t  get_max_size_tiny_malloc(void);
size_t  get_max_size_small_malloc(void);

void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);
void    show_alloc_mem();

#endif