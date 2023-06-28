CC 								:= gcc
CFLAGS 						:= -g -Wall -Wextra -Wpedantic \
		  							 -Wformat=2 -Wno-unused-parameter \
		  							 -Wshadow -Wwrite-strings -Wstrict-prototypes \
		  							 -Wold-style-definition -Wredundant-decls \
		  							 -Wnested-externs -Wmissing-include-dirs \
		  							 -Wjump-misses-init -Wlogical-op -O2

PADDING 					:= 		............................................................

SRC_PATH 					:= ../src
SRC_FILES 				:= $(wildcard $(SRC_PATH)/*.c)

LIBS_PATH 				:= 		../libs

DYNAMIC_LIB_NAME 	:= 		libfrac.so
DYNAMIC_LIB 			:= 		$(LIBS_PATH)/$(DYNAMIC_LIB_NAME)
DYNAMIC_OBJ_PATH 	:= 		dynamic_obj
DYNAMIC_OFILES 		:= 		$(patsubst $(SRC_PATH)/%.c,$(DYNAMIC_OBJ_PATH)/%.o,$(SRC_FILES))

STATIC_LIB_NAME 	:= 		libfrac.a
STATIC_LIB 				:= 		$(LIBS_PATH)/$(STATIC_LIB_NAME)
STATIC_OBJ_PATH 	:= 		static_obj
STATIC_OFILES 		:= 		$(patsubst $(SRC_PATH)/%.c,$(STATIC_OBJ_PATH)/%.o,$(SRC_FILES))

.PHONY: all build clean_o clean

all: header_print clean build clean_o
	@printf "finishing project "
	@printf "%0.21s" $(PADDING)
	@printf "\033[0;32m"
	@printf "%s\n" " passed"
	@printf "\033[0m"
	@printf "%0.46s\n\n" $(PADDING)

header_print:
	@printf "%0.17s" $(PADDING)
	@printf "\033[0;32m FRAC BUILD \033[0m"
	@printf "%0.17s\n" $(PADDING)

build: $(LIBS_PATH) $(DYNAMIC_OBJ_PATH) $(DYNAMIC_LIB) $(STATIC_OBJ_PATH) $(STATIC_LIB)

$(LIBS_PATH):
	@mkdir -p $@

$(DYNAMIC_OBJ_PATH):
	@mkdir -p $@

$(DYNAMIC_LIB): $(DYNAMIC_OFILES)
	@$(CC) -shared -o $@ $^

	@printf "%s" "building dynamic library"
	@printf "%0.15s" $(PADDING)
	@printf "\033[0;32m"
	@printf "%s\n" " passed"
	@printf "\033[0m"

$(DYNAMIC_OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@gcc -fPIC $(CFLAGS) -o $@ -c $<

	@printf "%s" "building dynamic $(shell basename $@ .o)-file"
	@file=$@ && printf "%0.$$((31 - $${#file}))s" $(PADDING)
	@printf "\033[0;32m"
	@printf "%s\n" " passed"
	@printf "\033[0m"

$(STATIC_OBJ_PATH):
	@mkdir -p $@

$(STATIC_LIB): $(STATIC_OFILES)
	@ar -rc $@ $^

	@printf "%s" "building static library "
	@printf "%0.15s" $(PADDING)
	@printf "\033[0;32m"
	@printf "%s\n" " passed"
	@printf "\033[0m"

$(STATIC_OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@gcc $(CFLAGS) -o $@ -c $<

	@printf "%s" "building static $(shell basename $@ .o)-file "
	@file=$@ && printf "%0.$$((30 - $${#file}))s" $(PADDING)
	@printf "\033[0;32m"
	@printf "%s\n" " passed"
	@printf "\033[0m"

clean_o:
	@rm -rf $(DYNAMIC_OBJ_PATH) $(STATIC_OBJ_PATH)

clean:
	@rm -rf $(DYNAMIC_OBJ_PATH) $(STATIC_OBJ_PATH) $(LIBS_PATH)
