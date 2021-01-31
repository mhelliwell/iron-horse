


IH_BASE_PATH := $(shell pwd)/

include build/basic_defs.mk


#$(eval $(call gen_sym, TEST_SYMBOL, this_is_a_test))
#$(info $(TEST_SYMBOL))

build: PREBUILD_TARGETS BUILD_TARGETS POSTBUILD_TARGETS
.PHONEY: build

include $(IH_BASE_PATH)source/ih.mk


PREBUILD_TARGETS:
BUILD_TARGETS:
POSTBUILD_TARGETS:
.PHONEY: PREBUILD_TARGETS BUILD_TARGETS POSTBUILD_TARGETS



create_dirs:
	@mkdir -p $(IH_BIN_RESULTS_PATH)
	@mkdir -p $(IH_LIB_RESULTS_PATH)
	@mkdir -p $(IH_OBJ_WORKING_PATH)
.PHONEY: create_dirs
PREBUILD_TARGETS: create_dirs



########################
#  old style is below  #
########################

#.PHONEY: all clean clobber rebuild


#CCFLAGS := -Wall -std=c++17


#all: hsc

#clean:
#	rm -f *.o

clobber:
	@rm -f hsc
	@rm -f *~
	@rm -rf results

#rebuild: clobber all


#hsc.cc:lex.h


#OBJECTS := state_machine.o tokenizer.o lex.o


#%.o : %.cc
#	g++ -c $(CCFLAGS) $< -o $@

#hsc: return.h hsc.cc tokenizer.h lex.h $(OBJECTS)
#	g++ $(CCFLAGS) $(OBJECTS) hsc.cc -o hsc

