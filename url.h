#define SCHEME_LEN 6 // http, https
#define PORT_LEN 6 // max 65535
#define HOST_LEN 256 // domain must shorter than 253(?)

struct url_t {
	char scheme[SCHEME_LEN];
	char port[PORT_LEN];
	char host[HOST_LEN];
	char path[SHORT_STR];
};

url_t *new_url(void);
void del_url(url_t *url);

url_t *parse_url(char *url);
