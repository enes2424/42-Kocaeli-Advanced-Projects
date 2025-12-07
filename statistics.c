#include "ft_ping.h"

#include <float.h>
#include <stdio.h>
#include <math.h>

void print_statistics(t_ping_ctx *ctx, char *raw_ip, int transmitted)
{
	int loss = 0;

	if (transmitted > 0)
		loss = (transmitted - ctx->success_packets) * 100 / transmitted;

	printf("--- %s ping statistics ---\n", raw_ip);
	printf("%d packets transmitted, %d packets received",
		   transmitted, ctx->success_packets);
	if (ctx->dup_packets > 0)
		printf(", +%d duplicates", ctx->dup_packets);
	printf(", %d%% packet loss\n", loss);
}
