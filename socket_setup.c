#include "ft_ping.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>

int create_socket(void)
{
	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (sockfd < 0)
	{
		perror("ft_ping: creating socket");
		return -1;
	}

	return sockfd;
}

int setup_socket_options(int sockfd)
{
	int ttl_opt = 1;

	if (setsockopt(sockfd, IPPROTO_IP, IP_RECVTTL, &ttl_opt, sizeof(ttl_opt)) < 0)
	{
		perror("ft_ping: setting socket options");
		return -1;
	}

	return 0;
}
