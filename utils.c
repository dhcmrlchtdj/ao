#include "ao.h"


char *_copy_str(char *src, size_t len) {
	char *dest = malloc(sizeof(char) * (len + 1));
	memcpy(dest, src, len);
	dest[len] = '\0';
	return dest;
}
