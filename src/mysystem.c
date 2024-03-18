#include "mysystem.h"

struct Shop chain_shop[SHOPS_COUNT];
pthread_key_t key_customer_need;
pthread_once_t once_init_cust_need = PTHREAD_ONCE_INIT;
int customers_is_dead = 0;

void* loader_thread(){
    // Работать, пока хоть один покупатель жив
    while(1) {

        // Искать магазин
        for (int i = 0; i < SHOPS_COUNT; i++){
            // Магазин свободен
            if (pthread_mutex_lock(&chain_shop[i].mutex) == 0){

                printf("|_>>>_| Loader --> Shop[%3d][%7d] ", i, chain_shop[i].weight);
                chain_shop[i].weight += LOADER_WEIGHT;
                printf("==> Shop[%3d][%7d]\n", i, chain_shop[i].weight);

                pthread_mutex_unlock(&chain_shop[i].mutex);
                break;
            }
        }

        if (customers_is_dead)
            break;
        
        sleep(1);
    }

    return NULL;
}

void key_cust_need_free(void* value){
    free(value);
}

void key_cust_need_create(){
    pthread_key_create(&key_customer_need, key_cust_need_free);
}

void* customer_thread() {
    // Создать потребность
    pthread_once(&once_init_cust_need, key_cust_need_create);
    // Инициализировать потребность
    int* need = pthread_getspecific(key_customer_need);
    if (need == NULL){
        need = malloc(sizeof(int));
        *need = rand() % ((int)CUSTOMER_MAX_NEED);
    }

    while (*need > 0){
        // Искать магазин
        for (int i = 0; i < SHOPS_COUNT; i++){
            // Магазин свободен
            if (pthread_mutex_lock(&chain_shop[i].mutex) == 0){

                // Вывести старое состояние (* - ширина поля будет передана как аргумент)
                printf("Customer[%*lu][%*d] --> Shop[%*d][%*d] ", 
                    PRINT_TID_WIDTH, pthread_self(), 
                    PRINT_NEED_WIDTH, *need, 
                    SHOP_ID_WIDTH, i, 
                    PRINT_WEIGHT_WIDTH, chain_shop[i].weight);
                // Если 
                // Товара нет
                if (chain_shop[i].weight == 0){
                    printf("==> Customer[%*lu][%*d] --> Shop[%*d][%*d] EMPTY STORE\n", 
                        PRINT_TID_WIDTH, pthread_self(), 
                        PRINT_NEED_WIDTH, *need, 
                        SHOP_ID_WIDTH, i, 
                        PRINT_WEIGHT_WIDTH, chain_shop[i].weight);
                }
                // Товара больше, чем потребности - забрать равное потребности
                else if (*need < chain_shop[i].weight) {
                    *need = 0;
                    chain_shop[i].weight -= *need;
                    printf("==> Customer[%*lu][%*d] --> Shop[%*d][%*d] STOPPED\n", 
                        PRINT_TID_WIDTH, pthread_self(), 
                        PRINT_NEED_WIDTH, *need, 
                        SHOP_ID_WIDTH, i, 
                        PRINT_WEIGHT_WIDTH, chain_shop[i].weight);
                // Товара меньше, чем потребности - забрать всё что есть
                } else {
                    *need -= chain_shop[i].weight;
                    chain_shop[i].weight = 0; 
                    printf("==> Customer[%*lu][%*d] --> Shop[%*d][%*d]\n", 
                        PRINT_TID_WIDTH, pthread_self(), 
                        PRINT_NEED_WIDTH, *need, 
                        SHOP_ID_WIDTH, i, 
                        PRINT_WEIGHT_WIDTH, chain_shop[i].weight);
                }

                pthread_mutex_unlock(&chain_shop[i].mutex);
                break;
            }
        }
        sleep(2);
    }

    return NULL;
}