TARGET := game
SRCS := $(shell find src -type f -name "*.cc")
OBJS := $(patsubst src/%.cc, build/%.o, $(SRCS))
DEPS := $(patsubst src/%.cc, build/%.d, $(SRCS))

CXX := g++
INCLUDE := \
	-Isrc/engine \
	-Isrc/engine/interface \
	-Isrc/engine/implement \
	-Isrc/games/snake
CXXFLAGS := -std=c++17 -Wall -Werror $(INCLUDE)
DEPFLAGS := -MMD -MP
LDLIBS := -lncurses

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDLIBS)

build/%.o: src/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

-include $(DEPS)

.PHONY: run debug clean

run: CXXFLAGS += -O2
run: clean $(TARGET)
	@./$(TARGET)

debug: CXXFLAGS += -g -O0
debug: clean $(TARGET)
	@gdb -tui ./$(TARGET)

clean:
	rm -rf build/ $(TARGET)