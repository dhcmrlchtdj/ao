#include "ao.h"

int main(int argc, char *argv[]) {
	opterr = 0; // do not print error message
	char *opt_string = "f:F:n:o:h";
	char opt_char;

	int from_file = 0; // 0 1 2
	int num = 5;
	char filename[SHORT_STR];

	while (1) {
		opt_char = getopt(argc, argv, opt_string);

		if (opt_char == -1) {
			break; //解析结束
		} else if (opt_char == '?' || opt_char == 'h') {
			print_usage(); // 错误选项 或 输出帮助
			exit(EXIT_FAILURE);
		} else if (opt_char == 'n') {
			num = atoi(optarg);
			if (num <= 0) { // 无效的线程数/任务数
				print_usage();
				exit(EXIT_FAILURE);
			}
		} else if (opt_char == 'o') {
			static_copy(filename, SHORT_STR, optarg, strlen(optarg));
		} else {
			// (opt_char == 'f' || opt_char == 'F')
			print_usage();
			exit(EXIT_FAILURE);
		}
	}

	if (argc - optind != 1) { // 没有输入下载地址
		print_usage();
		exit(EXIT_FAILURE);
	}

	AO *ao = init_AO(num);
	static_copy(ao->url, SHORT_STR, argv[optind], strlen(argv[optind]));
	dl_epoll(ao);
	free_AO(ao);

	return 0;
}



void print_usage(void) {
	char *usage = "Usage:"
		"\taxel URL\t下载\n"
		"\taxel -f FILE\t下载多个文件\n"
		"\taxel -F FILE\t同时下载多个文件\n"
		/*"\taxel -p[PORT]\t监听PORT端口\n"*/
		"\taxel -h\t\t帮助\n";
	printf("%s\n", usage);
}
