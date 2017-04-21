L1_SRC_DIR := src/L1
L1_AST_DIR := src/L1/AST
L1_OBJ_DIR := obj/

L1_AST_CPP := $(wildcard $(L1_AST_DIR)/*.cpp)
L1_AST_OBJ := $(addprefix $(L1_OBJ_DIR),$(L1_AST_CPP:src/%.cpp=%.o))
L1_AST_HDR:= $(L1_AST_CPP:%.cpp=%.h)

L1_CPP_FILES = $(wildcard $(L1_SRC_DIR)/*.cpp)
L1_MAIN_CPP := $(wildcard $(L1_SRC_DIR)/main/*.cpp)
L1_HDR_FILES := $(L1_CPP_FILES:%.cpp=%.h)

L1_MAIN_OBJ := $(addprefix $(L1_OBJ_DIR),$(notdir $(L1_MAIN_CPP:.cpp=.o)))
L1_OBJ_FILES := $(addprefix $(L1_OBJ_DIR),$(L1_CPP_FILES:src/%.cpp=%.o))
#L2
L2_SRC_DIR := src/L2
L2_AST_DIR := src/L2/AST
L2_OBJ_DIR := obj/

L2_AST_CPP := $(wildcard $(L2_AST_DIR)/*.cpp)
L2_AST_OBJ := $(addprefix $(L2_OBJ_DIR),$(L2_AST_CPP:src/%.cpp=%.o))
L2_AST_HDR:= $(L2_AST_CPP:%.cpp=%.h)

L2_CPP_FILES := $(wildcard $(L2_SRC_DIR)/*.cpp)
L2_MAIN_CPP := $(wildcard $(L2_SRC_DIR)/main/*.cpp)
L2_HDR_FILES := $(L2_CPP_FILES:%.cpp=%.h)

L2_MAIN_OBJ := $(addprefix $(L2_OBJ_DIR),$(notdir $(L2_MAIN_CPP:.cpp=.o)))
L2_OBJ_FILES := $(addprefix $(L2_OBJ_DIR),$(L2_CPP_FILES:src/%.cpp=%.o))
#tests
UNIT_TEST_SRC_DIR := src/unit_tests
UNIT_TEST_CPP_FILES := $(wildcard $(UNIT_TEST_SRC_DIR)/*.cpp)
UNIT_TEST_MAIN := $(UNIT_TEST_SRC_DIR)/main/catch_main.cpp

UNIT_TEST_OBJ := $(addprefix obj/,$(notdir $(UNIT_TEST_CPP_FILES:.cpp=.o)))
UNIT_TEST_MAIN_OBJ:= $(addprefix obj/,$(notdir $(UNIT_TEST_MAIN:.cpp=.o)))

CC_FLAGS := --std=c++11 -I./$(L1_SRC_DIR) -I./src -I./src/L2 -I./PEGTL  -g3 -Wno-write-strings
LD_FLAGS :=
CXX := g++

CXX_COMPILE := $(CXX) $(CC_FLAGS) $(LD_FLAGS)


all: dirs L2

.PHONY: clean dirs

dirs:
	mkdir -p obj/L1/AST; mkdir -p obj/L2/AST ; mkdir -p bin; mkdir -p lib;

clean:
	rm -rf bin/ obj/ *.out *.o *.S core.* tests/L1/*.tmp tests/liveness/*.tmp
	mkdir -p obj/L1/AST; mkdir -p obj/L2/AST ; mkdir -p bin; mkdir -p lib;

################################################################################
# unit tests
################################################################################
unit_test: $(L1_OBJ_FILES) $(L1_AST_OBJ) $(L2_OBJ_FILES) $(L2_AST_OBJ) $(UNIT_TEST_OBJ) $(UNIT_TEST_MAIN_OBJ)
	 $(CXX_COMPILE) -o bin/$@ $^
	./bin/unit_test

$(UNIT_TEST_OBJ): obj/%.o: $(UNIT_TEST_SRC_DIR)/%.cpp 
	$(CXX_COMPILE) -c -o $@ $<

$(UNIT_TEST_MAIN_OBJ) : $(UNIT_TEST_MAIN) 
	$(CXX_COMPILE) -c -o $@ $<
################################################################################
# L1
################################################################################
L1: $(L1_OBJ_FILES) $(L1_MAIN_OBJ) $(L1_AST_OBJ)
	$(CXX_COMPILE)  $(L1_AST_OBJ) $(L1_OBJ_FILES) $(L1_MAIN_OBJ)  -o ./bin/$@ 

$(L1_MAIN_OBJ): $(L1_MAIN_CPP)
	$(CXX_COMPILE) -c -o $@ $^

$(L1_OBJ_FILES) : obj/%.o: src/%.cpp 
	$(CXX_COMPILE) -c -o $@ $<



$(L1_AST_OBJ) : obj/%.o: src/%.cpp 
	$(CXX_COMPILE) -c -o $@ $<

$(L1_AST_CPP) : %.cpp:%.h
	touch $@

$(L1_CPP_FILES) : %.cpp:%.h
	touch $@


L1_test: L1
	./scripts/L1_test.sh

################################################################################
# L2
################################################################################
L2: $(L2_OBJ_FILES) $(L2_MAIN_OBJ) $(L2_AST_OBJ)
	$(CXX_COMPILE) -o ./bin/$@ $^

$(L2_MAIN_OBJ): $(L2_MAIN_CPP) $(L2_AST_OBJ)
	$(CXX_COMPILE) -c -o $@ $^

$(L2_OBJ_FILES) : obj/%.o: src/%.cpp
	$(CXX_COMPILE) -c -o $@ $<

$(L2_AST_OBJ) : obj/%.o: src/%.cpp 
	$(CXX_COMPILE) -c -o $@ $<

$(L2_AST_CPP) : %.cpp:%.h
	touch $@

$(L2_CPP_FILES) : %.cpp:%.h
	touch $@


L2_test: L2
	./scripts/test.sh


