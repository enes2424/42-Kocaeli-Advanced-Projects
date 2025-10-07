#include "../includes/malloc.h"
#include <unistd.h>

static void *free_and_malloc(void *ptr, size_t oldsize, size_t size);
static void *realloc_for_tiny_or_small(void *ptr, size_t size, int type);
static void *realloc_for_large(void *ptr, size_t size);

__attribute__((visibility("default")))
void    *realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return malloc(size);

    if (size == 0)
    {
        free(ptr);
        return NULL;
    }

    void    *res = NULL;

    pthread_mutex_lock(&g_memory_lock);

    if (g_mm.tiny_zones != NULL)
        res = realloc_for_tiny_or_small(ptr, size, TINY);
    if (res == NULL && g_mm.small_zones != NULL)
        res = realloc_for_tiny_or_small(ptr, size, SMALL);
    if (res == NULL && g_mm.large_zones != NULL)
        res = realloc_for_large(ptr, size);

    if (res == NULL)
        ft_putendl_fd("Warning: attempt to realloc non-malloc'd pointer", 2);

    pthread_mutex_unlock(&g_memory_lock);

    return res;
}

static void *free_and_malloc(void *ptr, size_t oldsize, size_t size)
{
    pthread_mutex_unlock(&g_memory_lock);

    void    *tmp = malloc(size);

    if (tmp == NULL)
        return NULL;

    if (oldsize > size)
        oldsize = size;

    ft_memmove(tmp, ptr, oldsize);

    free(ptr);

    pthread_mutex_lock(&g_memory_lock);

    return tmp;
}

static void *realloc_for_tiny_or_small(void *ptr, size_t size, int type)
{
    size_t  N = get_zone_size_tiny();
    size_t  M = get_zone_size_small();
    size_t  n = get_max_size_tiny_malloc();
    size_t  m = get_max_size_small_malloc();

    t_zone  *zone = type == TINY ? g_mm.tiny_zones : g_mm.small_zones;

    t_block *block;

    size_t  position;
    size_t  alignment = 2 * sizeof(size_t);

    void    *payload;

    bool    scribble = false;

    char    *env = getenv("MALLOC_SCRIBBLE");

    if (env && ft_strncmp(env, "1", 2) == 0)
        scribble = true;

    while (zone != NULL)
    {
        block = zone->ptr;

        while (block != NULL)
        {
            payload = (void *)((char *)block + sizeof(t_block));

            if (ptr == payload)
            {
                if (block->free)
                {
                    ft_putendl_fd("Warning: realloc on free'd pointer detected", 2);

                    return ptr;
                }

                if ((type == TINY && size > n) || (type == SMALL && (size <= n || size > m)))
                    return free_and_malloc(ptr, block->size, size);

                if (size < block->size)
                {
                    position = (size_t)payload + size;

                    if ((position + sizeof(t_block)) % alignment != 0)
                        position += alignment - ((position + sizeof(t_block)) % alignment);

                    if (block->next != NULL && ((size_t)block->next - position) > (sizeof(t_block) + (type == SMALL ? n : 0)))
                    {
                        t_block *new_block = (t_block *)position;

                        new_block->size = (size_t)block->next - position - sizeof(t_block);
                        new_block->free = true;
                        new_block->next = block->next;

                        if (scribble)
                            scribble_block(new_block);

                        block->next = new_block;
                    }
  
                    zone->size += block->size - size;
                    
                    g_mm.total_used_size -= block->size - size;

                    block->size = size;
                }
                else if (size == block->size)
                    return ptr;
                else
                {
                    if (block->next == NULL)
                    {
                        if ((size_t)payload + size - (size_t)zone > (type == TINY ? N : M))
                            return free_and_malloc(ptr, block->size, size);
                        
                        block->size = size;
                    } 
                    else
                    {
                        if (block->next->free)
                        {
                            if (size > ((size_t)(block->next->next) - (size_t)payload))
                                return free_and_malloc(ptr, block->size, size);
                            else
                            {
                                position = (size_t)payload + size;

                                if ((position + sizeof(t_block)) % alignment != 0)
                                    position += alignment - ((position + sizeof(t_block)) % alignment);

                                if (((size_t)block->next->next - position) > (sizeof(t_block) + (type == SMALL ? n : 0)))
                                {
                                    t_block *new_block = (t_block *)position;

                                    new_block->size = (size_t)block->next->next - position - sizeof(t_block);
                                    new_block->free = true;
                                    new_block->next = block->next->next;

                                    if (scribble)
                                        scribble_block(new_block);

                                    block->next = new_block;
                                }
                            }
                        }
                        else if (size > ((size_t)block->next - (size_t)payload))
                            return free_and_malloc(ptr, block->size, size);
                        
                        zone->size -= size - block->size;
                                
                        g_mm.total_used_size += size - block->size;

                        block->size = size;
                    }
                }

                return ptr;
            }

            block = block->next;
        }

        zone = zone->next;
    }

    return NULL;
}

static void *realloc_for_large(void *ptr, size_t size)
{
    for (t_zone *zone = g_mm.large_zones; zone != NULL; zone = zone->next)
    {
        if (ptr == zone->ptr)
        {
            if (size == zone->size)
                return ptr;

            size_t  alloc_size = (((sizeof(t_zone) + sizeof(size_t) + zone->size) + get_page_size() - 1) / get_page_size()) * get_page_size() - sizeof(t_zone);

            if (size > (alloc_size - get_page_size()) && size <= alloc_size)
            {
                zone->size = size;
                return ptr;
            }

            return free_and_malloc(ptr, zone->size, size);
        }
    }

    return NULL;
}