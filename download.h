struct url_t {
	char *host;
	char *port;
	char *path;
};

url_t *parse_url(char *url);
void free_url_t(url_t *url);

char *get_filename(char *str);


/* * * * * * * * * */


void download_by_url(char *url);


/* * * * * * * * * */

void save(ao_t *ao);
