CC = clang -O0 -g -pthread

.PHONY: clean

ao: ao.o \
	download.o \
	http_header.o \
	http_request.o \
	http_response.o \
	http_util.o \
	text.o \
	transport.o \
	url.o \

clean:
	-rm *.o ao
