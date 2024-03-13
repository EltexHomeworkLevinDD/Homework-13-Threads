#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <stdlib.h>
#include <pthread.h>
#include <time.h>

extern struct Shop;
extern Shop chain_shop[];

#define CUST_MAX_NEED 100000

pthread_key_t key_customer_need;
pthread_once_t once_init_cust_need = PTHREAD_ONCE_INIT;
pthread_mutex_t mutex_enter_shop = PTHREAD_MUTEX_INITIALIZER;

#endif // CUSTOMER_H