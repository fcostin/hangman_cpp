WARNING_FLAGS := -Wall -Wextra -Werror -pedantic -Wconversion
OPT_FLAGS := -O3
all: a.out
	./a.out
.PHONY: all

a.out: main.o hangman_context.o hangman_types.o alpha_beta.o evaluation.o move_apply.o \
		move_gen.o
	g++ $(WARNING_FLAGS) $(OPT_FLAGS) -o $@ $^

%.o:	%.cpp
	g++ $(WARNING_FLAGS) $(OPT_FLAGS) -fPIC -c $^

clean:
	rm -rf *.out
	rm -rf *.o
.PHONY: clean

