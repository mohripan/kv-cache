CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -g

kv-cache: main.c
	$(CC) $(CFLAGS) -o kv-cache main.c

clean:
	rm -f kv-cache
