#include "ft_ping.h"

#include <netinet/ip_icmp.h>
#include <sys/select.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

static void print_icmp_error(struct iphdr *orig_iphdr, struct icmphdr *recv_icmp, struct icmphdr *orig_icmp, ssize_t bytes, int ip_hdr_len, t_options *options, char *src_ip, char *dst_ip)
{
	char *ip = inet_ntoa(*(struct in_addr *)&orig_iphdr->saddr);

	switch (recv_icmp->type)
	{
	case ICMP_DEST_UNREACH:
	{
		const char *msg[] = {
			"Network Unreachable",
			"Host Unreachable",
			"Protocol Unreachable",
			"Port Unreachable",
			"Fragmentation Needed",
			"Source Route Failed",
			"Network Unknown"};
		int code = recv_icmp->code < 7 ? recv_icmp->code : 6;
		printf("%ld bytes from %s: Destination %s\n",
			   bytes - ip_hdr_len, ip, msg[code]);
		break;
	}

	case ICMP_TIME_EXCEEDED:
	{
		if (recv_icmp->code == 0)
			printf("%ld bytes from %s: Time to live exceeded\n", bytes - ip_hdr_len, ip);
		else if (recv_icmp->code == 1)
			printf("%ld bytes from %s: Fragment reassembly time exceeded\n", bytes - ip_hdr_len, ip);
		else
			printf("%ld bytes from %s: Time exceeded (unknown code %d)\n", bytes - ip_hdr_len, ip, recv_icmp->code);
		break;
	}

	case ICMP_PARAMETERPROB:
		printf("%ld bytes from %s: Parameter problem\n", bytes - ip_hdr_len, ip);
		break;

	case ICMP_SOURCE_QUENCH:
		printf("%ld bytes from %s: Source quench\n", bytes - ip_hdr_len, ip);
		break;

	case ICMP_REDIRECT:
	{
		const char *msg[] = {"Redirect Network", "Redirect Host", "Redirect TOS and Network", "Redirect TOS and Host"};
		int code = recv_icmp->code < 4 ? recv_icmp->code : 0;
		printf("%ld bytes from %s: %s\n", bytes - ip_hdr_len, ip, msg[code]);
		break;
	}

	default:
		printf("%ld bytes from %s: ICMP type %d code %d\n", bytes - ip_hdr_len, ip, recv_icmp->type, recv_icmp->code);
		break;
	}

	if (options->verbose)
	{
		printf("IP Header Dump:\n");
		for (ssize_t i = 0; i < 20 && i < bytes; i += 2)
			printf("%02x%02x ", ((unsigned char *)orig_iphdr)[i], ((unsigned char *)orig_iphdr)[i + 1]);
		printf("\n");
		printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst     Data\n");
		printf(" %1d  %1d  %02x %04x %04x   %1d %04x  %02x %02d %04x %s %s\n",
			   orig_iphdr->version,
			   orig_iphdr->ihl,
			   orig_iphdr->tos,
			   ntohs(orig_iphdr->tot_len),
			   ntohs(orig_iphdr->id),
			   (ntohs(orig_iphdr->frag_off) >> 13) & 0x7,
			   ntohs(orig_iphdr->frag_off) & 0x1FFF,
			   orig_iphdr->ttl,
			   orig_iphdr->protocol,
			   ntohs(orig_iphdr->check),
			   src_ip,
			   dst_ip);
		printf("ICMP: type %d, code %d, size %d, id 0x%x, seq 0x%04x\n",
			   orig_icmp->type,
			   orig_icmp->code,
			   options->size,
			   orig_icmp->un.echo.id,
			   ntohs(orig_icmp->un.echo.sequence));
	}
}

double process_reply(t_ping_ctx *ctx, char *buffer, ssize_t bytes, int id, char *dst_ip, int seq_number, char *ip, t_options *options, int *dup_seq)
{
	if (bytes < (ssize_t)sizeof(struct iphdr))
		return 0;

	struct iphdr *iphdr = (struct iphdr *)buffer;
	int ip_hdr_len = iphdr->ihl * 4;

	if (ip_hdr_len < 20 || ip_hdr_len > 60)
		return 0;

	bool truncated = bytes < ip_hdr_len + (ssize_t)sizeof(struct icmphdr);

	if (truncated)
	{
		printf("%ld bytes from %s: (TRUNCATED)\n", bytes, ip);
		return 0;
	}

	struct icmphdr *recv_icmp = (struct icmphdr *)(buffer + ip_hdr_len);

	if (ctx->have_timespec)
		truncated = bytes < ip_hdr_len + (ssize_t)sizeof(struct icmphdr) + (ssize_t)sizeof(struct timespec);

	uint16_t recv_checksum = recv_icmp->checksum;
	recv_icmp->checksum = 0;
	uint16_t calc_checksum = icmp_checksum(recv_icmp, bytes - ip_hdr_len);
	recv_icmp->checksum = recv_checksum;

	bool bad_checksum = recv_checksum != calc_checksum;

	int seq = ntohs(recv_icmp->un.echo.sequence);

	if (recv_icmp->type == ICMP_ECHOREPLY &&
		recv_icmp->un.echo.id == id &&
		(seq == seq_number || seq <= options->preload))
	{
		printf("%ld bytes from %s: icmp_seq=%d ttl=%d",
			   bytes - ip_hdr_len, ip, seq, iphdr->ttl);

		double rtt = 0;

		if (!truncated && ctx->have_timespec)
		{
			struct timespec *send_ts = (struct timespec *)(buffer + ip_hdr_len + sizeof(struct icmphdr));
			struct timespec recv_ts;

			if (clock_gettime(CLOCK_MONOTONIC, &recv_ts) < 0)
			{
				perror("ft_ping: getting time");
				return 0;
			}

			rtt = (recv_ts.tv_sec - send_ts->tv_sec) * 1000.0 +
				  (recv_ts.tv_nsec - send_ts->tv_nsec) / 1000000.0;

			printf(" time=%.3f ms", rtt);
		}

		dup_seq[seq]++;

		bool is_dup = dup_seq[seq] > 1;

		if (is_dup)
		{
			ctx->dup_packets++;
			printf(" (DUP!)");
		}

		if (bad_checksum)
			printf(" (BAD CHECKSUM)");

		if (truncated)
			printf(" (TRUNCATED)");

		printf("\n");

		return (truncated || bad_checksum) ? 0 : (is_dup ? -2 : rtt);
	}

	if (bytes < 2 * ip_hdr_len + 2 * (ssize_t)sizeof(struct icmphdr))
		return 0;

	struct icmphdr *orig_icmp = (struct icmphdr *)(buffer + 2 * ip_hdr_len + (ssize_t)sizeof(struct icmphdr));

	if (orig_icmp->un.echo.id != id)
		return 0;

	struct iphdr *orig_iphdr = (struct iphdr *)(buffer + ip_hdr_len + (ssize_t)sizeof(struct icmphdr));

	print_icmp_error(orig_iphdr, recv_icmp, orig_icmp, bytes, ip_hdr_len, options, inet_ntoa(*(struct in_addr *)&orig_iphdr->saddr), dst_ip);

	return 0;
}

void receive_replies(t_ping_ctx *ctx, int id, char *ip, int seq_number, t_options *options, int *dup_seq)
{
	char buffer[PACKET_BUFFER_SIZE];

	ssize_t bytes;

	double rtt;

	struct timespec start, now;
	if (clock_gettime(CLOCK_MONOTONIC, &start) < 0)
	{
		perror("ft_ping: getting time");
		cleanup_and_exit(ctx, 1);
	}

	double elapsed;
	double remaining;

	struct timeval tv;

	fd_set fds;

	int ret;

	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(struct sockaddr_in);

	while (is_open)
	{
		if (clock_gettime(CLOCK_MONOTONIC, &now) < 0)
		{
			perror("ft_ping: getting time");
			cleanup_and_exit(ctx, 1);
		}

		elapsed = (now.tv_sec - start.tv_sec) + (now.tv_nsec - start.tv_nsec) / 1000000000.0;
		remaining = options->interval - elapsed;

		if (remaining <= 0.0)
			break;

		tv.tv_sec = (int)remaining;
		tv.tv_usec = (remaining - tv.tv_sec) * 1000000;

		FD_ZERO(&fds);
		FD_SET(ctx->sockfd, &fds);

		ret = select(ctx->sockfd + 1, &fds, NULL, NULL, &tv);
		if (ret > 0)
		{
			bytes = recvfrom(ctx->sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, &addr_len);

			if (!is_open)
				return;

			if (bytes < 0)
			{
				if (errno == EINTR)
					continue;
				perror("ft_ping: receiving packet");
				cleanup_and_exit(ctx, 1);
			}
			if (bytes == 0)
				continue;

			rtt = process_reply(ctx, buffer, bytes, id, ip, seq_number, inet_ntoa(addr.sin_addr), options, dup_seq);

			if (rtt > 0)
			{
				ctx->success_packets++;

				if (options->count && ctx->success_packets == options->count)
					return;
			}
			else if (rtt < 0 && rtt != -2 && errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)
				cleanup_and_exit(ctx, 1);
		}
		else if (ret == 0)
			continue;
		else
		{
			if (errno == EINTR)
				break;
			perror("ft_ping: select error");
			cleanup_and_exit(ctx, 1);
		}
	}
}
