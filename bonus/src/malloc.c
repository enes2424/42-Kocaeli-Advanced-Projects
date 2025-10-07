#include "../includes/malloc.h"
#include <sys/resource.h>
#include <sys/mman.h>
#include <unistd.h>

static int      control_size(size_t size, int type);
static int      get_type_malloc(size_t size);
static t_zone   *new_zone(size_t size);
static void     *init_block_payload(t_zone *zone, t_block *block, size_t size);
static void     *first_init_block_payload(t_zone *zone, size_t size);
static void     *malloc_for_tiny_or_small(size_t size, int type);
static void     *malloc_for_large(size_t size);

__attribute__((visibility("default")))
void    *malloc(size_t size)
{
    int     type = get_type_malloc(size);
    void    *ptr = NULL;

    if (!control_size(size, type))
        return NULL;

    pthread_mutex_lock(&g_memory_lock);

    switch (type)
    {
        case TINY:
            ptr = malloc_for_tiny_or_small(size, TINY);

            break;
    
        case SMALL:
            ptr = malloc_for_tiny_or_small(size, SMALL);

            break;
        
        case LARGE:
            ptr = malloc_for_large(size);

            break;
    }

    if (ptr == NULL)
        ft_putendl_fd("Error: malloc failed", 2);

    pthread_mutex_unlock(&g_memory_lock);

    return ptr;
}

static int control_size(size_t size, int type)
{
    if (size == 0)
    {
        ft_putendl_fd("Error: malloc called with size 0", 2);

        return 0;
    }

    struct rlimit rl;

    if (getrlimit(RLIMIT_AS, &rl) != 0)
    {
        ft_putendl_fd("Error: malloc getrlimit failed", 2);

        return 0;
    }

    if (type == LARGE)
        size = (((sizeof(t_zone) + sizeof(size_t) + size) + get_page_size() - 1) / get_page_size()) * get_page_size();
    else
        size += sizeof(t_block);

    if (rl.rlim_cur != RLIM_INFINITY && size > rl.rlim_cur - g_mm.total_used_size)
    {
        ft_putendl_fd("Error: malloc request exceeds process memory limit", 2);

        return 0;
    }

    return 1;
}

static int get_type_malloc(size_t size)
{
    if (size <= get_max_size_tiny_malloc())
        return TINY;
    
    if (size <= get_max_size_small_malloc())
        return SMALL;

    return LARGE;
}

static t_zone   *new_zone(size_t size)
{
    t_zone  *zone = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

    if (zone == MAP_FAILED)
        return NULL;

    zone->size = size - sizeof(t_zone);
    zone->next = NULL;

    g_mm.total_used_size += sizeof(t_zone);

    return zone;
}

static void *init_block_payload(t_zone *zone, t_block *block, size_t size)
{
    block->size = size;
    block->free = false;
    block->next = NULL;

    zone->size -= sizeof(t_block) + size;

    g_mm.total_used_size += sizeof(t_block) + size;

    return (void *)((char *)block + sizeof(t_block));
}

static void *first_init_block_payload(t_zone *zone, size_t size)
{
    zone->ptr = (void *)((char *)zone + sizeof(t_zone));

    return init_block_payload(zone, zone->ptr, size);
}

static void *malloc_for_tiny_or_small(size_t size, int type)
{
    size_t  N = get_zone_size_tiny();
    size_t  M = get_zone_size_small();
    size_t  n = get_max_size_tiny_malloc();

    t_zone  *zone = type == TINY ? g_mm.tiny_zones : g_mm.small_zones;
    
    t_block *block;

    size_t  position;
    size_t  alignment = 2 * sizeof(size_t);

    bool    flag;
    bool    scribble = false;

    char    *env = getenv("MALLOC_SCRIBBLE");

    if (env && ft_strncmp(env, "1", 2) == 0)
        scribble = true;

    if (zone == NULL)
    {
        zone = new_zone(type == TINY ? N : M);

        if (zone == NULL)
            return NULL;

        if (type == TINY)
            g_mm.tiny_zones = zone;
        else
            g_mm.small_zones = zone;

        return first_init_block_payload(zone, size);
    }
    
    while (true)
    {
        if (sizeof(t_block) + size <= zone->size)
        {
            block = zone->ptr;

            flag = false;

            while (!(block->free && block->size >= size))
            {
                if (block->next == NULL)
                {
                    position = (size_t)block + sizeof(t_block) + block->size;

                    if ((position + sizeof(t_block)) % alignment != 0)
                        position += alignment - ((position + sizeof(t_block)) % alignment);

                    if (position + sizeof(t_block) + size - (size_t)zone > (type == TINY ? N : M))
                    {
                        flag = true;

                        break;
                    }

                    block->next = (t_block *)position;

                    return init_block_payload(zone, block->next, size);
                }

                block = block->next;
            }

            if (!flag)
            {
                position = (size_t)block + sizeof(t_block) + size;

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

                block->size = size;
                block->free = false;

                zone->size -= sizeof(t_block) + size;

                g_mm.total_used_size += sizeof(t_block) + size;

                return (void *)((char *)block + sizeof(t_block));
            }
        }

        if (zone->next == NULL)
        {
            t_zone *tmp = new_zone(type == TINY ? N : M);

            if (tmp == NULL)
                return NULL;

            if (type == TINY && (size_t)tmp < (size_t)g_mm.tiny_zones)
            {
                tmp->next = g_mm.tiny_zones;

                g_mm.tiny_zones = zone = tmp;
            }
            else if (type == SMALL && (size_t)tmp < (size_t)g_mm.small_zones)
            {
                tmp->next = g_mm.small_zones;

                g_mm.small_zones = zone = tmp;
            }
            else
            {
                for (zone = type == TINY ? g_mm.tiny_zones : g_mm.small_zones; zone->next != NULL && (size_t)tmp > (size_t)zone->next; zone = zone->next);

                tmp->next = zone->next;

                zone->next = tmp;
                zone = tmp;
            }

            return first_init_block_payload(zone, size);
        }

        zone = zone->next;
    }
}

static void *malloc_for_large(size_t size)
{
    t_zone  *zone;

    size_t  alloc_size = (((sizeof(t_zone) + sizeof(size_t) + size) + get_page_size() - 1) / get_page_size()) * get_page_size();

    if (g_mm.large_zones == NULL)
    {
        g_mm.large_zones = zone = new_zone(alloc_size);

        if (zone == NULL)
            return NULL;
    }
    else
    {
        t_zone *tmp = new_zone(alloc_size);

        if (tmp == NULL)
            return NULL;

        if ((size_t)tmp < (size_t)g_mm.large_zones)
        {
            tmp->next = g_mm.large_zones;

            g_mm.large_zones = zone = tmp;
        }
        else
        {
            for (zone = g_mm.large_zones; zone->next != NULL && (size_t)tmp > (size_t)zone->next; zone = zone->next);

            tmp->next = zone->next;

            zone->next = tmp;
            zone = tmp;
        }
    }

    zone->size = size;

    g_mm.total_used_size += alloc_size;

    return zone->ptr = (void *)((char *)zone + sizeof(t_zone) + sizeof(size_t));
}