#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>

void practice(FILE *session_pointer, bool session_backup, const char *password) {
	size_t options_size = 0;
	char **options_lines = file_to_array_of_lines("options.txt", '\n', &options_size);
	char **descriptions = (char **) calloc(options_size, sizeof(char*));
	char **programs = (char **) calloc(options_size, sizeof(char*));
	for (size_t i = 0; i < options_size; i++) {
		size_t parts_size = 0;
		char **parts = split(options_lines[i], ';', &parts_size);
		descriptions[i] = parts[0];
		programs[i] = parts[1];
	}
	display_options(descriptions, options_size, true, session_pointer, session_backup);
}
