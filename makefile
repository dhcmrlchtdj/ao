CC = clang
CFLAGS = -O0 -g
LDFLAGS = -lpthread
FLAGS = $(CFLAGS) $(LDFLAGS)

source = text.o\
		 utils.o

ao: $(source)
	$(CC) $(FLAGS) $(source) -o ao

.PHONY : clean
clean:
	-rm $(source) ao
