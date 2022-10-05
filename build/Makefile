### CONSTANT DEFINITIONS ###

# C language compiler and flags
CC 						:=		gcc
CFLAGS 					:= 		-g -Wall -Wextra -Wpedantic \
								-Wformat=2 -Wno-unused-parameter \
								-Wshadow -Wwrite-strings -Wstrict-prototypes \
								-Wold-style-definition -Wredundant-decls \
								-Wnested-externs -Wmissing-include-dirs \
								-Wjump-misses-init -Wlogical-op -O2

# Linux commands for basic routines
COPY					:=		cp
LIB_CONF				:=		ldconfig
REMOVE					:=		rm

# Maximum Padding String
PADDING 				:= 		............................................................

# Source files info
SRC_PATH 				:= 		../src
SRC_FILES 				:= 		$(wildcard $(SRC_PATH)/*.c)

# Libraries files info
LIBS_PATH 				:= 		../libs

# Dynamic Library info
DYNAMIC_LIB_NAME 		:= 		libdstruc.so
DYNAMIC_LIB 			:= 		$(LIBS_PATH)/$(DYNAMIC_LIB_NAME)
DYNAMIC_OBJ_PATH 		:= 		dynamic_obj
DYNAMIC_OFILES 			:= 		$(patsubst $(SRC_PATH)/%.c,$(DYNAMIC_OBJ_PATH)/%.o,$(SRC_FILES))

# Static Library info
STATIC_LIB_NAME 		:= 		libdstruc.a
STATIC_LIB 				:= 		$(LIBS_PATH)/$(STATIC_LIB_NAME)
STATIC_OBJ_PATH 		:= 		static_obj
STATIC_OFILES 			:= 		$(patsubst $(SRC_PATH)/%.c,$(STATIC_OBJ_PATH)/%.o,$(SRC_FILES))

# Header files info
INCLUDE_PATH 			:= 		$(SRC_PATH)/include
HEADER_FILES			:=		$(wildcard $(INCLUDE_PATH)/*.h)

# Installing Dynamic Library info
INSTALL_PATH 			:= 		/usr/local
INSTALL_INCLUDE_PATH	:= 		$(INSTALL_PATH)/include
INSTALL_LIB_PATH 		:= 		$(INSTALL_PATH)/lib

# Unninstall Dynamic Library info
UNINSTALL_HEADER_FILES	:=		$(patsubst $(INCLUDE_PATH)/%.h,$(INSTALL_INCLUDE_PATH)/%.h,$(HEADER_FILES))

.PHONY: all build clear_term install_header_files install install_dynamic_lib register_dynamic_lib clean uninstall uninstall_header_files uninstall_dynamic_lib

### END OF CONSTANT DEFINITIONS ###

### DEFAULT TARGET ###

# Target to Build and Install the project
all: build clean install
	@printf "Finished Project "
	@printf "%0.37s" $(PADDING)
	@printf "%s\n\n" " PASSED"

### END OF DEFAULT TARGET ###

### BUILDING ###

# Target to build the libraries from source files
build: clear_term $(LIBS_PATH) $(DYNAMIC_OBJ_PATH) $(DYNAMIC_LIB) $(STATIC_OBJ_PATH) $(STATIC_LIB)

# Target to clear the terminal screen
clear_term:
	@clear

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
$(DYNAMIC_OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(INCLUDE_PATH)/%.h
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
$(STATIC_OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(INCLUDE_PATH)/%.h
	@gcc $(CFLAGS) -o $@ -c $<

	@printf "%s" "Building static $(shell basename $@ .o) "
	@file=$@ && printf "%0.$$((50 - $${#file}))s" $(PADDING)
	@printf "%s\n" " PASSED"

### END OF BUILDING ###

### INSTALLING DYNAMIC LIBRARY ###

# Target to load the dynamic librari on system
install: install_header_files install_dynamic_lib register_dynamic_lib

# Target to copy all header files to default system header location
install_header_files: $(HEADER_FILES)
	@$(COPY) -R $^ $(INSTALL_INCLUDE_PATH)

	@printf "Installing Header Files "
	@printf "%0.30s" $(PADDING)
	@printf "%s\n" " PASSED"

# Target to copy the dynamic library to default system library location
install_dynamic_lib: $(DYNAMIC_LIB)
	@$(COPY) $(DYNAMIC_LIB) $(INSTALL_LIB_PATH)

	@printf "Installing Dynamic Library "
	@printf "%0.27s" $(PADDING)
	@printf "%s\n" " PASSED"

# Target to load the dynamic library on system
register_dynamic_lib: $(INSTALL_LIB_PATH)/$(DYNAMIC_LIB_NAME)
	@cd $(INSTALL_LIB_PATH)/ && $(LIB_CONF)

	@printf "Loading Dynamic Library "
	@printf "%0.30s" $(PADDING)
	@printf "%s\n\n" " PASSED"

### END OF INSTALLING DYNAMIC LIBRARY ###

### REMOVE DYNAMIC LIBRARY ###

uninstall: uninstall_header_files uninstall_dynamic_lib
	@printf "Library purged successfully\n"

uninstall_header_files: $(UNINSTALL_HEADER_FILES)
	@sudo $(REMOVE) -rf $^ 


uninstall_dynamic_lib: $(INSTALL_LIB_PATH)/$(DYNAMIC_LIB_NAME)
	@sudo $(REMOVE) -rf $<

### END OF REMOVE DYNAMIC LIBRARY ###

### CLEANING ###

# Target to clean just object files
clean:
	@$(REMOVE) -rf $(DYNAMIC_OBJ_PATH) $(STATIC_OBJ_PATH)

# Target to clean everything obtained from all or build target
cleanall:
	@$(REMOVE) -rf $(DYNAMIC_OBJ_PATH) $(STATIC_OBJ_PATH) $(LIBS_PATH)

### END OF CLEANING ###