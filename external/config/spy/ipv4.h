#pragma once

#include <stdint.h>

/* This structure defines an IPv4 address. */
typedef struct
{
    uint8_t a, b, c, d;
} ipv4_t;

char * ipv4_to_string(ipv4_t * ipv4);
ipv4_t * string_to_ipv4(char * string);
