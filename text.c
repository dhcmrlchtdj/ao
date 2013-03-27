#include "ao.h"


int main(int argc, char *argv[]) {
	initial_env(&env);

	opterr = 0; // do not print error message
	char *opt_string = "f:F:n:o:h";
	char opt_char;

	while (1) {
		opt_char = getopt(argc, argv, opt_string);

		if (opt_char == -1) {
			break; // all options got
		} else if (opt_char == '?' || opt_char == 'h') {
			print_usage(); // print help
			exit(EXIT_FAILURE);
		} else if (opt_char == 'n') {
			env.task_num = atoi(optarg); // how many threads used
			if (env.task_num <= 0) {
				print_usage();
				exit(EXIT_FAILURE);
			}
		} else if (opt_char == 'o') {
			static_copy(env.file_name, SHORT_STR, optarg, strlen(optarg));
		} else {
			// (opt_char == 'f' || opt_char == 'F')
			print_usage();
			exit(EXIT_FAILURE);
		}
	}

	if (argc - optind != 1) { // no url
		print_usage();
		exit(EXIT_FAILURE);
	} else {
		static_copy(env.url, SHORT_STR, argv[optind], strlen(argv[optind]));
	}

	dl_prepare();
	dl_start();

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



void print_progress_bar(void) {
	if (env.file_size == 0) {
		// not work very well
		_print_unknown_size();
	} else {
		_print_known_size();
	}
}



void _print_known_size(void) {
	// `st_blocks * 512` approximates the actual file size
	struct stat file_stat;
	off_t file_size;
	fstat(env.fd, &file_stat);
	file_size = file_stat.st_blocks * 512;
	// file_size maybe large than actual file size
	if (file_size > env.file_size) file_size = env.file_size;

	// speed and left time
	static int speed = 0;
	static int left_min = 0;
	static int left_sec = 0;
	int left_time;

	long delta = delta_time(&env.t1, &env.t2);
	if (delta >= 500) {
		speed = (file_size - env.last_size) * 1000 / delta; // in bytes
		if (speed == 0) speed = 1;
		left_time = (env.file_size - file_size) / speed;
		left_min = left_time / 60;
		left_sec = left_time % 60;
		speed /= 1024; // convert to kib

		env.last_size = file_size;
		env.t1 = env.t2;
	}

	// back to start of the progress bar
	int pos, percent;
	pos = 0;
	while (pos++ < 200) putchar('\b');

	percent = file_size * 100 / env.file_size;
	printf("[%3d%%] [", percent);

	pos = 0;
	percent /= 2;
	while (pos++ < percent) putchar('.');
	(pos == 51) ? putchar('.') : putchar('#');

	printf("%*c [%4dKiB/s] [%02d:%02d]",
			52 - pos, ']', speed, left_min, left_sec);

	fflush(stdout);
}



void _print_unknown_size(void) {
	// `st_blocks * 512` approximates the actual file size
	struct stat file_stat;
	off_t file_size;
	fstat(env.fd, &file_stat);
	file_size = file_stat.st_blocks * 512;

	// speed and left time
	static int speed = 0;
	static int left_min = 0;
	static int left_sec = 0;
	int left_time;

	long delta = delta_time(&env.t1, &env.t2);
	if (delta >= 500) {
		speed = (file_size - env.last_size) * 1000 / delta; // in bytes
		left_time = (env.file_size - file_size) / speed;
		left_min = left_time / 60;
		left_sec = left_time % 60;
		speed /= 1024; // convert to kib

		env.last_size = file_size;
		env.t1 = env.t2;
	}

	// back to start of the progress bar
	int pos;
	pos = 0;
	while (pos++ < 200) putchar('\b');

	printf("[  ?%%] [");

	static int dot = 0;
	dot = (dot == 50) ? 0 : dot + 1;

	pos = 0;
	while (pos++ < dot) putchar('.');
	(pos == 51) ? putchar('.') : putchar('#');

	printf("%*c [%4dKiB/s] [%02d:%02d]",
			52 - pos, ']', speed, left_min, left_sec);

	fflush(stdout);
}
