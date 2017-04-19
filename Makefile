L1_SRC_DIR := src/L1
L1_OBJ_DIR := obj/L1/

L1_CPP_FILES := $(wildcard $(L1_SRC_DIR)/*.cpp)
L1_MAIN_CPP := $(wildcard $(L1_SRC_DIR)/main/*.cpp)

L1_MAIN_OBJ := $(addprefix $(L1_OBJ_DIR),$(notdir $(L1_MAIN_CPP:.cpp=.o)))
L1_OBJ_FILES := $(addprefix $(L1_OBJ_DIR),$(notdir $(L1_CPP_FILES:.cpp=.o)))
#L2
L2_SRC_DIR := src/L2
L2_OBJ_DIR := obj/L2/

L2_CPP_FILES := $(wildcard $(L2_SRC_DIR)/*.cpp)
L2_MAIN_CPP := $(wildcard $(L2_SRC_DIR)/main/*.cpp)

L2_MAIN_OBJ := $(addprefix $(L2_OBJ_DIR),$(notdir $(L2_MAIN_CPP:.cpp=.o)))
L2_OBJ_FILES := $(addprefix $(L2_OBJ_DIR),$(notdir $(L2_CPP_FILES:.cpp=.o)))
#tests
UNIT_TEST_SRC_DIR := src/unit_tests
UNIT_TEST_CPP_FILES := $(wildcard $(UNIT_TEST_SRC_DIR)/*.cpp)
UNIT_TEST_MAIN := $(UNIT_TEST_SRC_DIR)/main/catch_main.cpp

UNIT_TEST_OBJ := $(addprefix obj/,$(notdir $(UNIT_TEST_CPP_FILES:.cpp=.o)))
UNIT_TEST_MAIN_OBJ:= $(addprefix obj/,$(notdir $(UNIT_TEST_MAIN:.cpp=.o)))

CC_FLAGS := --std=c++11 -I./$(L1_SRC_DIR) -I./src -I./src/L2 -I./PEGTL -g3 -Wno-write-strings
LD_FLAGS :=
CXX := g++

CXX_COMPILE := $(CXX) $(CC_FLAGS) $(LD_FLAGS)


all: dirs L2

dirs:
	mkdir -p obj/L1; mkdir -p obj/L2 ; mkdir -p bin;

################################################################################
# unit tests
################################################################################
unit_test: $(L1_OBJ_FILES) $(UNIT_TEST_OBJ) $(UNIT_TEST_MAIN_OBJ)
	 $(CXX_COMPILE) -o bin/$@ $^
	./bin/unit_test

$(UNIT_TEST_OBJ): obj/%.o: $(UNIT_TEST_SRC_DIR)/%.cpp dirs
	$(CXX_COMPILE) -c -o $@ $<

$(UNIT_TEST_MAIN_OBJ) : $(UNIT_TEST_MAIN) dirs
	$(CXX_COMPILE) -c -o $@ $<
################################################################################
# L1
################################################################################
L1: $(L1_OBJ_FILES) $(L1_MAIN_OBJ)
	$(CXX_COMPILE) -o ./bin/$@ $^

$(L1_MAIN_OBJ): $(L1_MAIN_CPP)
	$(CXX_COMPILE) -c -o $@ $^

$(L1_OBJ_FILES) : obj/%.o: src/%.cpp dirs
	$(CXX_COMPILE) -c -o $@ $<

L1_test: L1
	./scripts/L1_test.sh

clean:
	rm -rf bin/ obj/ *.out *.o *.S core.* tests/*.tmp
################################################################################
# L2
################################################################################
L2: $(L2_OBJ_FILES) $(L2_MAIN_OBJ)
	$(CXX_COMPILE) -o ./bin/$@ $^

$(L2_MAIN_OBJ): $(L2_MAIN_CPP)
	$(CXX_COMPILE) -c -o $@ $^

$(L2_OBJ_FILES) : obj/%.o: src/%.cpp
	$(CXX_COMPILE) -c -o $@ $<

L2_test: L2
	./scripts/test.sh


