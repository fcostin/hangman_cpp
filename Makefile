WARNING_FLAGS := -Wall -Wextra -Werror -pedantic -Wconversion
OPT_FLAGS := -g
# ref : http://stackoverflow.com/questions/9577627/ubuntu-11-10-linking-perftools-library
WITH_PROFILING := -Wl,--no-as-needed -lprofiler -Wl,-as-needed
all: a.out
	./a.out 3 1
.PHONY: all

a.out: main.o hangman_context.o hangman_types.o alpha_beta.o evaluation.o move_apply.o \
		move_gen.o
	g++ $(WARNING_FLAGS) $(OPT_FLAGS) $(WITH_PROFILING) -o $@ $^

%.o:	%.cpp
	g++ $(WARNING_FLAGS) $(OPT_FLAGS) -c $^

clean:
	rm -rf *.out
	rm -rf *.o
.PHONY: clean

