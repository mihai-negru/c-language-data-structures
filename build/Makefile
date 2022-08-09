STATIC_CFLAGS += -c -g -Wall -Wextra -Wpedantic \
		  -Wformat=2 -Wno-unused-parameter \
		  -Wshadow -Wwrite-strings -Wstrict-prototypes \
		  -Wold-style-definition -Wredundant-decls \
		  -Wnested-externs -Wmissing-include-dirs \
		  -Wjump-misses-init -Wlogical-op -O2

DYNAMIC_CFLAGS += -c -fPIC -g -Wall -Wextra -Wpedantic \
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

STATIC_LIB = libdstruc.a
DYNAMIC_LIB = libdstruc.so

build: dynamic_lib static_lib
	@echo "   <------ Building Project Went Successfully ------->"

dynamic_lib: dynamic_obj_files
	@gcc -shared -o $(DYNAMIC_LIB) *.o
	@find . -type f -name "*.o" -exec rm -rf {} \;
	@echo "<------ Building Dynamic Library Went Successfully ------->"

dynamic_obj_files: $(FILES)
	@gcc $(DYNAMIC_CFLAGS) $(FILES)

static_lib: static_obj_files
	@ar -rc $(STATIC_LIB) *.o
	@find . -type f -name "*.o" -exec rm -rf {} \;
	@echo "<------ Building Static Library Went Successfully ------->"

static_obj_files: $(FILES)
	@gcc $(STATIC_CFLAGS) $(FILES)

del_libs:
	@rm -rf $(DYNAMIC_LIB) $(STATIC_LIB)

clean: del_libs
	@find . -type f -name "*.o" -exec rm -rf {} \;