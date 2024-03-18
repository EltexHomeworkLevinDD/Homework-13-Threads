#ifndef MYSYSTEM_H
#define MYSYSTEM_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>

#define SHOPS_COUNT 5
#define SHOP_MAX_WEIGHT 10000
#define SHOP_ST_BUSY 0
#define SHOP_ST_FREE 1

#define LOADER_WEIGHT 5000

#define CUSTOMERS_COUNT 3
#define CUSTOMER_MAX_NEED 100000

struct Shop {
    int shop_weight;
    int status;
};

extern struct Shop chain_shop[];
extern pthread_key_t key_customer_need;
extern pthread_once_t once_init_cust_need;
extern pthread_mutex_t mutex_enter_shop;
extern int* customers_status[];

void* loader_thread(void* args);
void key_cust_need_create();
void key_cust_need_free(void* value);
void* customer_thread(void* args);

#endif // MYSYSTEM_H