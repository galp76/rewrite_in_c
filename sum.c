#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>

bool contains(char *string, char ch) {
	size_t i = 0;
	while (string[i] != '\0') {
		if (string[i] == ch) {
			return true;
		}
	}

	return false;
}

char *get_user_input_sum(const char *prompt, const char *operator, FILE *session_pointer, bool session_backup) {
	char user_input[50];
	char *prompt2 = (char*) calloc(100, 1);
	while (1) {
		puts(prompt);
		if (session_backup) {
			fputs(prompt, session_pointer);
		}
		int counter = fscanf(stdin, "%s", user_input);
		if (counter != 1) {
			sprintf(prompt2, "There was an error reading input.\n");
			printf("%s", prompt2);
			if (session_backup) {
				fputs(prompt2, session_pointer);
			}
			sleep(1);
			continue;
		}
		if (strcmp(user_input, "s") == 0) {
			if (session_backup) {
				fputs(user_input, session_pointer);
			}
			sleep(1);
			sprintf(prompt2, "\nGetting out of the exercise.\n");
			printf("%s", prompt2);
			if (session_backup) {
				fputs(prompt2, session_pointer);
			}
			sleep(1);
			continue;
			exit(0);
		}
		size_t len = strlen(user_input);
		char string[100] = "1234567890";
		strcat(string, operator);
		bool must_repeat = false;
		for (size_t i = 0; i < len; i++) {
			if (contains(string, user_input[i]) == false) {
				if (session_backup) {
					fputs(user_input, session_pointer);
				}
				sleep(1);
				sprintf(prompt2, "\nInvalid character found: %c.", string[i]);
				printf("%s", prompt2);
				if (session_backup) {
					fputs(prompt2, session_pointer);
				}
				sleep(1);
				puts(prompt);
				if (session_backup) {
					fputs(prompt, session_pointer);
				}
				while (getchar() != '\n');
				must_repeat = true;
				break;
			}
		}
		if (must_repeat) {
			continue;
		}
		break;
	}

	if (session_backup) {
		fputs(user_input, session_pointer);
	}

	return user_input;
}
