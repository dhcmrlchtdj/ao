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



void print_progress_bar(env_t *env) {
	if (env->filesize == 0) {
		// for unknown filesize
		printf("\b.#");
		fflush(stdout);
		return;
	}

	struct stat file_stat;
	off_t file_size;
	int pos, percent;
	
	static int speed = 0;
	static int left_time = 0;
	long delta = delta_time(&env->t1, &env->t2);

	// `st_blocks * 512` approximates the actual file size
	fstat(env->fd, &file_stat);
	file_size = file_stat.st_blocks * 512;

	if (delta >= 500) {
		speed = (file_size - env->last_size) * 1000 / delta;
		left_time = (env->filesize - file_size) / speed + 1;
		speed /= 1024;

		env->last_size = file_size;
		env->t1 = env->t2;
	}

	// back to start of the progress bar
	pos = 0;
	while (pos++ < 200) putchar('\b');

	// file_size maybe large than actual file size
	percent = file_size * 100 / env->filesize;
	if (percent > 100) percent = 100;
	printf("[%3d%%] [", percent);

	pos = 0;
	percent /= 2;
	while (pos++ < percent) putchar('.');
	(pos == 51) ? putchar('.') : putchar('#');

	printf("%*c [ %dKiB/s] [ %2ds]", 52 - pos, ']', speed, left_time);

	fflush(stdout);
}
