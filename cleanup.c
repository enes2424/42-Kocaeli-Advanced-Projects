#include "ft_ping.h"

#include <stdlib.h>
#include <unistd.h>

void cleanup_resources(t_ping_ctx *ctx)
{
	if (ctx->sockfd >= 0)
		close(ctx->sockfd);

	free(ctx->dup_seq);
	free(ctx->packet);
	free(ctx->ip);
}

void cleanup_and_exit(t_ping_ctx *ctx, int exit_code)
{
	cleanup_resources(ctx);
	exit(exit_code);
}
