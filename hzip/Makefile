TARGET := hzip
EXCLUDE := 
SRCS := $(filter-out $(EXCLUDE), $(wildcard *.c))
OBJS := $(patsubst %.c, %.o, $(SRCS))
DEPS := $(patsubst %.c, %.d, $(SRCS))

CC := gcc
CFLAGS := -Wall
DEPFLAGS := -MMD -MP

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(DEPFLAGS)

-include $(DEPS)

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)
