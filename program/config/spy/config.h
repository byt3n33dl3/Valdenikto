#pragma once

#include "ipv4.h"

typedef struct
{
	int thread_count;
	int timeout;
	struct
	{
		struct
		{
			char * string;
			ipv4_t * ipv4;
		} bottom;
		struct
		{
			char * string;
			ipv4_t * ipv4;
		} top;
	} range;
} config_t;
