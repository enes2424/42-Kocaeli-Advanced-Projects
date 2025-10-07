ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME	= libft_malloc_$(HOSTTYPE).so
SYMLINK = libft_malloc.so
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -fPIC -fvisibility=hidden -pthread
LDFLAGS = -shared -pthread
RM		= rm -rf

SRCDIR = src
OBJDIR = obj
INCDIR = includes
BONUSDIR = bonus

SOURCES = $(SRCDIR)/alloc.c $(SRCDIR)/malloc.c $(SRCDIR)/free.c $(SRCDIR)/realloc.c $(SRCDIR)/show_alloc_mem.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
HEADERS = $(INCDIR)/malloc.h

LIBFT_PATH 	= $(INCDIR)/libft
LIBFT		= $(LIBFT_PATH)/libft.a

all: libft $(NAME) $(SYMLINK)

bonus:
	make -C $(BONUSDIR)

$(NAME): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBFT)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(SYMLINK): $(NAME)
	ln -s $(NAME) $(SYMLINK)

libft:
	make -C $(LIBFT_PATH)

clean:
	$(RM) $(OBJDIR)
	make clean -C $(LIBFT_PATH)
	make clean -C $(BONUSDIR)
c: clean

fclean: clean
	$(RM) $(NAME) $(SYMLINK)
	make fclean -C $(LIBFT_PATH)
	make fclean -C $(BONUSDIR)
f: fclean

re: fclean all
r: re

bonusre:
	make re -C $(BONUSDIR)
br: bonusre

comp: re
	@$(CC) $(CFLAGS) main.c -L. -lft_malloc -o malloc

test: comp
	@echo "------------------------------------------------"
	@LD_LIBRARY_PATH=. ./malloc
	@echo "------------------------------------------------"
	@$(RM) $(OBJDIR) $(NAME) $(SYMLINK) malloc
	@make fclean -C $(LIBFT_PATH)

.PHONY: all bonus libft clean c fclean f re r bonusre br comp test