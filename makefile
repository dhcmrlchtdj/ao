CC = clang

.PHONY: clean

ao: ao.o \
	download.o \
	http_header.o \
	http_request.o \
	http_response.o \
	http_util.o \
	option.o \
	transport.o \

clean:
	-rm *.o ao
