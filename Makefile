WARNING_FLAGS := -Wall -Wextra -Werror -pedantic -Wconversion
OPT_FLAGS := -g -O3
CPPC_FLAGS := $(WARNING_FLAGS) $(OPT_FLAGS) -std=c++0x
# ref : http://stackoverflow.com/questions/9577627/ubuntu-11-10-linking-perftools-library
WITH_PROFILING := -Wl,--no-as-needed -lprofiler -Wl,-as-needed
all: a.out
.PHONY: all

a.out: main.o hangman_context.o alpha_beta.o evaluation.o move_apply.o \
		move_gen.o
	g++ $(CPPC_FLAGS) $(WITH_PROFILING) -o $@ $^

%.o:	%.cpp
	g++ $(CPPC_FLAGS) -c $^

clean:
	rm -rf *.out
	rm -rf *.o
.PHONY: clean

