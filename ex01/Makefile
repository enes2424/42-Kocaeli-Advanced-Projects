NAME = LinearCombination

SRCS = main.cpp

CXX = c++
RM = rm -f
FLAGS = -Wall -Wextra -Werror

OBJS	= $(SRCS:.cpp=.o)

.cpp.o:
	$(CXX) $(FLAGS) -c $< -o $@

all : $(NAME)

main.o: main.cpp Vector.hpp Vector.tpp Matrix.hpp Matrix.tpp

$(NAME) : $(OBJS)
		$(CXX) $(FLAGS) $(OBJS) -o $(NAME)

clean :
	$(RM) $(OBJS)
c: clean

fclean : clean
	$(RM) $(NAME)
f: fclean

re : fclean all
r: re

.PHONY : all clean c fclean f re r
