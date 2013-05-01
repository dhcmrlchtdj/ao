#include "ao.h"


int main(int argc, char *argv[]) {
	opterr = 0; // do not print error message
	char *opt_string = "n:o:h";
	char opt_char;

	while (1) {
		opt_char = getopt(argc, argv, opt_string);

		if (opt_char == -1) {
			break; // no more options
		} else if (opt_char == 'n') {
			// 块数
		} else if (opt_char == 'o') {
			// 文件名
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
		printf("%s\n", argv[optind]);
	}
	//sysconf(_SC_NPROCESSORS_CONF)

	return 0;
}



void print_usage(void) {
	char *usage = "Usage:\n"
		"\tao [options] <url>\tdownload url\n"
		"Options:\n"
		"\t-n<num>\t\t\tdivide into num parts, default 6\n"
		"\t-o<filename>\t\tsave as filename\n"
		"\t-h\t\t\tprint help info\n";
	printf("%s\n", usage);
}
