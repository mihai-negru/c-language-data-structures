CFLAGS = -c -g -Wextra -Werror -Wall

FILES = ./avlTreeUtils.c ./bstUtils.c \
		./weightedGraphsAlgo.c ./listUtils.c \
		./doubleListUtils.c ./scl_priority_queue.c \
		./hashTableUtils.c ./sortAlgorithms.c \
		./ordGraphUtils.c ./redBlackTreeUtils.c \
		./queueUtils.c ./stackUtils.c \
		./unordGraphUtils.c ./functionTypes.c \

OUTFILES = avlTreeUtils.o bstUtils.o weightedGraphsAlgo.o \
		 doubleListUtils.o hashTableUtils.o \
		 scl_priority_queue.o listUtils.o ordGraphUtils.o \
		 redBlackTreeUtils.o queueUtils.o sortAlgorithms.o \
		 stackUtils.o unordGraphUtils.o functionTypes.o \
		 

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