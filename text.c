#include "ao.h"


int main(int argc, char *argv[]) {
	environ_t *env = new_environ();

	opterr = 0; // do not print error message
	char *opt_string = "n:o:h";
	char opt_char;

	while (1) {
		opt_char = getopt(argc, argv, opt_string);

		if (opt_char == -1) {
			break; // no more options
		} else if (opt_char == 'n') {
			env->partition = atoi(optarg); // how many threads used
			if (env->partition <= 0) {
				print_usage();
				exit(EXIT_FAILURE);
			}
		} else if (opt_char == 'o') {
			int len = strlen(optarg);
			if (len >= SHORT_STR) {
				fprintf(stderr, "[ao] Filename too long.\n");
				exit(EXIT_FAILURE);
			}
			static_copy(env->filename, SHORT_STR, optarg, len);
		} else if (opt_char == 'h') {
			print_usage(); // print help
			exit(EXIT_SUCCESS);
		} else {
			// opt_char == '?'
			print_usage();
			exit(EXIT_FAILURE);
		}
	}

	if (argc - optind != 1) { // no url
		print_usage();
		exit(EXIT_FAILURE);
	} else {
		// 获取下载地址
		env->url = parse_url(argv[optind]);
		if (env->url == NULL) {
			fprintf(stderr, "[ao] Invalid url: '%s'\n", argv[optind]);
			exit(EXIT_FAILURE);
		}
	}

	environ_update(env);
	p(env);
	del_environ(env);

	return 0;
}



void print_usage(void) {
	char *usage = "Usage:\n"
		"\tao [options] <url>\tdownload url\n"
		"Options:\n"
		"\t-n <num>\t\tdivide into num parts, default 6\n"
		"\t-o <filename>\t\tsave as filename\n"
		"\t-h\t\t\tprint help info\n";
	printf("%s\n", usage);
}
