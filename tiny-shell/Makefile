TARGET := tinyshell
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRCS))

CC := gcc
CFLAGS := -Wall 
LDFLAGS :=
LDLIBS := -lreadline

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)


.PHONY: run debug memcheck run

run: $(TARGET)
	@./$(TARGET)

debug: CFLAGS += -g -O0
debug: clean $(TARGET)
	@gdb -tui ./$(TARGET)

memcheck: $(TARGET)
	@valgrind --tool=memcheck \
					  --leak-check=full \
					  --show-leak-kinds=all \
					  ./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)