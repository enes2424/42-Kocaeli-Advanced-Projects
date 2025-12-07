#include <stdlib.h>
#include <stdio.h>

void usage_option()
{
    printf("Usage: ./ft_ping [-v?] [-c NUMBER] [-i NUMBER] [-w N] [-l NUMBER] [-s NUMBER]\n");
    printf("                 [--count=NUMBER] [--interval=NUMBER] [--verbose] [--timeout=N]\n");
    printf("                 [--preload=NUMBER] [--size=NUMBER] [--help] [--usage]\n");
    printf("                 HOST ...\n");
    exit(0);
}
