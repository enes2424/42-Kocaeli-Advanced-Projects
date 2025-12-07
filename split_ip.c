#include <stdlib.h>
#include <string.h>

void free_double_array(char **arr)
{
	for (size_t i = 0; arr[i]; i++)
		free(arr[i]);

	free(arr);
}

static size_t count_words(char const *s, char c)
{
	size_t num_of_words = 0;

	while (*s)
	{
		if (*s == c)
			s++;
		if (*s == c)
			return 0;

		if (!*s)
			break;

		num_of_words++;

		while (*s && *s != c)
			s++;
	}
	return (num_of_words);
}

static char *ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t string_len = strlen(s);

	if (string_len < start)
		return (strdup(""));

	if (string_len - start < len)
		len = string_len - start;

	char *ret = malloc(len + 1);

	if (!ret)
		return (0);

	strncpy(ret, s + start, len);

	ret[len] = '\0';

	return (ret);
}

char **split_ip(char const *s)
{
	char c = '.';

	if (!*s || *s == c || s[strlen(s) - 1] == c)
		return (NULL);

	size_t len = count_words(s, c);

	if (len > 4 || len == 0)
		return (NULL);

	char **arr = malloc(sizeof(char *) * (len + 1));

	if (!arr)
		return (NULL);

	for (size_t i = 0; i < len; i++)
	{
		while (*s == c)
			s++;

		arr[i] = ft_substr(s, 0, strchr(s, c) - s);

		if (!arr[i])
		{
			free_double_array(arr);

			return NULL;
		}

		s += strlen(arr[i]);
	}

	arr[len] = NULL;

	return (arr);
}
