#include "ft_ping.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void handle_remaining_hosts(char **argv, int argc, char *raw_ip, t_options *options, int id)
{
	int i = 0;
	struct sockaddr_in addr;
	char *ip;

	while (argv[i] != raw_ip)
		i++;
	i++;

	while (i < argc)
	{
		if (argv[i])
		{
			raw_ip = argv[i];

			ip = init_addr(raw_ip, &addr);

			if (ip == NULL)
			{
				fprintf(stderr, "ft_ping: memory allocation failed\n");
				exit(1);
			}

			print_ping_header(raw_ip, ip, options, id);

			printf("--- %s ping statistics ---\n", raw_ip);
			printf("1 packets transmitted, 0 packets received, 100%% packet loss\n");

			free(ip);
		}

		i++;
	}
}
