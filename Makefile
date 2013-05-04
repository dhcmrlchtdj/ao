CC = clang
CFLAGS = -O0 -g
LDFLAGS = -lpthread
FLAGS = $(CFLAGS) $(LDFLAGS)

source = \
		 common.o\
		 download.o\
		 http_header.o\
		 http_request.o\
		 http_response.o\
		 text.o\
		 transport.o\
		 url.o\


ao: $(source)
	$(CC) $(FLAGS) $(source) -o ao


.PHONY : clean
clean:
	-rm $(source) ao
