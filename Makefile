SRC_DIRS :=
DEPDIR := .d


# TODO: Somehow use this to simplify...
# TODO: Refactor with minnet makefile as model
# FIND_SRCS := $(shell find src -iname *.cpp)
# $(info Found srcs  $(FIND_SRC))

L1_SRC_DIR := src/L1
L1_AST_DIR := src/L1/AST
L1_OBJ_DIR := obj/

SRC_DIRS += $(L1_SRC_DIR)
SRC_DIRS += $(L1_AST_DIR)

L1_AST_CPP := $(wildcard $(L1_AST_DIR)/*.cpp)
L1_AST_OBJ := $(addprefix $(L1_OBJ_DIR),$(L1_AST_CPP:src/%.cpp=%.o))
L1_AST_HDR:= $(L1_AST_CPP:%.cpp=%.h)

L1_CPP_FILES = $(wildcard $(L1_SRC_DIR)/*.cpp)
L1_MAIN_CPP := $(wildcard $(L1_SRC_DIR)/main/*.cpp)
L1_HDR_FILES := $(L1_CPP_FILES:%.cpp=%.h)

L1_MAIN_OBJ := $(addprefix $(L1_OBJ_DIR),$(L1_MAIN_CPP:src/%.cpp=%.o))
L1_OBJ_FILES := $(addprefix $(L1_OBJ_DIR),$(L1_CPP_FILES:src/%.cpp=%.o))
#L2
L2_SRC_DIR := src/L2
L2_AST_DIR := src/L2/AST
L2_REG_DIR := src/L2/reg_allocation
L2_OBJ_DIR := obj/

SRC_DIRS += $(L2_SRC_DIR)
SRC_DIRS += $(L2_AST_DIR)
SRC_DIRS += $(L2_REG_DIR)

L2_AST_CPP := $(wildcard $(L2_AST_DIR)/*.cpp)
L2_AST_OBJ := $(addprefix $(L2_OBJ_DIR),$(L2_AST_CPP:src/%.cpp=%.o))

L2_REG_CPP := $(wildcard $(L2_REG_DIR)/*.cpp)
L2_REG_OBJ := $(addprefix $(L2_OBJ_DIR),$(L2_REG_CPP:src/%.cpp=%.o))

L2_CPP_FILES := $(wildcard $(L2_SRC_DIR)/*.cpp)
L2_MAIN_CPP := $(wildcard $(L2_SRC_DIR)/main/compiler.cpp)
L2_LIVENESS_CPP := $(wildcard $(L2_SRC_DIR)/main/liveness.cpp)

L2_LIVENESS_OBJ := $(addprefix $(L2_OBJ_DIR),$(L2_LIVENESS_CPP:src/%.cpp=%.o))
L2_MAIN_OBJ := $(addprefix $(L2_OBJ_DIR),$(L2_MAIN_CPP:src/%.cpp=%.o))
L2_OBJ_FILES := $(addprefix $(L2_OBJ_DIR),$(L2_CPP_FILES:src/%.cpp=%.o))
#tests
UNIT_TEST_SRC_DIR := src/unit_tests
UNIT_TEST_CPP_FILES := $(wildcard $(UNIT_TEST_SRC_DIR)/*.cpp)
UNIT_TEST_MAIN := $(UNIT_TEST_SRC_DIR)/main/catch_main.cpp

SRC_DIRS += $(UNIT_TEST_SRC_DIR) $(L2_SRC_DIR)/main $(L1_SRC_DIR)/main

OBJ_DIRS := $(SRC_DIRS:src/%=obj/%)
DEP_DIRS := $(SRC_DIRS:src/%=$(DEPDIR)/%)
DIRS := $(OBJ_DIRS) $(DEP_DIRS)

SRCS := $(UNIT_TEST_CPP_FILES) $(L2_AST_CPP) $(L2_REG_CPP) $(L2_CPP_FILES) $(L2_MAIN_CPP) $(L1_AST_CPP) $(L1_CPP_FILES) $(L1_MAIN_CPP)

UNIT_TEST_OBJ := $(UNIT_TEST_CPP_FILES:src/%.cpp=obj/%.o)
UNIT_TEST_MAIN_OBJ:= $(UNIT_TEST_MAIN:.cpp=.o)

CXX_FLAGS := --std=c++1z -O0 -I./$(L1_SRC_DIR) -I./src -I./src/L2 -I./lib/PEGTL -I./lib/boost_1_64_0 -g3 -Wno-write-strings
LD_FLAGS :=
CXX := g++

CXX_COMPILE := $(CXX) $(CXX_FLAGS) $(LD_FLAGS)

$(shell mkdir -p $(DIRS) lib;)

all: L2clang

.PHONY: clean dirs

clean:
	rm -rf bin/ obj/ *.out *.o *.S core.* tests/L1/*.tmp tests/liveness/*.tmp
	mkdir -p obj/L1/AST; mkdir -p obj/L2/AST ; mkdir -p bin; mkdir -p lib;

SRCS := $(UNIT_TEST_CPP_FILES) $(L2_AST_CPP) $(L2_CPP_FILES) $(L2_MAIN_CPP) $(L1_AST_CPP) $(L1_CPP_FILES) $(L1_MAIN_CPP)

################################################################################
# unit tests
################################################################################
integration_test: L1_test liveness_test

unit_test: $(L1_OBJ_FILES) $(L1_AST_OBJ) $(L2_AST_OBJ) $(L2_REG_OBJ) $(L2_OBJ_FILES)  $(UNIT_TEST_OBJ) $(UNIT_TEST_MAIN_OBJ)
	 $(CXX_COMPILE) -o bin/$@ $^
	./bin/unit_test

$(UNIT_TEST_MAIN_OBJ) : $(UNIT_TEST_MAIN)
	$(CXX_COMPILE) -c -o $@ $<
################################################################################
# L1
################################################################################
L1: $(L1_OBJ_FILES) $(L1_MAIN_OBJ) $(L1_AST_OBJ)
	$(CXX_COMPILE)  $(L1_AST_OBJ) $(L1_OBJ_FILES) $(L1_MAIN_OBJ)  -o ./bin/$@


L1_test: L1
	./scripts/L1_test.sh

################################################################################
# L2
################################################################################
L2_Liveness: $(L2_OBJ_FILES) $(L2_LIVENESS_OBJ) $(L2_AST_OBJ) $(L2_REG_OBJ)
	$(CXX_COMPILE) -o ./bin/$@ $^

L2: $(L2_OBJ_FILES) $(L2_MAIN_OBJ) $(L2_AST_OBJ) $(L2_REG_OBJ) 
	$(CXX_COMPILE) -o ./bin/$@ $^


test: L2_test

liveness_test: L2_Liveness
	./scripts/test_liveness.sh

L2_test: L2
	./scripts/L2_test.sh

################################################################################
# Dependencies
################################################################################

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXX_FLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

obj/%.o : src/%.cpp
obj/%.o : src/%.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst src/%,$(DEPDIR)/%.d,$(basename $(SRCS))))
