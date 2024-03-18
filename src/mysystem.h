#ifndef MYSYSTEM_H
#define MYSYSTEM_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>

// Shop parameters
#define SHOPS_COUNT 5
#define SHOP_MAX_WEIGHT 10000
// Loader parameters
#define LOADER_WEIGHT 5000
// Customer parameters
#define CUSTOMERS_COUNT 3
#define CUSTOMER_MAX_NEED 100000

// Print arguments width
#define PRINT_TID_WIDTH 3
#define PRINT_NEED_WIDTH 5 // SHOP_MAX_WEIGHT symbol length
#define SHOP_ID_WIDTH 1
#define PRINT_WEIGHT_WIDTH PRINT_NEED_WIDTH

struct Shop {
    int weight;
    pthread_mutex_t mutex;
};

extern struct Shop chain_shop[];
extern pthread_key_t key_customer_need;
extern pthread_once_t once_init_cust_need;
extern int customers_is_dead;

void* loader_thread();
void key_cust_need_create();
void key_cust_need_free(void* value);
void* customer_thread();

#endif // MYSYSTEM_H