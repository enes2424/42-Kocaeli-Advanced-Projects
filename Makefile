NAME = ft_turing
ENTRY_POINT = bin/ft_turing.js lib/*
PERMISSIONS = --allow-all

all: $(NAME)

$(NAME): $(ENTRY_POINT)
	deno compile $(PERMISSIONS) $(ENTRY_POINT)

run1: all
	@./ft_turing machines/unary_addition.json "111+11=" || true

run2: all
	@./ft_turing machines/palindrome.json "0101010" || true

run3: all
	@./ft_turing machines/equal.json "00001111" || true

run4: all
	@./ft_turing machines/even_zeros.json "000000" || true

run5: all
	@./ft_turing machines/unary_sub.json "111-11=" || true

clean:
	rm -f $(NAME)
c: clean

re: clean all
r: re

.PHONY: all clean c re r
