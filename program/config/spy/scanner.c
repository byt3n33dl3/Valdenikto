#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "scanner.h"
#include "ipv4.h"
#include "check.h"
#include "address.h"
#include "ping.h"

extern char * log_filename;

extern char * proxy_ip;
extern int proxy_port;
extern int in_general;

typedef struct
{
    char *ip_address;
    int port;
    int timeout;
} threadargs_t;

void *scanner_thread(void *arg)
{
    threadargs_t *args = (threadargs_t *)arg;

    char *ip_address = args->ip_address;
    int port = args->port;
    int timeout = args->timeout;

    int proxy_or_not = 1;

    if(proxy_ip == NULL && proxy_port == -1)
    {
        proxy_or_not = 0;
    }

    if(in_general == 0)
    {
        switch(check(ip_address, port, timeout, proxy_ip, proxy_port, proxy_or_not))
        {
        case 0:
            printf("%s:%d => \x1b[32;1monline\x1b[0m\n", ip_address, port);

            if(log_filename != NULL)
            {
                FILE * log_file = fopen(log_filename, "a");
                if(log_file == NULL)
                {
                    fprintf(stderr, "\x1b[31;1mfatal error: error opening %s\x1b[0m\n", log_filename);
                    fclose(log_file);
                    exit(EXIT_FAILURE);
                }
                fprintf(log_file, "%s:%d\n", ip_address, port);
                fclose(log_file);
            }
            break;
        case -1:
            printf("%s:%d => \x1b[31;1mfailed to create socket\x1b[0m\n", ip_address, port);
            break;
        case -2:
            printf("%s:%d => \x1b[31;1mfailed to parse IP address\x1b[0m\n", ip_address, port);
            break;
        case -3:
           /*
            *  Either timed out, since it was timed out, there's no way to know whether the address is
            *  alive or not, unless if a larger timeout is set, although it is most likely that the address
            *  is not alive or an available connection.
            */
            printf("%s:%d => \x1b[31;1munreachable\x1b[0m\n", ip_address, port);
            break;
        case -4:
            printf("%s:%d => \x1b[31;1mfailed to check socket status\x1b[0m\n", ip_address, port);
            break;
        case -5:
            /* Connection failed or was rejected. */
            printf("%s:%d => \x1b[31;1mrejected\x1b[0m\n", ip_address, port);
            break;
        }
    }
    else if(in_general == 1)
    {
        switch(ping(ip_address))
        {
        case true:
            printf("%s => \x1b[32;1monline\x1b[0m\n", ip_address);
            break;
        case false:
            printf("%s => \x1b[31;1moffline\x1b[0m\n", ip_address);
            break;
        }
    }

    free(ip_address);
    free(args);
    pthread_exit(NULL);
}

void scanner(ipv4_t *ip0, ipv4_t *ip1, int timeout, int thread_count, uint16_t port)
{
    pthread_t threads[thread_count];
    int current_thread = 0;

    for (int a = (int)ip0->a; a <= (int)ip1->a; a++)
    {
        for (int b = (int)ip0->b; b <= (int)ip1->b; b++)
        {
            for (int c = (int)ip0->c; c <= (int)ip1->c; c++)
            {
                for (int d = (int)ip0->d; d <= (int)ip1->d; d++)
                {
                    /*
                     *  Create a new thread to check whether an address is alive/available or not. Once
                     *  the max thread limit is reached, iSpy will wait for all threads to finish so that
                     *  it can create a new set of threads.
                     */

                    ipv4_t *ipv4 = malloc(sizeof(ipv4_t));
                    ipv4->a = a;
                    ipv4->b = b;
                    ipv4->c = c;
                    ipv4->d = d;
                    char *ip_address = ipv4_to_string(ipv4);

                    threadargs_t *args = malloc(sizeof(threadargs_t));
                    args->ip_address = ip_address;
                    args->port = port;
                    args->timeout = timeout;

                    pthread_create(&threads[current_thread], NULL, scanner_thread, args);
                    current_thread = (current_thread + 1) % thread_count;

                    if (current_thread == 0)
                    {
                        for (int i = 0; i < thread_count; i++)
                        {
                            pthread_join(threads[i], NULL);
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < current_thread; i++)
    {
        pthread_join(threads[i], NULL);
    }
}
