#include "ft_ping.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>

static void unknown_host_error(void)
{
	printf("ft_ping: unknown host\n");
	exit(1);
}

char *init_addr(char *ip, struct sockaddr_in *addr)
{
	struct addrinfo hints;
	struct addrinfo *res;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;

	if (getaddrinfo(ip, NULL, &hints, &res) != 0)
		unknown_host_error();

	memcpy(addr, res->ai_addr, sizeof(struct sockaddr_in));

	char *resolved_ip = malloc(INET_ADDRSTRLEN);

	if (resolved_ip == NULL)
	{
		freeaddrinfo(res);
		return NULL;
	}

	if (inet_ntop(AF_INET, &(((struct sockaddr_in *)res->ai_addr)->sin_addr),
				  resolved_ip, INET_ADDRSTRLEN) == NULL)
	{
		free(resolved_ip);
		freeaddrinfo(res);
		unknown_host_error();
	}

	freeaddrinfo(res);
	return resolved_ip;
}
