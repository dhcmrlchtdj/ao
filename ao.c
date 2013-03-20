#include "ao.h"


void free_ao_t(ao_t *ao) {
	close(ao->sockfd);
	/*fclose(ao->file);*/

	free_url_t(ao->url);
	free_request_t(ao->request);
	free_response_t(ao->response);

	free(ao);
}



char *_copy_str(char *src, size_t len) {
	char *dest = malloc(sizeof(char) * (len + 1));
	memcpy(dest, src, len);
	dest[len] = '\0';
	return dest;
}
