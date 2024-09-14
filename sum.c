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

// ********* DEFINITIONS OF LINE STRUCT AND ITS ASSOCIATED FUNCTIONS **************

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
	size_t content_size = strlen(line.content);
	size_t total_len = 1 + line.left_pad + content_size + line.right_pad + 2;
	char *result = (char*) calloc(total_len, 1);
	result[0] = line.left_sep;
	strcat(result, repeat(' ', line.left_pad));
	strcat(result, line.content);
	strcat(result, repeat(' ', line.right_pad));
	strcat(result, &line.right_sep);
	result[total_len - 1] = '\0';

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

// *************** HERE FINISHES THE DECLARATIONS OF THE STRUCT LINE ************************

void display_sum(char **operands, size_t operands_size, FILE *session_pointer, bool session_backup) {
	size_t line_length = strlen(operands[0]);
	Line tmp_line = new_line(' ', 15 - strlen(operands[0]), operands[0], 3, '+');
	char *prompt = build_line(tmp_line);
	printf("\n%s\n", prompt);
	if (session_backup) {
		fprintf(session_pointer, "\n%s\n", prompt);
	}
	for (size_t i = 1; i < operands_size; i++) {
		if (strlen(operands[i]) > line_length) {
			line_length = strlen(operands[i]);
		}
		tmp_line = new_line(' ', 15 - strlen(operands[i]), operands[i], 5, ' ');
	    prompt = build_line(tmp_line);
		puts(prompt);
		if (session_backup) {
			fputs(prompt, session_pointer);
		}
	}
	tmp_line = new_line(' ', 15 - line_length, repeat('-', line_length), 5, ' ');
	prompt = build_line(tmp_line);
	puts(prompt);
	if (session_backup) {
		fputs(prompt, session_pointer);
	}
	free(prompt); prompt = NULL;
}

void sum(char **operands, size_t operands_size, FILE *session_pointer, bool session_backup) {
	bool use_carry = false;
	unsigned int total = 0;
	for (size_t i = 0; i < operands_size; i++) {
		total += atoi(operands[i]);
	}
	Line result_line = new_line(' ', 15, "", 5, ' ');
	Line carry_line = new_line(' ', 14, "", 5, ' ');
	char prompt[200];
	sprintf(prompt, "\nLet's do the following exercise:\n");
	puts(prompt);
	if (session_backup) {
		fputs(prompt, session_pointer);
	}
	sleep(1);
	display_sum(operands, operands_size, session_pointer, session_backup);
	printf("%s\n", build_line(result_line));
	if (session_backup) {
		fprintf(session_pointer, "%s\n", build_line(result_line));
	}

	int numbers[operands_size];
	for (size_t i = 0; i < operands_size; i++) {
		numbers[i] = atoi(operands[i]);
	}

	int carry = 0;
	while (total != 0) {
		int tmp_total = 0;

	}
}

int main(void) {
	char **operands;
	size_t operands_size = 0;
	const char *prompt = "\nIf you don't want to finish the exercise, you can get out entering \"s\". Please enter the operation without any whitespace, letters or special characters.\n\nExample:\n\t12345+6789+9685+12599\n";
	// ****** OJO ESTAS DOS LINEAS QUE SIGUEN HAAY QQUE BORRARLAS, TIENEN QUE SER ARGUMENTOS *****
	FILE *session_pointer = fopen("prueba.txt", "w");
	bool session_backup = true;
	while (1) {
		char *operation = get_user_input_sum(prompt, "+", session_pointer, true);
		operands = split(operation, '+', &operands_size);

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
	sum(operands, operands_size, session_pointer, session_backup);
}
