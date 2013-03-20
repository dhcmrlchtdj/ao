CC = clang

.PHONY: clean

ao: ao.o \
	download.o \
	http.o \
	option.o \
	transport.o \

clean:
	-rm *.o ao
