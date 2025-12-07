#include "ft_ping.h"

#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

void signal_control(int x)
{
	(void)x;
	is_open = 0;
}

void print_ping_header(char *raw_ip, char *ip, t_options *options, int id)
{
	printf("PING %s (%s): %d data bytes", raw_ip, ip, options->size - (int)sizeof(struct icmphdr));

	if (options->verbose)
		printf(", id 0x%x = %d", id, id);

	printf("\n");
}

static int resize_dup_array(int **dup_seq, int *dup_seq_size)
{
	*dup_seq_size += 1024;
	int *dup_seq_tmp = calloc(*dup_seq_size, sizeof(int));

	if (dup_seq_tmp == NULL)
		return -1;

	memcpy(dup_seq_tmp, *dup_seq, (*dup_seq_size - 1024) * sizeof(int));
	free(*dup_seq);
	*dup_seq = dup_seq_tmp;
	return 0;
}

static bool send_packet(t_ping_ctx *ctx, struct sockaddr_in *addr, short int size, int seq_number)
{
	prepare_packet(ctx, size, seq_number);

	if (sendto(ctx->sockfd, ctx->packet, size, 0,
			   (struct sockaddr *)addr, sizeof(*addr)) <= 0)
	{
		if (errno == EINTR)
			return false;
		perror("ft_ping: sending packet");
		cleanup_and_exit(ctx, 1);
	}

	return true;
}

int ping_loop(struct sockaddr_in *addr, char *raw_ip, char *ip, t_options *options, t_ping_ctx *ctx)
{
	ctx->sockfd = create_socket();
	if (ctx->sockfd < 0)
	{
		free(ip);
		exit(1);
	}

	if (setup_socket_options(ctx->sockfd) < 0)
		cleanup_and_exit(ctx, 1);

	ctx->packet = malloc(options->size);

	if (ctx->packet == NULL)
	{
		fprintf(stderr, "ft_ping: memory allocation failed\n");
		cleanup_and_exit(ctx, 1);
	}

	int id = getpid() & 0xFFFF;
	init_icmp_packet(ctx->packet, id);

	print_ping_header(raw_ip, ip, options, id);

	int seq_number = 0;
	int dup_seq_size = 1024;

	ctx->dup_seq = calloc(dup_seq_size, sizeof(int));

	if (ctx->dup_seq == NULL)
	{
		fprintf(stderr, "ft_ping: memory allocation failed\n");
		cleanup_and_exit(ctx, 1);
	}

	signal(SIGINT, signal_control);

	if (options->timeout)
	{
		signal(SIGALRM, signal_control);
		alarm(options->timeout);
	}

	while (is_open)
	{
		if (seq_number >= dup_seq_size)
		{
			if (resize_dup_array(&ctx->dup_seq, &dup_seq_size) < 0)
			{
				fprintf(stderr, "ft_ping: memory allocation failed\n");
				cleanup_and_exit(ctx, 1);
			}
		}

		if (seq_number == 0 && options->preload > 0)
		{
			for (int i = 0; is_open && i <= options->preload; i++)
			{
				if (seq_number >= dup_seq_size)
				{
					if (resize_dup_array(&ctx->dup_seq, &dup_seq_size) < 0)
					{
						fprintf(stderr, "ft_ping: memory allocation failed\n");
						cleanup_and_exit(ctx, 1);
					}
				}

				if (!send_packet(ctx, addr, options->size, seq_number))
					break;

				if (i != options->preload)
					seq_number++;
			}
		}
		else if (!send_packet(ctx, addr, options->size, seq_number))
			break;

		if (!is_open)
			break;

		receive_replies(ctx, id, ip, seq_number, options, ctx->dup_seq);

		if (!is_open || (options->count && ctx->success_packets == options->count))
			break;

		seq_number++;
	}

	close(ctx->sockfd);
	free(ctx->dup_seq);
	free(ctx->packet);
	free(ctx->ip);

	print_statistics(ctx, raw_ip, seq_number + 1);

	return id;
}
