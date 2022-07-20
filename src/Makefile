CFLAGS = -c -g -Wextra -Werror -Wall

FILES = ./avlTreeUtils.c ./bstUtils.c \
		./weightedGraphsAlgo.c ./scl_list.c \
		./scl_dlist.c ./scl_priority_queue.c \
		./hashTableUtils.c ./sortAlgorithms.c \
		./ordGraphUtils.c ./redBlackTreeUtils.c \
		./scl_queue.c ./scl_stack.c \
		./unordGraphUtils.c ./scl_func_types.c \

OUTFILES = avlTreeUtils.o bstUtils.o weightedGraphsAlgo.o \
		 scl_dlist.o hashTableUtils.o \
		 scl_priority_queue.o scl_list.o ordGraphUtils.o \
		 redBlackTreeUtils.o scl_queue.o sortAlgorithms.o \
		 scl_stack.o unordGraphUtils.o scl_func_types.o \
		 

LIBNAME = libdatastruc.a

build: objFiles staticLib clean

objFiles: $(FILES)
	@gcc $(CFLAGS) $(FILES)

staticLib: $(OUTFILES)
	@ar -rc $(LIBNAME) *.o

delLib: $(LIBNAME)
	@rm -rf $(LIBNAME)

test: libdatastruc.a ztest.c
	@gcc ztest.c -g -Wall -Wextra -L. -ldatastruc -o ztest

run: test
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./ztest

clean:
	@find . -type f -name "*.o" -exec rm -rf {} \;
	@rm -rf ztest