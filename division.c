#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include "lib.c"

// here starts the definitions of the Division struct
typedef struct {
	Line dividend;
	Line divisor;
	Line *operations;
	size_t operations_counter;
	Line quotient;
} Division;

Division new_division(char **operands, size_t number) {
	Division result;
	Line dividend = new_line(' ', 5, operands[0], 1, '|');
	result.dividend = dividend;
	Line divisor = new_line(' ', 0, operands[1], 0, ' ');
	result.divisor = divisor;
	Line *operations = (Line*) calloc(50, sizeof(Line*));
	operations[0] = new_line(' ', 5, repeat(' ', number), strlen(operands[0]) - number, ' ');
	result.operations = operations;
	result.operations_counter = 1;
	Line quotient = new_line(' ', 0, "", 10, ' ');
	result.quotient = quotient;

	return result;
}

void display_division(Division division, FILE *session_pointer, bool session_backup) {
	char prompt[100];
	sprintf(prompt, "\n%s%s\n", build_line(division.dividend), build_line(division.divisor));
	printf("%s", prompt);
	if (session_backup) {
		fprintf(session_pointer, "%s", prompt);
	}
	sprintf(prompt, "%s|%s\n", repeat(' ', 7 + strlen(division.dividend.content)), repeat('-', 5 + strlen(division.divisor.content)));
	printf("%s", prompt);
	if (session_backup) {
		fprintf(session_pointer, "%s", prompt);
	}
	sprintf(prompt, "%s|%s\n", build_line(division.operations[0]), build_line(division.quotient));
	printf("%s", prompt);
	if (session_backup) {
		fprintf(session_pointer, "%s", prompt);
	}
	if (division.operations_counter > 1) {
		for (size_t j = 1; j < division.operations_counter; j++) {
			sprintf(prompt, "%s\n", build_line(division.operations[j]));
			printf("%s", prompt);
			if (session_backup) {
				fprintf(session_pointer, "%s", prompt);
			}
		}
	}
}

bool contains_division(char *string, char ch) {
	size_t i = 0;
	while (string[i] != '\0') {
		if (string[i] == ch) {
			return true;
		}
		i++;
	}

	return false;
}

char *get_user_input_division(const char *prompt, const char *operator, FILE *session_pointer, bool session_backup) {
	char *user_input = (char*) calloc(50, 1);
	char *prompt2 = (char*) calloc(100, 1);
	while (1) {
		puts(prompt);
		if (session_backup) {
			fprintf(session_pointer, "%s\n", prompt);
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
				fprintf(session_pointer, "%s\n", user_input);
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
			if (contains_division(string, user_input[i]) == false) {
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
		fprintf(session_pointer, "%s\n", user_input);
	}

	return user_input;
}

void equal_or_not_division(int tmp_total, const char *prompt, FILE *session_pointer, bool session_backup) {
	while (1) {
		char *user_input = get_user_input_division(prompt, "", session_pointer, session_backup);
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
/*
void display_division(char **operands, size_t operands_size, FILE *session_pointer, bool session_backup) {
	size_t line_length = strlen(operands[0]);
	Line tmp_line = new_line(' ', 15 - strlen(operands[0]), operands[0], 3, '*');
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
		printf("%s\n", prompt);
		if (session_backup) {
			fprintf(session_pointer, "%s\n", prompt);
		}
	}
	tmp_line = new_line(' ', 15 - line_length, repeat('-', line_length), 5, ' ');
	prompt = build_line(tmp_line);
	printf("%s\n", prompt);
	if (session_backup) {
		fprintf(session_pointer, "%s\n", prompt);
	}
	free(prompt); prompt = NULL;
}
*/
void division(char **operands, size_t operands_size, FILE *session_pointer, bool session_backup) {
	char dividend_vector[strlen(operands[0])];
	// i: to control the number that is used in each iteration
	size_t i = 0;
	for (size_t k = 0; k < strlen(operands[0]); k++) {
		dividend_vector[k] = operands[0][k];
	}
	// number: the partial dividend to be used in each iteration
	char pre_number[1];
    sprintf(pre_number, "%c", dividend_vector[0]);
	int number = atoi(pre_number);
	int divisor = atoi(operands[1]);
	while (number / divisor < 1) {
		i += 1;
		char tmp_string[1];
		sprintf(tmp_string, "%c", dividend_vector[i]);
		number = number * 10 + atoi(tmp_string);
	}
	Division exercise = new_division(operands, i + 1);
	sleep(1);
	char prompt[200];
	sprintf(prompt, "%s", "\nLet's do the following division:\n");
	printf("%s", prompt);
	if (session_backup) {
		fprintf(session_pointer, "%s", prompt);
	}
	sleep(1);
	display_division(exercise, session_pointer, session_backup);
	sleep(1);
	sprintf(prompt, "\nTo begin with the exercise, we need a number greater than or equal to  %d.\n", divisor);
	printf("%s", prompt);
	if (session_backup) {
		fprintf(session_pointer, "%s", prompt);
	}
	sleep(1);
	bool first_iteration = true;
	// spaces: to control the left padding of the dividends
	int spaces = 0;
	while (1) {
		// tmp_quotient: the partial quotient in each iteration
		int tmp_quotient = number / divisor;
		// if tmp_quotient == 0 the residue will be equal to tmp_dividend, and we don't have to modify spaces
		if (tmp_quotient == 0) {
			char actual_dividend[20];
		    sprintf(actual_dividend, "%s", exercise.operations[exercise.operations_counter - 1].content);
			size_t k = 0;
			while (actual_dividend[k] != '\0') {
				if (actual_dividend[k] == ' ') {
					continue;
				} else if (actual_dividend[k] == '0') {
					spaces += 1;
				} else {
					break;
				}
			}
		} else {
			int tmp_number = number;
			while (tmp_number != 0) {
				spaces += 1;
				tmp_number /= 10;
			}
		}

	}
}

int main(/*FILE *session_pointer, bool session_backup*/) {
	// OJO ESTAS DOS LINEAS QUE SIGUEN HAY QUE BORRARLAS, DEBEN SER ARGUMENTOS
	FILE *session_pointer = fopen("prueba.txt", "w");
	bool session_backup = true;

	char **operands;
	size_t operands_size = 0;
	sleep(1);
	const char *prompt = "\nIf you don't want to finish the exercise, you can get out entering \"s\". Please enter the operation without any whitespace, letters or special characters.\n\nExample:\n\t123456/9\n";
	while (1) {
		char *operation = get_user_input_division(prompt, "/", session_pointer, true);
		operands = split(operation, '/', &operands_size);

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

		size_t i = 0;
		size_t operator_counter = 0;
		while (operation[i] != '\0') {
			if (operation[i] == '/') {
				operator_counter++;
			}
			i++;
		}

		char *warning = (char*) calloc(100, 1);
		if (wrong_operators || operator_counter != 1 || operands_size != 2) {
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

	division(operands, operands_size, session_pointer, session_backup);
}
