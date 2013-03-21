#include "ao.h"


ao_t *init_ao_t(void) {
	ao_t *ao = malloc(sizeof(ao_t));
	return ao;
}



void free_ao_t(ao_t *ao) {
	free_url_t(ao->url);
	free_request_t(ao->request);
	free_response_t(ao->response);
	free(ao);
}



