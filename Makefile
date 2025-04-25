ifeq ($(origin CC),default)
  CC = g++
endif


CFLAGS ?=
LDFLAGS =

BUILD_TYPE ?= debug
# BUILD_TYPE ?= release

ifeq ($(BUILD_TYPE), release)
	CFLAGS += -D _NDEBUG -O3
endif 

ifeq ($(BUILD_TYPE), debug)
	CFLAGS += -g -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ 								          \
			  -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations 					           \
			  -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported 							        \
			  -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral 			             \
			  -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op 					              \
			  -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith 				           \
			  -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel              \
			  -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types             \
			  -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused           \
			  -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing          \
			  -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector         \
			  -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192         \
			  -pie -fPIE -Werror=vla 																		                  \
			  -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr \


	LDFLAGS += -fsanitize=address,undefined -lasan -lubsan
endif


OUT_O_DIR ?= bin
EXECUTABLE_DIR ?= build
COMMONINC = -I./include
SRC = ./src
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
EXECUTABLE ?= fileread

-include local.mk

override CFLAGS += $(COMMONINC)

CSRC =  main.cpp                  \
		src/fileread/fileread.cpp  \


ifeq ($(BUILD_TYPE), debug)
CSRC += src/log/log.cpp
endif


COBJ := $(addprefix $(OUT_O_DIR)/,$(CSRC:.cpp=.o))
DEPS = $(COBJ:.o=.d)

.PHONY: all
all: $(EXECUTABLE_DIR)/$(EXECUTABLE)

$(EXECUTABLE_DIR)/$(EXECUTABLE): $(COBJ)
	@mkdir -p $(@D)
	$(CC) $^ -o $@ $(LDFLAGS)

$(COBJ) : $(OUT_O_DIR)/%.o : %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEPS) : $(OUT_O_DIR)/%.d : %.cpp
	@mkdir -p $(@D)
	@$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@


#======= run ==========================================

.PHONY: run

run:
	@./$(EXECUTABLE_DIR)/$(EXECUTABLE)

#======= clean ========================================

.PHONY: clean cleanDirs cleanLog
clean:
	rm -rf $(COBJ) $(DEPS) $(EXECUTABLE_DIR)/$(EXECUTABLE) $(OUT_O_DIR)/$(SRC)

cleanDirs:
	rm -rf $(OUT_O_DIR) $(EXECUTABLE_DIR)

cleanLog:
	rm -rf Log/

#======================================================

NODEPS = clean

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
include $(DEPS)
endif
