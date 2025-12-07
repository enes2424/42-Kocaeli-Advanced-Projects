#include "ft_ping.h"

#include <netinet/ip_icmp.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define INT 0
#define SHORT 1
#define LONG 3

static void suggest_help_or_usage_option()
{
	fprintf(stderr, "Try './ft_ping --help' or './ft_ping --usage' for more information.\n");
	exit(64);
}

static void missing_error()
{
	fprintf(stderr, "ft_ping: missing host operand\n");
	suggest_help_or_usage_option();
}

static void invalid_option_error(char c)
{
	fprintf(stderr, "ft_ping: invalid option -- '%c'\n", c);
	suggest_help_or_usage_option();
}

static void unrecognized_option_error(char *option)
{
	fprintf(stderr, "ft_ping: unrecognized option '--%s'\n", option);
	suggest_help_or_usage_option();
}

static void option_doesnt_allow_argument_error(char *option)
{
	fprintf(stderr, "ft_ping: option '--%s' doesn't allow an argument\n", option);
	suggest_help_or_usage_option();
}

static void ambiguous_option_error()
{
	fprintf(stderr, "ft_ping: option '--=' is ambiguous; possibilities: '--preload'\n");
	suggest_help_or_usage_option();
}

static void option_requires_argument_error(char *option)
{
	fprintf(stderr, "ft_ping: option '--%s' requires an argument\n", option);
	suggest_help_or_usage_option();
}

static void short_option_requires_argument_error(char option)
{
	fprintf(stderr, "ft_ping: option requires an argument -- '%c'\n", option);
	suggest_help_or_usage_option();
}

static void invalid_preload_value_error(char *preload_arg)
{
	fprintf(stderr, "ft_ping: invalid preload value (%s)\n", preload_arg);
	exit(1);
}

void option_too_small_error(char *arg)
{
	fprintf(stderr, "ft_ping: option value too small: %s\n", arg);
	exit(1);
}

void option_too_big_error(char *arg)
{
	fprintf(stderr, "ft_ping: option value too big: %s\n", arg);
	exit(1);
}

void invalid_value_error(char *orig_arg, char *remaining_arg)
{
	fprintf(stderr, "ft_ping: invalid value (`%s' near `%s')\n", orig_arg, remaining_arg);
	exit(1);
}

long try_convert_int(char *arg, int flag)
{
	if (arg[0] == '\0')
		return 0;

	char *orig_arg = arg;

	while (isspace(*arg))
		arg++;

	bool is_negative = (arg[0] == '-');

	if (is_negative)
		arg++;

	if (arg[0] == '\0' || !isdigit(arg[0]))
		invalid_value_error(orig_arg, orig_arg);

	for (int i = 1; arg[i]; i++)
		if (!isdigit(arg[i]))
			invalid_value_error(orig_arg, &arg[i]);

	if (flag == LONG)
		return atol(orig_arg);

	if (is_negative)
		option_too_big_error(orig_arg);

	size_t len = strlen(arg);

	if (flag == SHORT)
	{
		if (len > 5 || (len == 5 && strcmp(arg, "65399") > 0))
			option_too_big_error(orig_arg);
	}
	else
	{
		if (len > 10 || (len == 10 && strcmp(arg, "2147483647") > 0))
			option_too_big_error(orig_arg);
	}

	return atoi(orig_arg);
}

double try_convert_double(char *arg)
{
	if (arg[0] == '\0')
		return 0;

	char *orig_arg = arg;

	while (isspace(*arg))
		arg++;

	bool is_negative = (arg[0] == '-');

	if (is_negative)
		arg++;

	if (arg[0] == '\0')
		invalid_value_error(orig_arg, orig_arg);

	char *end;

	double res = strtod(arg, &end);

	if (end[0] != '\0')
		invalid_value_error(orig_arg, end);

	if (!is_negative && res >= 18446744073709550.0)
		option_too_small_error(orig_arg);

	return is_negative ? -res : res;
}

int try_convert_int_for_preload(char *preload_arg)
{
	if (preload_arg[0] == '\0')
		return 0;

	while (isspace(*preload_arg))
		preload_arg++;

	size_t len = strlen(preload_arg);

	if (len == 0 || len > 10)
		return -1;

	for (int i = 0; preload_arg[i]; i++)
		if (!isdigit(preload_arg[i]))
			return -1;

	if (len == 10 && strcmp(preload_arg, "2147483647") > 0)
		return -1;

	return atoi(preload_arg);
}

void control_args(int argc, char **argv, char **ip, t_options *options)
{
	if (argc == 0)
		missing_error();

	char *arg;

	size_t len;

	bool arg_found;

	for (int i = 0; i < argc; i++)
	{
		arg = argv[i];

		if (arg[0] == '-' && arg[1])
		{
			argv[i] = NULL;

			if (arg[1] == '-')
			{
				arg = arg + 2;

				arg_found = strchr(arg, '=') != NULL;

				if (arg_found)
					*strchr(arg, '=') = '\0';

				len = strlen(arg);

				if (len == 0)
				{
					if (arg_found)
						ambiguous_option_error();

					if (*ip == NULL)
						*ip = argv[i + 1];

					break;
				}
				else if (!strncmp(arg, "count", len))
				{
					char *count_arg;

					if (arg_found)
						count_arg = arg + len + 1;
					else
						count_arg = argv[++i];

					if (count_arg == NULL)
						option_requires_argument_error("count");

					long count_arg_long = try_convert_int(count_arg, LONG);

					if (count_arg_long < 0)
						count_arg_long = 0;

					options->count = count_arg_long;
				}
				else if (!strncmp(arg, "interval", len))
				{
					char *interval_arg;

					if (arg_found)
						interval_arg = arg + len + 1;
					else
						interval_arg = argv[++i];

					if (interval_arg == NULL)
						option_requires_argument_error("interval");

					double interval_arg_double = try_convert_double(interval_arg);

					if (interval_arg_double >= 0 && interval_arg_double < 0.2)
						option_too_small_error(interval_arg);

					options->interval = interval_arg_double;
				}
				else if (!strncmp(arg, "verbose", len))
				{
					if (arg_found)
						option_doesnt_allow_argument_error("verbose");

					options->verbose = true;
				}
				else if (!strncmp(arg, "timeout", len))
				{
					char *timeout_arg;

					if (arg_found)
						timeout_arg = arg + len + 1;
					else
						timeout_arg = argv[++i];

					if (timeout_arg == NULL)
						option_requires_argument_error("timeout");

					int timeout_arg_integer = try_convert_int(timeout_arg, INT);

					if (timeout_arg_integer == 0)
						option_too_small_error(timeout_arg);

					if (timeout_arg_integer == -1)
						exit(1);

					options->timeout = timeout_arg_integer;
				}
				else if (!strncmp(arg, "preload", len))
				{
					char *preload_arg;

					if (arg_found)
						preload_arg = arg + len + 1;
					else
						preload_arg = argv[++i];

					if (preload_arg == NULL)
						option_requires_argument_error("preload");

					int preload_arg_integer = try_convert_int_for_preload(preload_arg);

					if (preload_arg_integer == -1)
						invalid_preload_value_error(preload_arg);

					options->preload = preload_arg_integer;
				}
				else if (!strncmp(arg, "size", len))
				{
					char *size_arg;

					if (arg_found)
						size_arg = arg + len + 1;
					else
						size_arg = argv[++i];

					if (size_arg == NULL)
						option_requires_argument_error("size");

					short int size_arg_short_integer = try_convert_int(size_arg, SHORT);

					if (size_arg_short_integer == -1)
						exit(1);

					options->size = size_arg_short_integer + sizeof(struct icmphdr);
				}
				else if (!strncmp(arg, "help", len))
				{
					if (arg_found)
						option_doesnt_allow_argument_error("help");

					help_option();
				}
				else if (!strncmp(arg, "usage", len))
				{
					if (arg_found)
						option_doesnt_allow_argument_error("usage");

					usage_option();
				}
				else
					unrecognized_option_error(arg);
			}
			else
			{
				for (int j = 1; arg[j]; j++)
				{
					arg_found = arg[j + 1] != '\0';

					if (arg[j] == 'c')
					{
						char *count_arg;

						if (arg_found)
							count_arg = arg + j + 1;
						else
							count_arg = argv[++i];

						if (count_arg == NULL)
							short_option_requires_argument_error('c');

						long count_arg_long = try_convert_int(count_arg, LONG);

						if (count_arg_long < 0)
							count_arg_long = 0;

						options->count = count_arg_long;

						break;
					}
					else if (arg[j] == 'i')
					{
						char *interval_arg;

						if (arg_found)
							interval_arg = arg + j + 1;
						else
							interval_arg = argv[++i];

						if (interval_arg == NULL)
							short_option_requires_argument_error('i');

						double interval_arg_double = try_convert_double(interval_arg);

						if (interval_arg_double >= 0 && interval_arg_double < 0.2)
							option_too_small_error(interval_arg);

						options->interval = interval_arg_double;

						break;
					}
					else if (arg[j] == 'v')
						options->verbose = true;
					else if (arg[j] == 'w')
					{
						char *timeout_arg;

						if (arg_found)
							timeout_arg = arg + j + 1;
						else
							timeout_arg = argv[++i];

						if (timeout_arg == NULL)
							short_option_requires_argument_error('w');

						int timeout_arg_integer = try_convert_int(timeout_arg, INT);

						if (timeout_arg_integer == 0)
							option_too_small_error(timeout_arg);

						if (timeout_arg_integer == -1)
							exit(1);

						options->timeout = timeout_arg_integer;

						break;
					}
					else if (arg[j] == 'l')
					{
						char *preload_arg;

						if (arg_found)
							preload_arg = arg + j + 1;
						else
							preload_arg = argv[++i];

						if (preload_arg == NULL)
							short_option_requires_argument_error('l');

						int preload_arg_integer = try_convert_int_for_preload(preload_arg);

						if (preload_arg_integer == -1)
							invalid_preload_value_error(preload_arg);

						options->preload = preload_arg_integer;

						break;
					}
					else if (arg[j] == 's')
					{
						char *size_arg;

						if (arg_found)
							size_arg = arg + j + 1;
						else
							size_arg = argv[++i];

						if (size_arg == NULL)
							short_option_requires_argument_error('s');

						short int size_arg_short_integer = try_convert_int(size_arg, SHORT);

						if (size_arg_short_integer == -1)
							exit(1);

						options->size = size_arg_short_integer + sizeof(struct icmphdr);

						break;
					}
					else
					{
						if (arg[j] == '?')
							help_option();

						invalid_option_error(arg[j]);
					}
				}
			}

			if (!arg_found)
				argv[i] = NULL;
		}
		else if (*ip == NULL)
			*ip = arg;
	}

	if (*ip == NULL)
		missing_error();
}
