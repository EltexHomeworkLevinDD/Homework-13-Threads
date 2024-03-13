#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "shop/shop.h"
#include "customer/customer.h"

#define SHOPS_COUNT 5
#define SHOP_MAX_WEIGHT 10000

int main(void)
{
    memset(&chain_shop, '\0', sizeof(chain_shop));

    for (int i = 0; i < SHOPS_COUNT; i++){
        chain_shop[i].shop_weight = rand() % SHOP_MAX_WEIGHT;
        chain_shop[i].status = SHOP_ST_FREE;
    }


    
    return 0;
}