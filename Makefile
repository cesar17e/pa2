# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

SRCS = outlier.c hashTable.c localHashLL.c wordProcessing.c
OBJS = $(SRCS:.c=.o)

TARGET = outlier

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
