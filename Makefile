NAME        = libasm.a
TEST_NAME   = test

NASM        = nasm
CC          = gcc
AR          = ar rcs

NASMFLAGS   = -f elf64

CFLAGS      = -Wall -Wextra -Werror

ASM_SRC     = ft_strlen.s ft_strcpy.s ft_strcmp.s ft_write.s ft_read.s ft_strdup.s

ASM_OBJ     = $(ASM_SRC:.s=.o)

BONUS_SRC   = ft_atoi_base_bonus.s ft_list_push_front_bonus.s ft_list_size_bonus.s ft_list_sort_bonus.s ft_list_remove_if_bonus.s

BONUS_OBJ   = $(BONUS_SRC:.s=.o)

TEST_SRC    = main.c
TEST_OBJ    = $(TEST_SRC:.c=.o)

all: $(NAME)

$(NAME): $(ASM_OBJ)
	$(AR) $(NAME) $(ASM_OBJ)

%.o: %.s
	$(NASM) $(NASMFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

bonus: .bonus

.bonus: $(BONUS_OBJ) 
	$(AR) $(NAME) $(BONUS_OBJ)
	@touch .bonus

test: $(TEST_OBJ)
	@$(CC) $(TEST_OBJ) $(BONUS_OBJ) -L. -lasm -o $(TEST_NAME)
	@./$(TEST_NAME)

vtest: $(TEST_OBJ)
	@$(CC) $(TEST_OBJ) $(BONUS_OBJ) -L. -lasm -o $(TEST_NAME)
	@valgrind ./$(TEST_NAME)

clean:
	rm -f $(ASM_OBJ) $(BONUS_OBJ) $(TEST_OBJ) .bonus

fclean: clean
	rm -f $(NAME) $(TEST_NAME)

re: fclean all

.PHONY: all clean fclean re test vtest bonus
