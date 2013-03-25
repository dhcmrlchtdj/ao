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
	/*static off_t last_size;*/
	/*static time_t last_time;*/

	struct stat file_stat;
	fstat(env->fd, &file_stat);
	// st_blocks * 512 approximates the actual file size
	off_t file_size = file_stat.st_blocks * 512;
	/*time_t now_time = time(NULL);*/
	/*double speed = (file_size - last_size) / difftime(now_time, last_time);*/
	/*double remain = (env->filesize - file_size) / speed;*/

	int pos = 100;
	while (pos-- > 0) putchar('\b');
	int percent = file_size * 100 / env->filesize;
	printf("[%3d%%] [", percent);
	percent /= 2;
	while (pos++ < percent) putchar('.');
	if (pos++ < 51) {
		putchar('#');
		while (pos++ < 51)
			putchar(' ');
	}
	/*printf("] [speed %f] [time %f]\n", speed, remain);*/
	printf("] [speed] [time]");

	fflush(stdout);
	/*last_size = file_size;*/
	/*last_time = now_time;*/
}

