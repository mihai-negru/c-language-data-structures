CFLAGS += -g -Wall -Wextra -Wpedantic \
		  -Wformat=2 -Wno-unused-parameter \
		  -Wshadow -Wwrite-strings -Wstrict-prototypes \
		  -Wold-style-definition -Wredundant-decls \
		  -Wnested-externs -Wmissing-include-dirs \
		  -Wjump-misses-init -Wlogical-op -O2

VALGRIND_FLAGS += --leak-check=full \
				  --show-leak-kinds=all \
				  --track-origins=yes

VALGRIND += valgrind $(VALGRIND_FLAGS)

.PHONY: build e1 e2 e3 clean

build: e1 e2 e3

e1: example1
	@printf "Run MQUEUE example 1: "
	@$(VALGRIND) --log-file=valgrind_log_1.txt ./example1
	@printf "Done\n"
	@rm -rf example1

e2: example2
	@printf "Run MQUEUE example 2: "
	@$(VALGRIND) --log-file=valgrind_log_2.txt ./example2
	@printf "Done\n"
	@rm -rf example2

e3: example3
	@printf "Run MQUEUE example 3: "
	@$(VALGRIND) --log-file=valgrind_log_3.txt ./example3
	@printf "Done\n"
	@rm -rf example3

example%: example%.c
	@gcc $(CFLAGS) $< -o $@

clean:
	@rm -rf *.txt example1 example2 example3
