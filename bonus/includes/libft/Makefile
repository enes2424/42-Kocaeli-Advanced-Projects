SRCS	=	ft_isprint.c ft_strlen.c ft_memmove.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_strncmp.c ft_memset.c

OBJS = $(SRCS:.c=.o)

NAME = libft.a

all : $(NAME)

$(OBJS): libft.h

$(NAME): $(OBJS)
		ar rcs $(NAME) $(OBJS)

%.o: %.c
	cc -Wall -Wextra -Werror -fPIC -fvisibility=hidden -c $< -o $@

clean :
	rm -rf $(OBJS) $(BONUS_OBJS)
c: clean

fclean : clean
	rm -rf $(NAME)
f: fclean

re : fclean $(NAME)
r: re

.PHONY : all clean c fclean f re r

