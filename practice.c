#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include "sum.c"

void practice(FILE *session_pointer, bool session_backup) {
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
	char prompt[200];
	size_t user_option = 0;
	while (1) {
		user_option = get_user_option(session_pointer, session_backup);
		if (user_option != 0 && user_option <= options_size) {
			switch (user_option) {
				case 1:
					main_sum(session_pointer, session_backup);
					break;
			}
		} else {
			sleep(1);
			sprintf(prompt, "\nInvalid option.\n\nEnter your option again:");
			puts(prompt);
			if (session_backup) {
				fputs(prompt, session_pointer);
			}
			sleep(1);
			display_options(descriptions, options_size, true, session_pointer, session_backup);
			continue;
		}
		sleep(1);
		sprintf(prompt, "\n%s", repeat('*', 105));
		puts(prompt);
		if (session_backup) {
			fputs(prompt, session_pointer);
		}
		sprintf(prompt, "\nSelect an option to continue with another exercise, or enter the letter \"s\" to quit the program.\n");
		puts(prompt);
		if (session_backup) {
			fputs(prompt, session_pointer);
		}
		display_options(descriptions, options_size, true, session_pointer, session_backup);
	}
}
