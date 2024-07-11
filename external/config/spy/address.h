#pragma once

#include <stdint.h>

#include "ipv4.h"

typedef struct
{
	ipv4_t *ipv4;
	uint16_t port;
} address_t;

address_t * string_to_address(char * string);
char * address_to_tring(address_t * address);
