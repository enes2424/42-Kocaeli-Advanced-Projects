#ifndef FT_PING_H
#define FT_PING_H

#include <arpa/inet.h>
#include <stdbool.h>
#include <signal.h>

#define PACKET_BUFFER_SIZE 65535
#define ICMP_PATTERN 0x42
#define CLOCK_MONOTONIC 1

typedef struct s_options
{
	bool verbose;
	int preload;
	int timeout;
	long count;
	double interval;
	short int size;
} t_options;

typedef struct s_ping_ctx
{
	int sockfd;
	void *packet;
	int *dup_seq;
	char *ip;
	int success_packets;
	int dup_packets;
	bool have_timespec;
} t_ping_ctx;

extern volatile sig_atomic_t is_open;

// args_control.c
void control_args(int argc, char **argv, char **ip, t_options *options);

// usage_option.c
void usage_option(void);

// help_option.c
void help_option(void);

// split_ip.c
void free_double_array(char **arr);
char **split_ip(char const *s);

// init_addr.c
char *init_addr(char *ip, struct sockaddr_in *addr);

// ping_loop.c
void signal_control(int x);
void print_ping_header(char *raw_ip, char *ip, t_options *options, int id);
int ping_loop(struct sockaddr_in *addr, char *raw_ip, char *ip, t_options *options, t_ping_ctx *ctx);

// socket_setup.c
int create_socket(void);
int setup_socket_options(int sockfd);

// packet.c
uint16_t icmp_checksum(void *buf, size_t len);
void init_icmp_packet(void *packet, int id);
void prepare_packet(t_ping_ctx *ctx, short int size, int seq_number);

// receive.c
double process_reply(t_ping_ctx *ctx, char *buffer, ssize_t bytes, int id, char *target_ip, int seq_number,
					 char *ip, t_options *options, int *dup_seq);
void receive_replies(t_ping_ctx *ctx, int id, char *ip, int seq_number,
					 t_options *options, int *dup_seq);

// statistics.c
void print_statistics(t_ping_ctx *ctx, char *raw_ip, int transmitted);

// cleanup.c
void cleanup_resources(t_ping_ctx *ctx);
void cleanup_and_exit(t_ping_ctx *ctx, int exit_code);

// handle_remaining_hosts.c
void handle_remaining_hosts(char **argv, int argc, char *raw_ip, t_options *options, int id);

#endif
