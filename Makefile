CC:=gcc
CFLAGS:=-g

CUSTOMER_SRC := customer/customer.c
CUSTOMER_OBJ = $(CUSTOMER_SRC:.c=.o)

SHOP_SRC := shop/shop.c
SHOP_OBJ = $(SHOP_SRC:.c=.o)

LIFE_SRC := life.c
LIFE_OBJ = $(LIFE_SRC:.c=.o)

.PHONY: all

$(CUSTOMER_OBJ): $(CUSTOMER_SRC)
	$(CC) $^ $(CFLAGS) -c -o $@ -pthread

$(SHOP_OBJ): $(SHOP_SRC)
	$(CC) $^ $(CFLAGS) -c -o $@ -pthread

$(LIFE_OBJ): $(LIFE_SRC)
	$(CC) $^ $(CFLAGS) -c -o $@ -pthread

all: $(CUSTOMER_OBJ) $(SHOP_OBJ) $(LIFE_OBJ)

