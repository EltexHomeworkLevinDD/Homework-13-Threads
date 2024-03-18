#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "mysystem.h"

int main()
{
    for (int i = 0; i < SHOPS_COUNT; i++){
        chain_shop[i].shop_weight = rand() % SHOP_MAX_WEIGHT;
        chain_shop[i].status = SHOP_ST_FREE;
        printf("Shop[%3d][%5d]\n", i, chain_shop[i].shop_weight);
    }

    pthread_t loader;
    int* loader_status;
    pthread_create(&loader, NULL, loader_thread, NULL);

    pthread_t customers[CUSTOMERS_COUNT];

    for (int i = 0; i < CUSTOMERS_COUNT; i++)
        pthread_create(&customers[i], NULL, customer_thread, NULL);
    
    for (int i = 0; i < CUSTOMERS_COUNT; i++)
        pthread_join(customers[i], (void**)&(customers_status[i]));
    pthread_join(loader, (void**)&loader_status);
    
    return 0;
}
