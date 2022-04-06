CFLAGS = -Wall -Wextra -Werror -std=c++11
CC = c++

_COLOR := \033[1;31m
_END := \033[0m

SRC_DIR ?= ../src
OBJ_DIR ?= ../build

UNAME = $(shell uname -s)

TEST_FLAGS ?=
TEST_OBJ ?=

SRCS = $(shell find $(SRC_DIR) -name *.cpp)
OBJS = $(SRCS:%=$(OBJ_DIR)/%.o)

TESTS = $(shell find . -name *.cpp)
BIN_DIR = ./bin
TEST_BINS = $(patsubst ./%.cpp, $(BIN_DIR)/%, $(TESTS))
ifeq ($(UNAME), Linux)
	CRITERION_DIR = $(dir $(shell find criterion* -type f -name "libcriterion.so"))
else
	CRITERION_DIR = $(dir $(shell find criterion* -type f -name "libcriterion.dylib"))
endif
CRITERION_INC = $(shell find $(CRITERION_DIR:%/lib/=%) -type d -name include)

all:	$(TEST_BINS)
	@for test in $(TEST_BINS); do \
		export "DYLD_LIBRARY_PATH=$(shell pwd)/$(CRITERION_DIR)" ; \
		export "LD_LIBRARY_PATH=$(shell pwd)/$(CRITERION_DIR)" ; \
		printf "[$(_COLOR)test$(_END)] running $$test\n" ; \
		./$$test || true; \
	done
	@$(MAKE) clean

$(BIN_DIR)/%: ./%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $< $(OBJS) $(addprefix ../,$(filter-out %main.cpp.o,$(TEST_OBJ))) -L$(CRITERION_DIR) -lcriterion -o $@ -I../src -I$(CRITERION_INC) $(TEST_FLAGS)

clean:
	@rm -rf $(BIN_DIR)
	@printf "[$(_COLOR)test$(_END)] clean done\n"

.PHONY: all clean
