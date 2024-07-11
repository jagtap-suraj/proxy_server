#include "proxy_parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h> // for time_t

#define MAX_CLIENTS 10

typedef struct cache_element cache_element;

struct cache_element
{
    char* responseData; // to store response
    int responseLength; // to store the length of response
    char* endpointUrl; // to store the request url
    time_t lru_time_track; // to store the latest time the element is accessed
    cache_element* next; // pointer to the next element
};

cache_element* find(char* url);
int add_cache_element(char* responseData, int responseLength, char* endpointUrl);
void remove_cache_element();

int port_number = 8080;	// default port number
int proxy_socketId; // socket id for proxy server
pthread_t tid[MAX_CLIENTS]; // we are alloting one thread per client so here we are storing all the thread ids
// each socket connection is handled by a separate thread   

// Let's suppose a client requests google.com, and we store the response in a cache.
// If at the same time another client requests facebook.com, 
// since the LRU cache is a shared resource,
// concurrent access by thread1 and thread2 could lead to a race condition.
// This can be managed using a mutex lock.
