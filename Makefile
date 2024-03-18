CC := gcc
CFLAGS := -g
LDFLAGS := -pthread

MYSYSTEM_SRC := mysystem.c
MYSYSTEM_OBJ := $(MYSYSTEM_SRC:.c=.o)

LIFE_SRC := life.c
LIFE_OBJ := $(LIFE_SRC:.c=.o)

.PHONY: all clean

all: life

$(MYSYSTEM_OBJ): $(MYSYSTEM_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIFE_OBJ): $(LIFE_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

life: $(LIFE_OBJ) $(MYSYSTEM_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o life