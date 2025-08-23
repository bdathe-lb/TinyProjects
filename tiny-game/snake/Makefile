TARGET := snake
SRCS := $(wildcard *.cc)
OBJS := $(patsubst %.cc, %.o, $(SRCS))
DEPS := $(patsubst %.cc, %.d, $(SRCS))

CXX := g++ 
CXXFLAGS := -Wall -std=c++11
DEPFLAGS := -MMD -MP
LDFLAGS := 
LDLIBS := -lncurses

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $(TARGET) $(LDFLAGS) $(LDLIBS)

%.o: %.cc
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(DEPFLAGS)


-include $(DEPFLAGS)

.PHONY: clean run debug memcheck

run: $(TARGET)
	./$(TARGET)

debug: CXXFLAGS += -g -O0
debug: clean $(TARGET)
	@gdb -tui ./$(TARGET)

memcheck: $(TARGET)
	@valgrind --tool=memcheck \
						--leak-check=full \
						--show-leak-kinds=all \
						./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)

