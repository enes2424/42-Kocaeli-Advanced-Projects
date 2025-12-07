#include "ft_ping.h"

#include <netinet/ip_icmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{
    char *raw_ip = NULL;
    t_options options = {0};

    options.size = 64;
    options.interval = 1;

    control_args(argc - 1, argv + 1, &raw_ip, &options);

    struct sockaddr_in addr;
    char *ip = init_addr(raw_ip, &addr);

    if (ip == NULL)
    {
        fprintf(stderr, "ft_ping: memory allocation failed\n");
        exit(1);
    }

    t_ping_ctx ctx = {-1, NULL, NULL, ip, 0, 0, (size_t)options.size >= sizeof(struct icmphdr) + sizeof(struct timespec)};

    int id = ping_loop(&addr, raw_ip, ip, &options, &ctx);

    handle_remaining_hosts(argv + 1, argc - 1, raw_ip, &options, id);

    return 0;
}
