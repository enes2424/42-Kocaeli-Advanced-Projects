#include "../includes/malloc.h"

static int  min_mem(t_zone *tiny, t_zone *small, t_zone *large);
static void ft_putsize(size_t n);
static void ft_putaddr(size_t n);

__attribute__((visibility("default")))
void    show_alloc_mem()
{
    pthread_mutex_lock(&g_memory_lock);

    t_zone  *tiny_zone = g_mm.tiny_zones;
    t_zone  *small_zone = g_mm.small_zones;
    t_zone  *large_zone = g_mm.large_zones;

    t_block *block;

    size_t  total = 0;

    void    *payload;

    int     type;

    while (tiny_zone != NULL || small_zone != NULL || large_zone != NULL)
    {
        type = min_mem(tiny_zone, small_zone, large_zone);

        switch (type)
        {
            case TINY:
                ft_putstr_fd("TINY : ", 1);
                ft_putaddr((size_t)tiny_zone);
                ft_putchar_fd('\n', 1);

                block = tiny_zone->ptr;

                while (block != NULL)
                {
                    payload = (void *)((char *)block + sizeof(t_block));
    
                    if (!block->free)
                    {
                        ft_putaddr((size_t)payload);
                        ft_putstr_fd(" - ", 1);
                        ft_putaddr((size_t)payload + block->size);
                        ft_putstr_fd(" : ", 1);
                        ft_putsize(block->size);
                        ft_putendl_fd(" bytes", 1);

                        total += block->size;
                    }
    
                    block = block->next;
                }

                tiny_zone = tiny_zone->next;

                break;
            case SMALL:
                ft_putstr_fd("SMALL : ", 1);
                ft_putaddr((size_t)small_zone);
                ft_putchar_fd('\n', 1);

                block = small_zone->ptr;
    
                while (block != NULL)
                {
                    payload = (void *)((char *)block + sizeof(t_block));
    
                    if (!block->free)
                    {
                        ft_putaddr((size_t)payload);
                        ft_putstr_fd(" - ", 1);
                        ft_putaddr((size_t)payload + block->size);
                        ft_putstr_fd(" : ", 1);
                        ft_putsize(block->size);
                        ft_putendl_fd(" bytes", 1);

                        total += block->size;
                    }

                    block = block->next;
                }

                small_zone = small_zone->next;

                break;
            case LARGE:
                ft_putstr_fd("LARGE : ", 1);
                ft_putaddr((size_t)large_zone);
                ft_putchar_fd('\n', 1);

                payload = large_zone->ptr;

                ft_putaddr((size_t)payload);
                ft_putstr_fd(" - ", 1);
                ft_putaddr((size_t)payload + large_zone->size);
                ft_putstr_fd(" : ", 1);
                ft_putsize(large_zone->size);
                ft_putendl_fd(" bytes", 1);

                total += large_zone->size;

                large_zone = large_zone->next;

                break;
        }
    }

    ft_putstr_fd("Total : ", 1);
    ft_putsize(total);
    ft_putendl_fd(" bytes\n", 1);

    pthread_mutex_unlock(&g_memory_lock);
}

static int min_mem(t_zone *tiny, t_zone *small, t_zone *large)
{
    if (tiny != NULL && ((small != NULL && (size_t)tiny < (size_t)small) || small == NULL))
    {
        if ((large != NULL && (size_t)tiny < (size_t)large) || large == NULL)
            return TINY;
        else
            return LARGE;
    }
    else
    {
        if (small != NULL && ((large != NULL && (size_t)small < (size_t)large) || large == NULL))
            return SMALL;
        else
            return LARGE;
    }
}

static void ft_putsize(size_t n)
{
    char    buffer[21];
    int     i = 0;

    if (n == 0)
    {
        ft_putchar_fd('0', 1);
        return;
    }

    while (n > 0)
    {
        buffer[i++] = (n % 10) + '0';
        n /= 10;
    }

    while (i-- > 0)
        ft_putchar_fd(buffer[i], 1);
}

static void	ft_putaddr(size_t n)
{
    char    hex[17] = "0123456789ABCDEF";
    char    buffer[19];
    int     i = 0;

    ft_putstr_fd("0x", 1);
    if (n == 0)
    {
        ft_putchar_fd('0', 1);
        return;
    }

    while (n > 0)
    {
        buffer[i++] = hex[n % 16];
        n /= 16;
    }

    while (i-- > 0)
        ft_putchar_fd(buffer[i], 1);
}