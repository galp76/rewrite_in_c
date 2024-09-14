#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include "lib.c"

bool contains(char *string, char ch) {
	size_t i = 0;
	while (string[i] != '\0') {
//		printf("Comparing %c with %c", string[i], ch);
		if (string[i] == ch) {
			return true;
		}
		i++;
	}

	return false;
}

char *get_user_input_sum(const char *prompt, const char *operator, FILE *session_pointer, bool session_backup) {
	char *user_input = (char*) calloc(50, 1);
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
			exit(0);
		}
		size_t len = strlen(user_input);
		char string[50] = "1234567890";
		strcat(string, operator);
		bool must_repeat = false;
		for (size_t i = 0; i < len; i++) {
			if (contains(string, user_input[i]) == false) {
				if (session_backup) {
					fputs(user_input, session_pointer);
				}
				sleep(1);
				sprintf(prompt2, "\nInvalid character found: %c.\n", user_input[i]);
				printf("%s", prompt2);
				if (session_backup) {
					fputs(prompt2, session_pointer);
				}
				sleep(1);
/*				puts(prompt);
				if (session_backup) {
					fputs(prompt, session_pointer);
				}*/
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

void equal_or_not_sum(int tmp_total, const char *prompt, FILE *session_pointer, bool session_backup) {
	while (1) {
		char *user_input = get_user_input_sum(prompt, "", session_pointer, session_backup);
		int number = atoi(user_input);
		if (number == tmp_total) {
			break;
		} else {
			sleep(1);
			char prompt[100] = "\nThis is not the number we're looking for, please try again.\n";
			printf("%s", prompt);
			if (session_backup) {
				fputs(prompt, session_pointer);
			}
			sleep(1);
		}
	}
}

// ********* DEFINITIONS OF LINE STRUCT AND IYS ASSOCIATED FUNCTIONS **************

typedef struct {
	char left_sep;
	size_t left_pad;
	char *content;
	size_t right_pad;
	char right_sep;
} Line;

Line new_line(char left_sep, size_t left_pad, char *content, size_t right_pad, char right_sep) {
	Line result;
	result.left_sep = left_sep;
	result.left_pad = left_pad;
	result.content = content;
	result.right_pad = right_pad;
	result.right_sep = right_sep;

	return result;
}

char *repeat(char ch, size_t number) {
	char *result = (char*) calloc(number + 1, 1);
	for (size_t i = 0; i < number; i++) {
		result[i] = ch;
	}
	result[number] = '\0';

	return result;
}

char *build_line(Line line) {
	char *result = (char*) calloc(300, 1);
	result[0] = line.left_sep;
	strcat(result, repeat(' ', line.left_pad));
	strcat(result, line.content);
	strcat(result, repeat(' ', line.right_pad));
	strcat(result, &line.right_sep);

	return result;
}

void append_to_line(Line *line, char *new_content) {
	strcat(line->content, new_content);
	line->left_pad -= strlen(new_content);
}

void prepend_to_line(Line *line, char *new_content) {
	char *tmp_string = (char*) calloc(100, 1);
	strcpy(tmp_string, new_content);
	strcat(tmp_string, line->content);
	line->content = tmp_string;
	line->left_pad -= strlen(new_content);
}

void sum() {
	char content[100] = "Inside sum()";
	Line test = new_line('*', 20, content, 10, '*');
	puts(build_line(test));
	prepend_to_line(&test, "German");
	puts(build_line(test));
	append_to_line(&test, "Leonardo");
	puts(build_line(test));
	puts("Inside sum()");
}

int main(void) {
	const char *prompt = "\nIf you don't want to finish the exercise, you can get out entering \"s\". Please enter the operation without any whitespace, letters or special characters.\n\nExample:\n\t12345+6789+9685+12599\n";
	// ****** OJO ESTAS DOS LINEAS QUE SIGUEN HAAY QQUE BORRARLAS, TIENEN QUE SER ARGUMENTOS *****
	FILE *session_pointer = fopen("prueba.txt", "w");
	bool session_backup = true;
	while (1) {
		char *operation = get_user_input_sum(prompt, "+", session_pointer, true);
		size_t operands_size = 0;
		char **operands = split(operation, '+', &operands_size);

		bool wrong_operators = false;
		for (size_t i = 0; i < operands_size; i++) {
			if (strlen(operands[i]) == 0) {
				wrong_operators = true;
				break;
			}
		}

		bool wrong_operands = false;
		if (operands_size == 1) {
			wrong_operands = true;
		}

		char *warning = (char*) calloc(100, 1);
		if (wrong_operators || wrong_operands) {
			sprintf(warning, "\nThere is an error in the quantity of operators and/or operands.\n");
			sleep(1);
			printf("%s", warning);
			if (session_backup) {
				fputs(warning, session_pointer);
			}
			while (getchar() != '\n');
			sleep(1);
			continue;
		}
		break;
	}

	sleep(1);
	sum();
	equal_or_not_sum(23, "\nEnter 23 to exit:\n", session_pointer, true);
}
