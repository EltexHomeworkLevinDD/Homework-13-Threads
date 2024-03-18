#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include "mysystem.h"

/*
Симуляция жизни
При запуске сформировать массив из пяти int чисел и случайно заполнять их в пределах 10 000
Каждое число - магазин
Затем главная программа порождает три потока покупателя. При создании покупателя ему назначается 
случайная потребность в пределах 100 000. Покупатель пытается зайти в первый свободный магазин 
(в каждом магазине может быть только один покупатель).
Покупатель забирает из магазина Все доступные товары, уменьшая свою потребность. Засыпает на 2 секунды и ищет свободный магазин.

Как только он достигает потребности ноль, поток завершает свою работу.
Есть четвёртый поток погрузчик он заходит в свободные магазины и добавляют туда по 5.000  продуктов.  Засыпает на секунду и ищет следующий магазин.
Каждый поток выводит информацию о себе и своих действиях

Когда все покупатели насытились то погрузчик завершается работу и программа завершается

*/

int main()
{
    
    for (int i = 0; i < SHOPS_COUNT; i++){
        chain_shop[i].weight = rand() % SHOP_MAX_WEIGHT;
        // Инициализация мьютекса (у & приоритет выше, чем у [])
        if (pthread_mutex_init(&chain_shop[i].mutex, NULL) != 0) {
            perror("Mutex init error");
            exit(EXIT_FAILURE);
        }
        printf("Shop[%3d][%5d]\n", i, chain_shop[i].weight);
    }

    pthread_t loader;
    int* loader_status;
    int* customer_status;
    pthread_create(&loader, NULL, loader_thread, NULL);

    pthread_t customers[CUSTOMERS_COUNT];

    for (int i = 0; i < CUSTOMERS_COUNT; i++)
        if (pthread_create(&customers[i], NULL, customer_thread, NULL) != 0){
            perror("Customer thread create error");
            exit(EXIT_FAILURE);
        }
    
    for (int i = 0; i < CUSTOMERS_COUNT; i++){
        pthread_join(customers[i], (void**)&customer_status);
    }
    customers_is_dead = 1;
    pthread_join(loader, (void**)&loader_status);
    
    return 0;
}
