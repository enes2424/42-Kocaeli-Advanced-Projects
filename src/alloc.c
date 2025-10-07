#include "../includes/malloc.h"
#include <unistd.h>

t_memory_management g_mm = {0};
pthread_mutex_t g_memory_lock = PTHREAD_MUTEX_INITIALIZER;

size_t  get_page_size(void)
{
    static size_t pagesize = 0;

    if (pagesize == 0)
    {
        long    ps = sysconf(_SC_PAGESIZE);

        if (ps == -1)
            ps = 4096;

        pagesize = ps;
    }

    return pagesize;
}


size_t  get_zone_size_tiny(void) /* N */
{
    return 16 * get_page_size();
}

size_t  get_zone_size_small(void) /* M */
{
    return 128 * get_page_size();
}

size_t  get_max_size_tiny_malloc(void) /* n */
{
    size_t N = get_zone_size_tiny();

    if (N < (100 * (sizeof(t_block) + 1) + sizeof(t_zone)))
        return 0;

    size_t n = (N - sizeof(t_zone)) / 100 - sizeof(t_block);

    if (n > 256)
        n = 256;

    return n;
}

size_t  get_max_size_small_malloc(void) /* m */
{
    size_t M = get_zone_size_small();
    size_t n = get_max_size_tiny_malloc();

    if (M < (100 * (sizeof(t_block) + n + 1) + sizeof(t_zone)))
        return 0;

    size_t m = (M - sizeof(t_zone)) / 100 - sizeof(t_block);

    if (m > get_page_size())
        m = get_page_size();

    return m;
}
