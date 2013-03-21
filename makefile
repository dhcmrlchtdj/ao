CC = clang

.PHONY: clean

ao: ao.o \
	download.o \
	http_conn.o \
	http_header.o \
	http_request.o \
	http_response.o \
	option.o \
	transport.o \
	url.o \

clean:
	-rm *.o ao
