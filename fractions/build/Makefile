CC := gcc
CFLAGS := -g -Wall -Wextra -Wpedantic \
		  -Wformat=2 -Wno-unused-parameter \
		  -Wshadow -Wwrite-strings -Wstrict-prototypes \
		  -Wold-style-definition -Wredundant-decls \
		  -Wnested-externs -Wmissing-include-dirs \
		  -Wjump-misses-init -Wlogical-op -O2

PADDING 				:= 		............................................................

SRC_PATH := ../src
SRC_FILES := $(wildcard $(SRC_PATH)/*.c)

LIBS_PATH 				:= 		../libs

DYNAMIC_LIB_NAME 		:= 		libfrac.so
DYNAMIC_LIB 			:= 		$(LIBS_PATH)/$(DYNAMIC_LIB_NAME)
DYNAMIC_OBJ_PATH 		:= 		dynamic_obj
DYNAMIC_OFILES 			:= 		$(patsubst $(SRC_PATH)/%.c,$(DYNAMIC_OBJ_PATH)/%.o,$(SRC_FILES))

STATIC_LIB_NAME 		:= 		libfrac.a
STATIC_LIB 				:= 		$(LIBS_PATH)/$(STATIC_LIB_NAME)
STATIC_OBJ_PATH 		:= 		static_obj
STATIC_OFILES 			:= 		$(patsubst $(SRC_PATH)/%.c,$(STATIC_OBJ_PATH)/%.o,$(SRC_FILES))

.PHONY: all build clean

all: build clean
	@printf "Finished Project "
	@printf "%0.37s" $(PADDING)
	@printf "%s\n\n" " PASSED"

build: $(LIBS_PATH) $(DYNAMIC_OBJ_PATH) $(DYNAMIC_LIB) $(STATIC_OBJ_PATH) $(STATIC_LIB)

# Create a directory for the libraries
$(LIBS_PATH):
	@mkdir -p $@

# Create a directory for the dynamic object files
$(DYNAMIC_OBJ_PATH):
	@mkdir -p $@

# Create the dynamic library
$(DYNAMIC_LIB): $(DYNAMIC_OFILES)
	@printf "\n"
	
	@$(CC) -shared -o $@ $^

	@printf "%s" "Building Dynamic Library "
	@printf "%0.29s" $(PADDING)
	@printf "%s\n\n" " PASSED"

# Create all object files from the source code for dynamic library
$(DYNAMIC_OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@gcc -fPIC $(CFLAGS) -o $@ -c $<

	@printf "%s" "Building dynamic $(shell basename $@ .o) "
	@file=$@ && printf "%0.$$((50 - $${#file}))s" $(PADDING)
	@printf "%s\n" " PASSED"

# Create a directory for the static object files
$(STATIC_OBJ_PATH):
	@mkdir -p $@

# Create the static library
$(STATIC_LIB): $(STATIC_OFILES)
	@printf "\n"
	
	@ar -rc $@ $^

	@printf "%s" "Building Static Library "
	@printf "%0.30s" $(PADDING)
	@printf "%s\n\n" " PASSED"

# Create all object files from the source code for static library
$(STATIC_OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@gcc $(CFLAGS) -o $@ -c $<

	@printf "%s" "Building static $(shell basename $@ .o) "
	@file=$@ && printf "%0.$$((50 - $${#file}))s" $(PADDING)
	@printf "%s\n" " PASSED"

clean:
	@rm -rf $(DYNAMIC_OBJ_PATH) $(STATIC_OBJ_PATH)

cleanall:
	@rm -rf $(DYNAMIC_OBJ_PATH) $(STATIC_OBJ_PATH) $(LIBS_PATH)