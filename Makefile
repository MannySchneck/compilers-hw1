CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
CC_FLAGS := --std=c++11 -I./src -I./PEGTL -g3
LD_FLAGS :=
CC := g++

all: dirs L1

dirs:
	mkdir -p obj ; mkdir -p bin ;

L1: $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o ./bin/$@ $^

obj/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

test: L1
	./scripts/test.sh

clean:
	rm -f bin/L1 obj/*.o *.out *.o *.S core.* tests/*.tmp
