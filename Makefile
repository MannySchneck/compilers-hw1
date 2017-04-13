CPP_FILES := $(wildcard src/*.cpp)

L1_MAIN_CPP := $(wildcard src/main/*.cpp)
L1_MAIN_OBJ := $(addprefix obj/,$(notdir $(L1_MAIN_CPP:.cpp=.o)))

UNIT_TEST_CPP_FILES := $(wildcard src/unit_tests/*.cpp)
UNIT_TEST_OBJ := $(addprefix obj/,$(notdir $(UNIT_TEST_CPP_FILES:.cpp=.o)))

OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
CC_FLAGS := --std=c++11 -I./src -I./PEGTL -g3 -Wno-write-strings
LD_FLAGS :=
CC := g++

all: dirs L1

dirs:
	mkdir -p obj ; mkdir -p bin ;

unit_test: $(OBJ_FILES) $(UNIT_TEST_OBJ)
	$(info $$UNIT_TEST_OBJ is [${OBJ_FILES}]) $(CC) $(LD_FLAGS) -o ./bin/$@ $^
	./bin/unit_test

obj/%.o:  src/unit_tests/%.cpp
	$(CC) $(LD_FLAGS) -c -o $@ $<

L1: $(OBJ_FILES) $(L1_MAIN_OBJ)
	$(CC) $(LD_FLAGS) -o ./bin/$@ $^

obj/compiler.o: src/main/compiler.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

obj/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

test: L1
	./scripts/test.sh

clean:
	rm -f bin/L1 obj/*.o *.out *.o *.S core.* tests/*.tmp
