#include "ft_ping.h"

#include <netinet/ip_icmp.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

uint16_t icmp_checksum(void *buf, size_t len)
{
	uint32_t sum = 0;
	uint16_t *data = buf;

	while (len > 1)
	{
		sum += *data++;
		len -= 2;
	}

	if (len == 1)
		sum += *(uint8_t *)data;

	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	return ~sum;
}

void init_icmp_packet(void *packet, int id)
{
	struct icmphdr *icmp = (struct icmphdr *)packet;

	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->un.echo.id = id;
}

void prepare_packet(t_ping_ctx *ctx, short int size, int seq_number)
{
	struct icmphdr *icmp = (struct icmphdr *)ctx->packet;
	void *payload = ctx->packet + sizeof(struct icmphdr);

	if (ctx->have_timespec)
	{
		struct timespec ts;
		if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0)
		{
			perror("ft_ping: getting time");
			exit(1);
		}

		memcpy(payload, &ts, sizeof(ts));
		memset(payload + sizeof(ts), ICMP_PATTERN, size - sizeof(struct icmphdr) - sizeof(struct timespec));
	}

	icmp->un.echo.sequence = htons(seq_number);
	icmp->checksum = 0;
	icmp->checksum = icmp_checksum(ctx->packet, size);
}
