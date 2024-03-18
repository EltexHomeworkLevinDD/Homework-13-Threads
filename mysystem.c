#include "mysystem.h"

struct Shop chain_shop[SHOPS_COUNT];
pthread_key_t key_customer_need;
pthread_once_t once_init_cust_need = PTHREAD_ONCE_INIT;
pthread_mutex_t mutex_enter_shop = PTHREAD_MUTEX_INITIALIZER;
int* customers_status[CUSTOMERS_COUNT];

void* loader_thread(void* args){
    int id = -1;
    int dead_customers_count = 0;
    while (dead_customers_count < CUSTOMERS_COUNT) {
        dead_customers_count = 0;
        // Искать магазин
        pthread_mutex_lock(&mutex_enter_shop);
        for (int i = 0; i < SHOPS_COUNT; i++){
            //printf("---[%d] STATUS: %d\n", i, chain_shop[i].status);
            // Магазин свободен
            if (chain_shop[i].status == SHOP_ST_FREE){
                id = i;
                chain_shop[i].status = SHOP_ST_BUSY;
                break;
            }
        }
        pthread_mutex_unlock(&mutex_enter_shop);
        
        // Магазин найден, войти
        if (id > 0){
                //pthread_mutex_lock(&mutex_enter_shop);
                printf("|_>>>_| Loader --> Shop[%3d][%7d] ", id, chain_shop[id].shop_weight);
                chain_shop[id].shop_weight += LOADER_WEIGHT;
                printf("==> Shop[%3d][%7d]\n", id, chain_shop[id].shop_weight);

                chain_shop[id].status = SHOP_ST_FREE;
                //pthread_mutex_unlock(&mutex_enter_shop);
                id = -1;
                sleep(1);
            }
        }

        for (int i = 0; i < CUSTOMERS_COUNT; i++)
            if (customers_status[i] == NULL)
                dead_customers_count ++;
    

    return NULL;
}

void key_cust_need_free(void* value){
    free(value);
}

void key_cust_need_create(){
    pthread_key_create(&key_customer_need, key_cust_need_free);
}

void* customer_thread(void* args) {
    // Создать потребность
    pthread_once(&once_init_cust_need, key_cust_need_create);
    // Инициализировать потребность
    int* need = pthread_getspecific(key_customer_need);
    if (need == NULL){
        need = malloc(sizeof(int));
        *need = rand() % ((int)CUSTOMER_MAX_NEED);
    }

    int id = -1;
    // Постоянное действие
    while (*need > 0){
        // Искать магазин
        pthread_mutex_lock(&mutex_enter_shop);
        for (int i = 0; i < SHOPS_COUNT; i++){

            //printf("---[%d] STATUS: %d\n", i, chain_shop[i].status);
            // Магазин свободен
            if (chain_shop[i].status == SHOP_ST_FREE){
                id = i;
                chain_shop[id].status = SHOP_ST_BUSY;
                break;
            }
        }
        pthread_mutex_unlock(&mutex_enter_shop);
        
        // Магазин найден, войти
        if (id > 0){
            //pthread_mutex_unlock(&mutex_enter_shop);
            printf("Customer[%10u][%7d] --> Shop[%3d][%7d] ", (unsigned)pthread_self(), *need, id, chain_shop[id].shop_weight);
            // Товара больше, чем потребности - забрать равное потребности
            if (chain_shop[id].shop_weight == 0){
                printf("==> Customer[%10u][%7d] --> Shop[%3d][%7d] EMPTY STORE\n", (unsigned)pthread_self(), *need, id, chain_shop[id].shop_weight);
            }
            else if (*need < chain_shop[id].shop_weight) {
                *need = 0;
                chain_shop[id].shop_weight -= *need;
                printf("==> Customer[%10u][%7d] --> Shop[%3d][%7d] STOPPED\n", (unsigned)pthread_self(), *need, id, chain_shop[id].shop_weight);
            // Товара меньше, чем потребности - забрать всё что есть
            } else {
                *need -= chain_shop[id].shop_weight;
                chain_shop[id].shop_weight = 0; 
                printf("==> Customer[%10u][%7d] --> Shop[%3d][%7d]\n", (unsigned)pthread_self(), *need, id, chain_shop[id].shop_weight);
            }
            chain_shop[id].status = SHOP_ST_FREE;
            id = -1;
            //pthread_mutex_unlock(&mutex_enter_shop);
            sleep(2);
        }

    }

    return NULL;
}

// #include "mysystem.h"

// struct Shop chain_shop[SHOPS_COUNT];
// pthread_key_t key_customer_need;
// pthread_once_t once_init_cust_need = PTHREAD_ONCE_INIT;
// pthread_mutex_t mutex_enter_shop = PTHREAD_MUTEX_INITIALIZER;
// int* customers_status[CUSTOMERS_COUNT];

// void* loader_thread(void* args){
//     int shop_id = -1;
//     int shop_not_found = 1;
//     int exitflag = 0;
//     while (1)
//     {
//         pthread_mutex_lock(&mutex_enter_shop);  
//         while (shop_not_found){
//             shop_id++;
//             if (shop_id >= SHOPS_COUNT)
//                 shop_id = 0;
//             if (chain_shop[shop_id].status == SHOP_ST_FREE){
//                 chain_shop[shop_id].status = SHOP_ST_BUSY;
//                 printf("  Loader[%10u] \t\t --> Shop[%3d][%10d] ", (unsigned)pthread_self(), shop_id, chain_shop[shop_id].shop_weight);
//                 chain_shop[shop_id].shop_weight += LOADER_WEIGHT;
//                 printf("==> \t\t\t\t\t    Shop[%3d][%10d]\n", shop_id, chain_shop[shop_id].shop_weight);
//                 chain_shop[shop_id].status = SHOP_ST_FREE;

//                 pthread_mutex_unlock(&mutex_enter_shop);
//                 sleep(1);
//             }

//             for (int i = 0; i < CUSTOMERS_COUNT; i++){
//                 if (customers_status[i] == NULL) {
//                     exitflag ++;
//                 }
//             }
//             if (exitflag == CUSTOMERS_COUNT){
//                 break;
//             }else{
//                 exitflag = 0;
//             }
            
//         }
//     }

//     return NULL;
// }

// void key_cust_need_free(void* value){
//     free(value);
// }

// void key_cust_need_create(){
//     pthread_key_create(&key_customer_need, key_cust_need_free);
// }

// void* customer_thread(void* args) {
//     // Создать потребность
//     pthread_once(&once_init_cust_need, key_cust_need_create);
//     // Инициализировать потребность
//     int* need = pthread_getspecific(key_customer_need);
//     if (need == NULL){
//         need = malloc(sizeof(int));
//         *need = rand() % ((int)CUSTOMER_MAX_NEED);
//     }
//     int exitflag = 0;

//     while (*need > 0) {
//         // Искать магазин
//         for (int i = 0; i < SHOPS_COUNT; i++){

//             if (chain_shop[i].status == SHOP_ST_FREE) {
//                 pthread_mutex_lock(&mutex_enter_shop);

//                 if (chain_shop[i].status == SHOP_ST_FREE) {
//                     chain_shop[i].status = SHOP_ST_BUSY;
//                     if (chain_shop[i].shop_weight > 0) {
//                         printf("Customer[%10u][%10d] --> Shop[%3d][%10d] ", (unsigned)pthread_self(), *need, i, chain_shop[i].shop_weight);
//                         if (*need < chain_shop[i].shop_weight) {
//                             *need = 0;
//                             chain_shop[i].shop_weight -= *need;
//                             printf("==> Customer[%10u][%10d] --> Shop[%3d][%10d] STOPPED\n", (unsigned)pthread_self(), *need, i, chain_shop[i].shop_weight);
//                             break;
//                         } else {
//                             *need -= chain_shop[i].shop_weight;
//                             chain_shop[i].shop_weight = 0; 
//                             printf("==> Customer[%10u][%10d] --> Shop[%3d][%10d]\n", (unsigned)pthread_self(), *need, i, chain_shop[i].shop_weight);
//                         }
//                     }
//                     chain_shop[i].status = SHOP_ST_FREE;
//                 }

//                 pthread_mutex_unlock(&mutex_enter_shop);
//                 sleep(2);
//             }
//         }
        
//     }


    
//     return NULL;
// }