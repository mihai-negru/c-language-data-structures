CFLAGS = -c -g -Wextra -Werror -Wall

FILES = ./avlTreeUtils.c ./bstUtils.c \
		./costGraphsAlgo.c ./listUtils.c \
		./doubleListUtils.c ./heapUtils.c \
		./hashTableUtils.c ./sortAlgorithms.c \
		./ordGraphUtils.c ./priQueueUtils.c \
		./queueUtils.c ./stackUtils.c \
		./unordGraphUtils.c ./functionTypes.c \
		./redBlackTreeUtils.c

OUTFILES = avlTreeUtils.o bstUtils.o costGraphsAlgo.o \
		 doubleListUtils.o hashTableUtils.o \
		 heapUtils.o listUtils.o ordGraphUtils.o \
		 priQueueUtils.o queueUtils.o sortAlgorithms.o \
		 stackUtils.o unordGraphUtils.o functionTypes.o \
		 redBlackTreeUtils.o

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

run: ztest
	@valgrind --leak-check=full --show-leak-kinds=all ./ztest

clean:
	@find . -type f -name "*.o" -exec rm -rf {} \;
	@rm -rf test