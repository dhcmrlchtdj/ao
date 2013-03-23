#include "ao.h"


int main(int argc, char *argv[]) {
	int max_task = 5;

	opterr = 0; // do not print error message
	char *opt_string = "f:F:p::n::o::h";
	char opt_char = getopt(argc, argv, opt_string);

	switch (opt_char) {
		case 'f':
			printf("%c: %s\n", opt_char, optarg);
			// start download
			break;
		case 'F':
			printf("%c: %s\n", opt_char, optarg);
			// start download
			break;
		case 'p':
			printf("%c: %s\n", opt_char, optarg);
			// start rpc
			break;
		case 'h':
		case '?':
			print_usage();
			break;
		case -1:
			if (argc != 2) {
				print_usage();
				return 0;
			}
	}

	AO *ao = init_ao(max_task);
	memcpy(ao->url, argv[1], strlen(argv[1]) + 1);
	start_download(ao);

	return 0;
}



void print_usage(void) {
	char *usage = "Usage:"
		"\taxel URL\t下载\n"
		"\taxel -f FILE\t下载多个文件\n"
		"\taxel -F FILE\t同时下载多个文件\n"
		"\taxel -p[PORT]\t监听PORT端口\n"
		"\taxel -h\t\t帮助\n";
	printf("%s\n", usage);
}
