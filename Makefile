CC = clang
CFLAGS = -O0 -g
LDFLAGS = -pthread
FLAGS = $(CFLAGS) #$(LDFLAGS)

source = \
		 ao.o\
		 download.o\
		 http_header.o\
		 http_request.o\
		 http_response.o\
		 text.o\
		 transport.o\
		 url.o\
		 utils.o\
		 wrap.o\


ao: $(source)
	$(CC) $(FLAGS) $(source) -o ao


.PHONY : clean
clean:
	-rm $(source) ao
