#ifndef SHOP_H
#define SHOP_H

#include <stdlib.h>
#include <stdio.h>

#define SHOP_ST_BUSY 0
#define SHOP_ST_FREE 1

#define SHOPS_COUNT 5
#define SHOP_MAX_WEIGHT 10000

typedef struct
{
    int shop_weight;
    int status;
} Shop;

Shop chain_shop[SHOPS_COUNT];

#endif // SHOP_H