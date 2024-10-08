#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>

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
		printf("%s\n", prompt);
		if (session_backup) {
			fprintf(session_pointer, "%s\n", prompt);
		}
	}
	tmp_line = new_line(' ', 15 - line_length, repeat('-', line_length), 5, ' ');
	prompt = build_line(tmp_line);
	puts(prompt);
	if (session_backup) {
		fprintf(session_pointer, "%s\n", prompt);
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
		char prompt2[100];
		sprintf(prompt2, "\nHow much is ");
		for (size_t i = 0; i < operands_size; i++) {
			tmp_total += numbers[i] % 10;
			char tmp_string[20];
			if (i < operands_size - 1) {
				sprintf(tmp_string, "%d + ", numbers[i] % 10);
			} else {
				sprintf(tmp_string, "%d", numbers[i] % 10);
			}
			strcat(prompt2, tmp_string);
			numbers[i] /= 10;
		}
		sleep(1);
		equal_or_not_sum(tmp_total, prompt2, session_pointer, session_backup);
		if (use_carry) {
			sleep(1);
			puts("\nCorrect.");
			if (session_backup) {
				fprintf(session_pointer, "%s\n", "\nCorrect.");
			}
			sleep(1);
			sprintf(prompt2, "And with %d that we carry, how much is it?", carry);
			tmp_total += carry;
			equal_or_not_sum(tmp_total, prompt2, session_pointer, session_backup);
		}
		carry = tmp_total / 10;
		if (total >= 10) {
			sleep(1);
			puts("\nCorrect.");
			if (session_backup) {
				fprintf(session_pointer, "%s\n", "\nCorrect.");
			}

			int tmp_sum = 0;
			for (size_t k = 0; k < operands_size; k++) {
				tmp_sum += numbers[k];
			}
			if (tmp_sum == 0) {
				sleep(1);
				sprintf(prompt, "We put the %d, and we're donde with the exercise.", tmp_total);
				puts(prompt);
				if (session_backup) {
					fprintf(session_pointer, "%s\n", prompt);
				}
				char tmp_prompt[10];
				sprintf(tmp_prompt, "%d", tmp_total);
				prepend_to_line(&result_line, tmp_prompt);
				sleep(1);
				sprintf(prompt, "\n%s%s", build_line(carry_line), "<--- Carry");
				printf("%s\n", prompt);
				if (session_backup) {
					fprintf(session_pointer, "%s\n", prompt);
				}
				display_sum(operands, operands_size, session_pointer, session_backup);
				sprintf(prompt, "%s", build_line(result_line));
				puts(prompt);
				if (session_backup) {
					fprintf(session_pointer, "%s\n", prompt);
				}
				return;
			}
			char tmp_prompt[10];
			sprintf(tmp_prompt, "%d", carry);
			prepend_to_line(&carry_line, tmp_prompt);
			sleep(1);
			sprintf(prompt, "We put the %d and carry %d.", tmp_total % 10, tmp_total / 10);
			puts(prompt);
			if (session_backup) {
				fprintf(session_pointer, "%s\n", prompt);
			}
			sleep(1);
			sprintf(prompt, "Let's continue with the exercise.");
			puts(prompt);
			if (session_backup) {
				fprintf(session_pointer, "%s\n", prompt);
			}
		} else {
			sleep(1);
			sprintf(prompt, "\nCorrect, we're done with the exercise.");
			puts(prompt);
			if (session_backup) {
				fprintf(session_pointer, "%s\n", prompt);
			}
		}
		char tmp_prompt[10];
		sprintf(tmp_prompt, "%d", tmp_total % 10);
		prepend_to_line(&result_line, tmp_prompt);
		sleep(1);
		sprintf(prompt, "\n%s%s", build_line(carry_line), "<--- Carry");
		printf("%s\n", prompt);
		if (session_backup) {
			fprintf(session_pointer, "%s\n", prompt);
		}
		display_sum(operands, operands_size, session_pointer, session_backup);
		sprintf(prompt, "%s", build_line(result_line));
		puts(prompt);
		if (session_backup) {
			fprintf(session_pointer, "%s\n", prompt);
		}
		if (!use_carry) {
			use_carry = true;
		}
		total /= 10;
	}

	return;
}

int main_sum(FILE *session_pointer, bool session_backup) {
	char **operands;
	size_t operands_size = 0;
	const char *prompt = "\nIf you don't want to finish the exercise, you can get out entering \"s\". Please enter the operation without any whitespace, letters or special characters.\n\nExample:\n\t12345+6789+9685+12599\n";
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

		size_t i = 0;
		size_t operator_counter = 0;
		while (operation[i] != '\0') {
			if (operation[i] == '+') {
				operator_counter++;
			}
			i++;
		}

		char *warning = (char*) calloc(100, 1);
		if (wrong_operators || wrong_operands || (operator_counter >= operands_size)) {
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
