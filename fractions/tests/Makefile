CC 					:= gcc
CFLAGS 			:= -g -Wall -Wextra -Wpedantic \
		  		 		 -Wformat=2 -Wno-unused-parameter \
		  		 		 -Wshadow -Wwrite-strings -Wstrict-prototypes \
		  		 		 -Wold-style-definition -Wredundant-decls \
		  		 		 -Wnested-externs -Wmissing-include-dirs \
		  		 		 -Wjump-misses-init -Wlogical-op -O2

MAKEFLAGS 	+= --no-print-directory

all: create_lib run_test destroy_lib clean

run_test: units
	@./units

create_lib:
	@make -C ../build

destroy_lib:
	@make -C ../build clean

units: units.o
	@$(CC) -static units.o -L../libs -lfrac -o units

units.o: units.c
	@$(CC) $(CFLAGS) -c units.c

clean:
	@rm -rf units.o units
