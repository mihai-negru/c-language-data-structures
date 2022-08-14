CFLAGS += -c -g -Wall -Wextra -Wpedantic \
		  -Wformat=2 -Wno-unused-parameter \
		  -Wshadow -Wwrite-strings -Wstrict-prototypes \
		  -Wold-style-definition -Wredundant-decls \
		  -Wnested-externs -Wmissing-include-dirs \
		  -Wjump-misses-init -Wlogical-op -O2

PATH_TO_FILES += ../src
PATH_TO_HEADER_FILES += $(PATH_TO_FILES)/include
INSTALL_PATH += /usr/local
INSTALL_LIB_PATH += $(INSTALL_PATH)/lib
INSTALL_HEADER_PATH += $(INSTALL_PATH)/include

FILES += $(PATH_TO_FILES)/scl_avl_tree.c $(PATH_TO_FILES)/scl_bst_tree.c \
		$(PATH_TO_FILES)/scl_graph.c $(PATH_TO_FILES)/scl_list.c \
		$(PATH_TO_FILES)/scl_dlist.c $(PATH_TO_FILES)/scl_priority_queue.c \
		$(PATH_TO_FILES)/scl_hash_table.c $(PATH_TO_FILES)/scl_sort_algo.c \
		$(PATH_TO_FILES)/scl_red_black_tree.c \
		$(PATH_TO_FILES)/scl_queue.c $(PATH_TO_FILES)/scl_stack.c $(PATH_TO_FILES)/scl_config.c \
		$(PATH_TO_FILES)/scl_func_types.c \

STATIC_LIB += libdstruc.a
DYNAMIC_LIB += libdstruc.so

.PHONY: all build dynamic_lib dynamic_obj_files static_lib static_obj_files install install_dynamic_lib register_dynamic_lib del_libs clean

all: clean build install_header_files install

build: dynamic_lib static_lib install_header_files
	@printf "   <------ Building Project Went Successfully ------->\n"

dynamic_lib: dynamic_obj_files
	@gcc -shared -o $(DYNAMIC_LIB) *.o
	@find . -type f -name "*.o" -exec rm -rf {} \;
	@printf "<------ Building Dynamic Library Went Successfully ------->\n"

dynamic_obj_files: $(FILES)
	@gcc -fPIC $(CFLAGS) $(FILES)

static_lib: static_obj_files
	@ar -rc $(STATIC_LIB) *.o
	@find . -type f -name "*.o" -exec rm -rf {} \;
	@printf "<------ Building Static Library Went Successfully ------->\n"

static_obj_files: $(FILES)
	@gcc $(CFLAGS) $(FILES)

install: install_dynamic_lib register_dynamic_lib
	@printf "  <------ Installing Project Went Successfully ------->\n"

install_dynamic_lib: $(DYNAMIC_LIB)
	@mv $(DYNAMIC_LIB) $(INSTALL_LIB_PATH)

install_header_files:
	@cp -R $(PATH_TO_HEADER_FILES)/* $(INSTALL_HEADER_PATH)

register_dynamic_lib: $(INSTALL_LIB_PATH)/$(DYNAMIC_LIB)
	@cd $(INSTALL_LIB_PATH)/ && ldconfig

del_libs:
	@rm -rf $(DYNAMIC_LIB) $(STATIC_LIB)

clean: del_libs
	@find . -type f -name "*.o" -exec rm -rf {} \;