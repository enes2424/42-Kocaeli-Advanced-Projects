#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct s_list
{
	void *data;
	struct s_list *next;
} t_list;

size_t ft_strlen(const char *str);
char *ft_strcpy(char *dst, const char *src);
int ft_strcmp(const char *s1, const char *s2);
ssize_t ft_write(int fd, const void *buf, size_t count);
ssize_t ft_read(int fd, void *buf, size_t count);
char *ft_strdup(const char *s);
int ft_atoi_base(const char *str, const char *base);
void ft_list_push_front(t_list **begin_list, void *data);
int ft_list_size(t_list *begin_list);
void ft_list_sort(t_list **begin_list, int (*cmp)());
void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *));

void free_data(void *data)
{
	free(data);
}

void print_list(t_list *lst)
{
	while (lst)
	{
		printf("%s -> ", (char *)lst->data);
		lst = lst->next;
	}
	printf("NULL\n");
}

int main(void)
{
	printf("ft_strlen: %zu\n", ft_strlen("Hello world"));

	char dest[100];
	ft_strcpy(dest, "Assembly!");
	printf("ft_strcpy: %s\n", dest);

	printf("ft_strcmp (equal): %d\n", ft_strcmp("abc", "abc"));
	printf("ft_strcmp (diff):  %d\n", ft_strcmp("abz", "abd"));

	ssize_t ret = ft_write(1, "ft_write: Hello\n", 16);
	printf("ft_write return: %zd | errno: %d\n", ret, errno);

	char buffer[100];
	int fd = 0;
	printf("Enter something for ft_read: ");
	ret = ft_read(fd, buffer, 99);
	if (ret >= 0)
	{
		buffer[ret] = '\0';
		printf("You typed: %s", buffer);
	}
	else
	{
		perror("ft_read");
	}

	char *dup = ft_strdup("duplicate me");
	printf("ft_strdup: %s\n", dup);
	free(dup);

	printf("ft_atoi_base (42, base 10): %d\n", ft_atoi_base("42", "0123456789"));
	printf("ft_atoi_base (2A, base 16): %d\n", ft_atoi_base("2A", "0123456789ABCDEF"));
	printf("ft_atoi_base (1010, base 2): %d\n", ft_atoi_base("1010", "01"));

	t_list *list = NULL;
	ft_list_push_front(&list, ft_strdup("a"));
	ft_list_push_front(&list, ft_strdup("e"));
	ft_list_push_front(&list, ft_strdup("c"));
	ft_list_push_front(&list, ft_strdup("b"));
	ft_list_push_front(&list, ft_strdup("d"));

	printf("List after push_front:\n");
	print_list(list);

	printf("ft_list_size: %d\n", ft_list_size(list));
	ft_list_sort(&list, strcmp);
	printf("List after sort:\n");
	print_list(list);
	ft_list_remove_if(&list, "d", strcmp, free_data);
	printf("List after remove_if(\"d\"):\n");
	print_list(list);

	while (list)
	{
		t_list *tmp = list;
		list = list->next;
		free(tmp->data);
		free(tmp);
	}

	return 0;
}
