#include <stdlib.h>
#include <stdio.h>

void help_option()
{
	printf(
		"Usage: ./ft_ping [OPTION...] HOST ...\n"
		"Send ICMP ECHO_REQUEST packets to network hosts.\n"
		"\n"
		" Options valid for all request types:\n"
		"  -c, --count=NUMBER         stop after sending NUMBER packets\n"
		"  -i, --interval=NUMBER      wait NUMBER seconds between sending each packet\n"
		"  -v, --verbose              verbose output\n"
		"  -w, --timeout=N            stop after N seconds\n"
		"\n"
		" Options valid for --echo requests:\n"
		"  -l, --preload=NUMBER       send NUMBER packets as fast as possible before\n"
		"                             falling into normal mode of behavior (root only)\n"
		"  -s, --size=NUMBER          send NUMBER data octets"
		"  -?, --help                 give this help list\n"
		"      --usage                give a short usage message\n"
		"\n"
		"Mandatory or optional arguments to long options are also mandatory or optional\n"
		"for any corresponding short options.\n"
		"\n"
		"Options marked with (root only) are available only to superuser.\n"
		"\n"
		"Report bugs to <bug-inetutils@gnu.org>.\n");
	exit(0);
}
