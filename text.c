#include "ao.h"

int main(int argc, char *argv[]) {
	env_t *env = init_env();

	opterr = 0; // do not print error message
	char *opt_string = "f:F:n:o:h";
	char opt_char;

	while (1) {
		opt_char = getopt(argc, argv, opt_string);

		if (opt_char == -1) {
			break; //解析结束
		} else if (opt_char == '?' || opt_char == 'h') {
			print_usage(); // 错误选项 或 输出帮助
			exit(EXIT_FAILURE);
		} else if (opt_char == 'n') {
			env->task_num = atoi(optarg); // 获取线程数
			if (env->task_num <= 0) {
				print_usage();
				exit(EXIT_FAILURE);
			}
		} else if (opt_char == 'o') {
			static_copy(env->filename, SHORT_STR, optarg, strlen(optarg));
		} else {
			// (opt_char == 'f' || opt_char == 'F')
			print_usage();
			exit(EXIT_FAILURE);
		}
	}

	if (argc - optind != 1) { // 没有输入下载地址
		print_usage();
		exit(EXIT_FAILURE);
	} else {
		static_copy(env->url, SHORT_STR, argv[optind], strlen(argv[optind]));
	}

	dl_prepare(env);
	dl_start(env);
	free_env(env);

	return 0;
}



void print_usage(void) {
	char *usage = "Usage:"
		"\taxel [option] URL    下载\n"
		"    -n[num]\t  开启 N 个线程\n"
		"    -o[filename]  保存为 filename\n"
		"    -h\t\t  帮助\n";
	printf("%s\n", usage);
}



void print_speed(env_t *env) {
	int pos = 0;
	while (pos++ < 100) putchar('\b');

	struct stat file_stat;
	fstat(env->fd, &file_stat);

	int percent = (double)file_stat.st_size / env->filesize * 100;
	printf("[%3d%%]", percent);

	percent /= 2;
	pos = 0;
	printf(" [");
	while (pos++ < percent) putchar('.');
	while (pos++ < 50) putchar(' ');
	printf("] ");

	printf("[%s] ", "unkonwn");
	printf("[%s]", "unkonwn");

	fflush(stdout);
}

