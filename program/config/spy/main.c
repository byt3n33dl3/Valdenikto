#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "ipv4.h"
#include "scanner.h"
#include "address.h"

int thread_count = 5;
int timeout = 1;
char * opt_ip0 = NULL;
char * opt_ip1 = NULL;
char * log_filename = NULL;
int port = 80;
int in_general = 0;

char * proxy_ip = NULL;
int proxy_port = -1;

int main(int argc, char * argv[])
{
    if(argc < 2)
    {
        goto usage_err;
    }

    int opt;
    while ((opt = getopt(argc, argv, "t:T:0:1:p:l:P:v:g:")) != -1) {
        switch (opt) {
            case 't':
                thread_count = atoi(optarg);
                break;
            case 'T':
                timeout = atoi(optarg);
                break;
            case '0':
                opt_ip0 = optarg;
                break;
            case '1':
                opt_ip1 = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'l':
                log_filename = optarg;
                break;
            case 'P':
                proxy_ip = optarg;
                break;
            case 'v':
                proxy_port = atoi(optarg);
                break;
            case 'g':
                in_general = atoi(optarg);
                break;
            case '?':
                goto usage_err;
                break;
            default:
usage_err:
                fprintf(stderr, "Usage: %s -t <thread_count> -T <timeout> -0 <ip0> -1 <ip1> -p <port>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if(opt_ip0 == NULL)
    {
        goto usage_err;
    }

    if(opt_ip1 == NULL)
    {
        goto usage_err;
    }

    ipv4_t * ip0 = NULL;
    ipv4_t * ip1 = NULL;

    ip0 = string_to_ipv4(opt_ip0);
    ip1 = string_to_ipv4(opt_ip1);

    scanner(ip0, ip1, timeout, thread_count, port);

    exit(EXIT_SUCCESS);
}
