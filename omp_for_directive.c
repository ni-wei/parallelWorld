#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char *colored_digit[] = {
	"\e[1;30;1m0",
	"\e[1;31;1m1",
	"\e[1;32;1m2",
	"\e[1;33;1m3",
	"\e[1;34;1m4",
	"\e[1;35;1m5",
	"\e[1;36;1m6",
	"\e[1;37;1m7"
};

int main(int argc, char** argv) {

	unsigned int x_size = 80;
	unsigned int y_size = 40;
	unsigned long str_len = strlen(colored_digit[0]);
	char *string_2D = malloc(x_size * y_size * str_len + y_size);

	#pragma omp parallel for schedule(runtime)
	for (unsigned long i = 0; i < y_size; i++) {
		for (unsigned int j = 0; j < x_size; j++) {
			memcpy(string_2D + (i * x_size * str_len + i) + (j * str_len), colored_digit[omp_get_thread_num()], str_len);
		}
	}
	for (unsigned int i = 0; i < y_size; i++) {
		unsigned long row = i * x_size * str_len + i;
		printf("%s\n", string_2D + row);
	}
	printf("\033[0m");
	return 0;
}