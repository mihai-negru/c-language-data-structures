CFLAGS += -c -g -Wall -Wextra -Wpedantic \
		  -Wformat=2 -Wno-unused-parameter \
		  -Wshadow -Wwrite-strings -Wstrict-prototypes \
		  -Wold-style-definition -Wredundant-decls \
		  -Wnested-externs -Wmissing-include-dirs \
		  -Wjump-misses-init -Wlogical-op -O2

PATH_TO_FILES = ../src/

FILES = $(PATH_TO_FILES)scl_avl_tree.c $(PATH_TO_FILES)scl_bst_tree.c \
		$(PATH_TO_FILES)scl_graph.c $(PATH_TO_FILES)scl_list.c \
		$(PATH_TO_FILES)scl_dlist.c $(PATH_TO_FILES)scl_priority_queue.c \
		$(PATH_TO_FILES)scl_hash_table.c $(PATH_TO_FILES)scl_sort_algo.c \
		$(PATH_TO_FILES)scl_red_black_tree.c \
		$(PATH_TO_FILES)scl_queue.c $(PATH_TO_FILES)scl_stack.c $(PATH_TO_FILES)scl_config.c \
		$(PATH_TO_FILES)scl_func_types.c \

OUTFILES = scl_avl_tree.o scl_bst_tree.o scl_graph.o \
		 scl_dlist.o scl_hash_table.o scl_config.o\
		 scl_priority_queue.o scl_list.o \
		 scl_red_black_tree.o scl_queue.o scl_sort_algo.o \
		 scl_stack.o scl_func_types.o \
		 

LIB = libdatastruc.a

build: objFiles staticLib clean

objFiles: $(FILES)
	gcc $(CFLAGS) $(FILES)

staticLib: $(OUTFILES)
	ar -rc $(LIB) *.o

delLib: $(LIB)
	rm -rf $(LIB)

clean:
	@find . -type f -name "*.o" -exec rm -rf {} \;