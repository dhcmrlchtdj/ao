CC = clang
CFLAGS = -O0 -g
LDFLAGS = -lpthread
FLAGS = $(CFLAGS) $(LDFLAGS)

source = \
		 common.o\
		 text.o\
		 url.o\
		 http_header.o\
		 http_request.o\
		 http_response.o\


ao: $(source)
	$(CC) $(FLAGS) $(source) -o ao


.PHONY : clean
clean:
	-rm $(source) ao
