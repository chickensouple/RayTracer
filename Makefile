
export MAKEFLAGS=--no-print-directory

CXX_SOURCES := $(wildcard src/*.cpp)
# CXX_SOURCES := $(filter-out RayTracer.cpp, $(CXX_SOURCES))

# directory variables
ROOT_DIR = $(shell pwd)
BUILD_DIR := $(ROOT_DIR)/build
OUTPUT_DIR := $(ROOT_DIR)/bin

# compiler
CXX := g++

# compiler flags
COMMON_FLAGS := -std=c++11 -Wall -Wno-unused-parameter
CXX_FLAGS := $(COMMON_FLAGS) -O3
CXX_DEBUG_FLAGS := $(COMMON_FLAGS) -g

# library linker flags
LD_FLAGS := -L. -ldl `pkg-config --libs opencv` `pkg-config --libs eigen3`

# include flags
INCLUDES := -I. `pkg-config --cflags opencv` `pkg-config --cflags eigen3`

# adding prefix to objects, so that they will be put in correct directory
OBJECT_PATH := $(subst $(ROOT_DIR), $(BUILD_DIR), $(shell pwd))
CXX_OBJECTS := $(addprefix $(OBJECT_PATH)/Release/, $(CXX_SOURCES:.cpp=.o))
CXX_DEBUG_OBJECTS := $(addprefix $(OBJECT_PATH)/Debug/, $(CXX_SOURCES:.cpp=.o))

EXECUTABLE := $(OUTPUT_DIR)/main.out
TEST_EXECUTABLE := $(OUTPUT_DIR)/debug_main.out

all: $(CXX_OBJECTS) | directories
	@$(CXX) $(CXX_FLAGS) $^ $(LD_FLAGS) $(INCLUDES) -o $(EXECUTABLE)
	@echo "Built $@"

debug: $(CXX_DEBUG_OBJECTS) | directories
	@$(CXX) $(CXX_DEBUG_FLAGS) $^ $(LD_FLAGS) $(INCLUDES) -o $(TEST_EXECUTABLE)
	@echo "Built $@"

run:
	bin/main.out

rundebug:
	ddd bin/debug_main.out

# dependency files
DEP_FILES := $(shell find $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST)))) -name '*.d')
-include $(DEP_FILES)

$(OBJECT_PATH)/Release/%.o: %.cpp
# mkdir will recreate the file structure of the original
	@mkdir -p $(dir $@)
# taking the output of dependency generation, and formatting it correctly
	@$(CXX) -MM $(CXX_FLAGS) $(INCLUDES) $< | \
		perl -pe 's/([a-zA-Z0-9_\/-]*)\.((?!o)[a-zA-Z]*)/$$1.$$2/g' | \
		perl -pe 's/([a-zA-Z0-9_\/-]*)\.o/$(subst /,\/,$(dir $@))$$1.o/g' > $@.d
	@echo "Object [$@]"
	@$(CXX) $(CXX_FLAGS) $(INCLUDES) -S -o $(addsuffix .s, $(basename $@)) $<
	@$(CXX) $(CXX_FLAGS) $(INCLUDES) -c -o $@ $(addsuffix .s, $(basename $@))

$(OBJECT_PATH)/Debug/%.o: %.cpp
# mkdir will recreate the file structure of the original
	@mkdir -p $(dir $@)
# taking the output of dependency generation, and formatting it correctly
	@$(CXX) -MM $(CXX_DEBUG_FLAGS) $(INCLUDES) $< | \
		perl -pe 's/([a-zA-Z0-9_\/-]*)\.((?!o)[a-zA-Z]*)/$$1.$$2/g' | \
		perl -pe 's/([a-zA-Z0-9_\/-]*)\.o/$(subst /,\/,$(dir $@))$$1.o/g' > $@.d
	@echo "Object [$@]"
	@$(CXX) $(CXX_DEBUG_FLAGS) $(INCLUDES) -S -o $(addsuffix .s, $(basename $@)) $<
	@$(CXX) $(CXX_DEBUG_FLAGS) $(INCLUDES) -c -o $@ $(addsuffix .s, $(basename $@))

clean: 
	@rm -f -r $(BUILD_DIR)
	@rm -f -r $(OUTPUT_DIR)

cleandep:
	@find . -name "*.d" -type f -delete

directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OUTPUT_DIR)

.PHONY: all clean cleandep directories upload

.PRECIOUS: $(OBJECT_PATH)/Release/%.o $(OBJECT_PATH)/Debug/%.o
