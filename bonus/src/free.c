#include "../includes/malloc.h"
#include <sys/mman.h>
#include <unistd.h>

static void *free_for_tiny_or_small(void *ptr, int type, bool scribble);
static void *free_for_large(void *ptr, bool scribble);

__attribute__((visibility("default")))
void    free(void *ptr)
{
    if (ptr == NULL)
        return;

    pthread_mutex_lock(&g_memory_lock);
    
    bool    scribble = false;

    char    *env = getenv("MALLOC_SCRIBBLE");

    if (env && ft_strncmp(env, "1", 2) == 0)
        scribble = true;

    if (g_mm.tiny_zones != NULL)
        ptr = free_for_tiny_or_small(ptr, TINY, scribble);

    if (ptr != NULL && g_mm.small_zones != NULL)
        ptr = free_for_tiny_or_small(ptr, SMALL, scribble);

    if (ptr != NULL && g_mm.large_zones != NULL)
        ptr = free_for_large(ptr, scribble);

    if (ptr != NULL)
        ft_putendl_fd("Warning: attempt to free non-malloc'd pointer", 2);

    pthread_mutex_unlock(&g_memory_lock);
}

static void *free_for_tiny_or_small(void *ptr, int type, bool scribble)
{
    t_zone  *zone = type == TINY ? g_mm.tiny_zones : g_mm.small_zones;
    t_block *block;
    void    *payload;

    while (zone != NULL)
    {
        block = zone->ptr;

        payload = (void *)((char *)block + sizeof(t_block));

        if (ptr == payload)
        {
            if (!block->free)
            {
                zone->size += sizeof(t_block) + block->size;

                g_mm.total_used_size -= sizeof(t_block) + block->size;

                if (block->next != NULL && block->next->free)
                    block->next = block->next->next;

                if (scribble)
                    scribble_block(block);

                block->size = (size_t)block->next - (size_t)payload;
                block->free = true;
            }
            else
                ft_putendl_fd("Warning: double free detected", 2);

            return NULL;
        }
        else
        {
            while (block->next != NULL)
            {
                payload = (void *)((char *)block->next + sizeof(t_block));

                if (ptr == payload)
                {
                    if (!block->next->free)
                    {
                        if (scribble)
                            scribble_block(block->next);

                        zone->size += sizeof(t_block) + block->next->size;

                        g_mm.total_used_size -= sizeof(t_block) + block->next->size;

                        if (block->free)
                        {
                            if (block->next->next != NULL)
                            {
                                if (block->next->next->free)
                                    block->next = block->next->next->next;

                                block->size = (size_t)block->next - (size_t)block - sizeof(t_block);
                            }
                            else
                            {
                                t_block *tmp = zone->ptr;

                                while (tmp->next != block->next)
                                    tmp = tmp->next;
                                    
                                tmp->next = NULL;
                            }
                        }
                        else 
                        {
                            if (block->next->next != NULL)
                            {
                                size_t  position = (size_t)block + sizeof(t_block) + block->size;
                                size_t  alignment = 2 * sizeof(size_t);

                                if ((position + sizeof(t_block)) % alignment != 0)
                                    position += alignment - ((position + sizeof(t_block)) % alignment);

                                ft_memmove((void *)position, block->next, sizeof(t_block));

                                block->next = (t_block *)position;

                                if (block->next->next->free)
                                    block->next->next = block->next->next->next;
                                
                                block->next->size = ((size_t)block->next->next) - ((size_t)block->next) - sizeof(t_block);
                                block->next->free = true;
                            }
                            else
                                block->next = NULL;
                        }
                    }
                    else
                        ft_putendl_fd("Warning: double free detected", 2);

                    return NULL;
                }

                block = block->next;
            }
        }

        zone = zone->next;
    }

    return ptr;
}

static void *free_for_large(void *ptr, bool scribble)
{
    t_zone  *zone = g_mm.large_zones;
    t_zone  *prev = NULL;

    while (zone != NULL)
    {
        if (ptr == zone->ptr)
        {
            t_zone  *tmp = zone->next;
            size_t  alloc_size = (((sizeof(t_zone) + sizeof(size_t) + zone->size) + get_page_size() - 1) / get_page_size()) * get_page_size();
            
            if (scribble)
                ft_memset(ptr, 0x55, zone->size);

            if (munmap(zone, alloc_size) == -1)
                ft_putendl_fd("Error: free failed", 2);
            else
            {
                g_mm.total_used_size -= alloc_size;

                if (prev == NULL)
                    g_mm.large_zones = tmp;
                else
                    prev->next = tmp;
            }

            return NULL;
        }

        prev = zone;
        zone = zone->next;
    }

    return ptr;
}