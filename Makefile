NAME	= computor

SRCS	= main.cpp utils.cpp parser.cpp solver.cpp

OBJS	= $(SRCS:.cpp=.o)

HEADERS	= utils.hpp parser.hpp solver.hpp

CC		= c++
CFLAGS	= -Wall -Wextra -Werror

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

c: clean
f: fclean
r: re

.PHONY: all clean c fclean f re r
