# g++
WARNING_FLAGS := -Wall -Wextra -Werror -pedantic -Wconversion
OPT_FLAGS := -O3
GPROF_FLAGS := -g -pg
CPPC_FLAGS := $(WARNING_FLAGS) $(OPT_FLAGS) -std=c++0x -march=core2
# for google-perftools profiler
# ref : http://stackoverflow.com/questions/9577627/ubuntu-11-10-linking-perftools-library
WITH_PROFILING := -Wl,--no-as-needed -lprofiler -Wl,-as-needed
CPPC := g++

# # clang
# CPPC := clang++
# CPP_FLAGS := -std=c++0x

all: a.out
.PHONY: all

a.out: main.o hangman_context.o alpha_beta.o evaluation.o move_apply.o \
		move_gen.o exclusion.o forest.o
	$(CPPC) $(CPPC_FLAGS) -o $@ $^

%.o:	%.cpp
	$(CPPC) $(CPPC_FLAGS) -c $^

clean:
	rm -rf *.out
	rm -rf *.o
.PHONY: clean

