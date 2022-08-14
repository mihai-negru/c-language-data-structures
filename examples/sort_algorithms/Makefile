CFLAGS += -c -g -Wall -Wextra -Wpedantic \
		  -Wformat=2 -Wno-unused-parameter \
		  -Wshadow -Wwrite-strings -Wstrict-prototypes \
		  -Wold-style-definition -Wredundant-decls \
		  -Wnested-externs -Wmissing-include-dirs \
		  -Wjump-misses-init -Wlogical-op -O2

DYNAMIC_LIB += /usr/local/lib/libdstruc.so

VALGRIND_FLAGS += --leak-check=full \
				  --show-leak-kinds=all \
				  --track-origins=yes

VALGRIND += valgrind $(VALGRIND_FLAGS)

.PHONY: build e1 clean

build: e1

e1: example1
	@printf "Run SORT ALGOS example no. 1: "
	@$(VALGRIND) --log-file=valgrind_log_1.txt ./example1
	@printf "Done\n"
	@rm -rf example1 example1.o

example1: example1.o $(DYNAMIC_LIB)
	@gcc example1.o -ldstruc -o example1

example1.o: example1.c
	@gcc $(CFLAGS) example1.c

clean:
	@rm -rf *.txt *o example1 example2 example3
