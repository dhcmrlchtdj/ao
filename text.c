#include "ao.h"


void parse_options(environ_t *env, int argc, char *argv[]) {
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
		parse_url(&env->url, argv[optind]); // get download link
		printf("[ao] fetch '%s'.\n", argv[optind]);
	}
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


///////////////////////////////////////////////////////////////////////////////


void output_progress_bar(environ_t *env) {
	Gettimeofday(&env->t2);
	if (env->filesize == 0) {
		_unknown_size(env);
	} else {
		_known_size(env);
	}
}



void _known_size(environ_t *env) {
	static size_t last_size = 0;

	long delta_time = time_diff(&env->t1, &env->t2);
	off_t delta_size = env->download_size - last_size;
	if (delta_time == 0 || delta_size == 0) return;

	double speed = delta_size / delta_time; // byte / ms
	int left_time = (env->filesize - env->download_size) / speed / 1000;
	int left_min = left_time / 60;
	int left_sec = left_time % 60;

	last_size = env->download_size;
	env->t1 = env->t2;

	// back to start of the progress bar
	int pos, percent;
	pos = 0;
	while (pos++ < 200) putchar('\b');

	percent = env->download_size / env->filesize;
	printf("[%3d%%] [", percent);

	pos = 0;
	percent /= 2;
	while (pos++ < percent) putchar('.');
	(pos == 51) ? putchar('.') : putchar('#');

	printf("%*c [ %4.1fKiB/s] [%02d:%02d]   \b\b\b",
			52 - pos, ']', speed * 0.97656, left_min, left_sec);

	fflush(stdout);
}



void _unknown_size(environ_t *env) {
	static size_t last_size = 0;

	long delta_time = time_diff(&env->t1, &env->t2);
	off_t delta_size = env->download_size - last_size;
	if (delta_time == 0 || delta_size == 0) return;

	double speed = delta_size / delta_time; // byte / ms

	last_size = env->download_size;
	env->t1 = env->t2;

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

	printf("%*c [ %4.1fKiB/s] [??:??]   \b\b\b",
			52 - pos, ']', speed * 0.97656);

	fflush(stdout);
}
