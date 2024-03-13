#include "customer.h"
extern SHOPS_COUNT;
extern SHOP_ST_BUSY ;
extern SHOP_ST_FREE;

#define SHOPS_COUNT SHOPS_COUNT
#define SHOP_ST_BUSY SHOP_ST_BUSY
#define SHOP_ST_FREE SHOP_ST_FREE

void key_cust_need_create(){
    pthread_key_create(&key_customer_need, NULL);
}

void* customer_thread(void* args) {
    // Создать потребность
    pthread_once(&once_init_cust_need, key_cust_need_create);
    // Инициализировать потребность
    int* need = pthread_getspecific(key_customer_need);
    if (need == NULL){
        *need = rand() % CUST_MAX_NEED;
    }

    // 
    for (int i = 0; i < SHOPS_COUNT; i++){
        if (chain_shop[i].status == SHOP_ST_FREE)
        {
            pthread_mutex_lock(&mutex_enter_shop);

            if (chain_shop[i].status == SHOP_ST_FREE) {
                printf("Customer[%10ld][%10d] --> Shop[%10d][%10d] ", (long int)pthread_self(), *need, i, chain_shop[i].shop_weight);

                chain_shop[i].status = SHOP_ST_BUSY;
                if (chain_shop[i].shop_weight > 0){
                    if (*need < chain_shop[i].shop_weight) {
                        *need = 0;
                        chain_shop[i].shop_weight -= *need;
                    } else {
                        *need -= chain_shop[i].shop_weight;
                        chain_shop[i].shop_weight = 0; 
                    }
                }
                printf("==> Customer[%10ld][%10d] --> Shop[%10d][%10d]\n", (long int)pthread_self(), *need, i, chain_shop[i].shop_weight);
                chain_shop[i].status = SHOP_ST_FREE;
                break;
            }

            pthread_mutex_unlock(&mutex_enter_shop);
        }
    }
    
    return NULL;
}