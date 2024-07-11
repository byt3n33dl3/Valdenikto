#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "ping.h"
#include "ipv4.h"

bool ping(char * ipv4)
{
    char * command = malloc(sizeof(char) * 128);
    sprintf(command, "ping -c 1 %s >/dev/null 2>&1", ipv4);
    if(system(command) == 0)
    {
        free(command);
        return true;
    }
    free(command);
    return false;
}
