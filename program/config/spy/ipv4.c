#include <stdio.h>
#include <stdlib.h>

#include "ipv4.h"

char * ipv4_to_string(ipv4_t * ipv4)
{
    char * string = malloc(sizeof(char) * 32);
    sprintf(string, "%d.%d.%d.%d", (int)ipv4->a, (int)ipv4->b, (int)ipv4->c, (int)ipv4->d);
    return string;
}

ipv4_t * string_to_ipv4(char * string)
{
    ipv4_t * ipv4 = malloc(sizeof(ipv4_t));
    sscanf(string, "%d.%d.%d.%d", (int *)&ipv4->a, (int *)&ipv4->b, (int *)&ipv4->c, (int *)&ipv4->d);
    return ipv4;
}