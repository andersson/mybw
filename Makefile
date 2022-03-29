OUT := mybw

CFLAGS := -O2 -Wall -fno-builtin
LDFLAGS := -static -static-libgcc

SRCS := mybw.c
OBJS := $(SRCS:.c=.o)

$(OUT): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OUT) $(OBJS)
