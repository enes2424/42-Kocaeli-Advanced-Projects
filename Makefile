NAME	= ft_ping

SRCS	= main.c ft_ping.c args_control.c usage_option.c help_option.c split_ip.c init_addr.c \
		  socket_setup.c packet.c receive.c statistics.c cleanup.c ping_loop.c \
		  handle_remaining_hosts.c

OBJS	= $(SRCS:.c=.o)

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g
LDFLAGS	= -lm

%.o: %.c ft_ping.h
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

c: clean
f: fclean
r: re

.PHONY: all clean c fclean f re r
