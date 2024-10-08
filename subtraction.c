#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

bool contains_subtraction(char *string, char ch) {
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

char *get_user_input_subtraction(const char *prompt, const char *operator, FILE *session_pointer, bool session_backup) {
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
			if (contains_subtraction(string, user_input[i]) == false) {
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

void equal_or_not_subtraction(int tmp_total, const char *prompt, FILE *session_pointer, bool session_backup) {
	while (1) {
		char *user_input = get_user_input_subtraction(prompt, "", session_pointer, session_backup);
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

void display_subtraction(char **operands, size_t operands_size, FILE *session_pointer, bool session_backup) {
	size_t line_length = strlen(operands[0]);
	Line tmp_line = new_line(' ', 15 - strlen(operands[0]), operands[0], 3, '-');
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

// **********  The followimg functions are necessary in the case with more than 1 sustrahend

char *get_user_input_sum_subtraction(const char *prompt, const char *operator, FILE *session_pointer, bool session_backup) {
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
			if (contains_subtraction(string, user_input[i]) == false) {
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

void equal_or_not_sum_subtraction(int tmp_total, const char *prompt, FILE *session_pointer, bool session_backup) {
	while (1) {
		char *user_input = get_user_input_sum_subtraction(prompt, "", session_pointer, session_backup);
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

void display_sum_subtraction(char **operands, size_t operands_size, FILE *session_pointer, bool session_backup) {
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

void sum_subtraction(char **operands, size_t operands_size, FILE *session_pointer, bool session_backup) {
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
	display_sum_subtraction(operands, operands_size, session_pointer, session_backup);
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
		equal_or_not_sum_subtraction(tmp_total, prompt2, session_pointer, session_backup);
		if (use_carry) {
			sleep(1);
			puts("\nCorrect.");
			if (session_backup) {
				fprintf(session_pointer, "%s\n", "\nCorrect.");
			}
			sleep(1);
			sprintf(prompt2, "And with %d that we carry, how much is it?", carry);
			tmp_total += carry;
			equal_or_not_sum_subtraction(tmp_total, prompt2, session_pointer, session_backup);
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
				display_sum_subtraction(operands, operands_size, session_pointer, session_backup);
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
		display_sum_subtraction(operands, operands_size, session_pointer, session_backup);
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

// ***************** Here finishes the functions for more than 1 sustrahend ************************

void modify_minuend(Line *modified_minuend_line, int *minuend, int *counter) {
	int number = *minuend;
	number /= 10;
	char *modified_minuend = (char*) calloc(50, 1);
	while (1) {
		if (number % 10 == 0) {
			*counter += 1;
			prepend_to_line(modified_minuend_line, "9");
			char *tmp_string = (char*) calloc(50, 1);
			sprintf(tmp_string, "%d", 9);
			strcat(tmp_string, modified_minuend);
			modified_minuend = tmp_string;
			number /= 10;
		} else {
			char ch[1];
			sprintf(ch, "%d", (number % 10) - 1);
			prepend_to_line(modified_minuend_line, ch);
			char *tmp_string = (char*) calloc(50, 1);
			sprintf(tmp_string, "%d", (number % 10) - 1);
			strcat(tmp_string, modified_minuend);
			modified_minuend = tmp_string;
			break;
		}
	}

	*minuend = (*minuend / ((int) pow(10.0, (double) (strlen(modified_minuend) + 1))))
			* ((int) pow(10.0, (double) (strlen(modified_minuend) + 1)))
			+ (atoi(modified_minuend) * 10)
			+ (*minuend % 10);
}

void subtraction(char **operands, size_t operands_size, size_t original_length, FILE *session_pointer, bool session_backup) {
	bool use_modified_minuend = false;
	int numbers[2];
	// max_length -> to control the while
	size_t max_length = 0;
	for (size_t i = 0; i < 2; i++) {
		if (strlen(operands[i]) > max_length) {
			max_length = strlen(operands[i]);
		}
		numbers[i] = atoi(operands[i]);
	}
	int total = numbers[0] - numbers[1];
	char *prompt = (char*) calloc(200, 1);
	if (total < 0) {
		sprintf(prompt, "\nThe result of the subtraction is negative.\n");
		printf("%s", prompt);
		if (session_backup) {
			fprintf(session_pointer, "%s", prompt);
		}
		sleep(1);
		sprintf(prompt, "Check the exercise and try again.\n");
		printf("%s", prompt);
		if (session_backup) {
			fprintf(session_pointer, "%s", prompt);
		}
		exit(0);
	}
	Line result_line = new_line(' ', 15, "", 5, ' ');
	Line modified_minuend_line = new_line(' ', 15, "", 5, ' ');
	int modified_minuend_counter = 0;
	sleep(1);
	if (original_length == 2) {
		sprintf(prompt, "\nLet's do the following exercise:\n");
		printf("%s", prompt);
		if (session_backup) {
			fprintf(session_pointer, "%s", prompt);
		}
	} else {
		sprintf(prompt, "\nNow let's continue with the subtraction:\n");
		printf("%s", prompt);
		if (session_backup) {
			fprintf(session_pointer, "%s", prompt);
		}
	}
	sleep(1);
	display_subtraction(operands, operands_size, session_pointer, session_backup);
	sprintf(prompt, "%s", build_line(result_line));
	printf("%s\n", prompt);
	if (session_backup) {
		fprintf(session_pointer, "%s\n", prompt);
	}
	while (max_length > 0) {
		int tmp_total = numbers[0]%10 - numbers[1]%10;
		char *tmp_string = (char*) calloc(50, 1);
		sprintf(tmp_string, "%d-%d", numbers[0]%10, numbers[1]%10);
		if (tmp_total < 0) {
			if (!use_modified_minuend) {
				use_modified_minuend = true;
				prepend_to_line(&modified_minuend_line, " ");
			}
			sleep(1);
			sprintf(prompt, "\nSince %d is less than %d, we borrow from the left and continue:\n", numbers[0]%10, numbers[1]%10);
			printf("%s", prompt);
			if (session_backup) {
				fprintf(session_pointer, "%s", prompt);
			}
			modify_minuend(&modified_minuend_line, &numbers[0], &modified_minuend_counter);
			char *tmp_string2 = (char*) calloc(50, 1);
			sprintf(tmp_string2, "%d", 1);
			strcat(tmp_string2, tmp_string);
			tmp_string = tmp_string2;
			tmp_total += 10;
		} else {
			modified_minuend_counter -= 1;
			if (modified_minuend_counter < 0) {
				prepend_to_line(&modified_minuend_line, " ");
			}
		}
		numbers[0] /= 10;
		numbers[1] /= 10;
		sprintf(prompt, "\nHow much is %s?\n", tmp_string);
		sleep(1);
		equal_or_not_subtraction(tmp_total, prompt, session_pointer, session_backup);
		if (max_length > 1) {
			sleep(1);
			sprintf(prompt, "\nCorrect.\n");
			printf("%s", prompt);
			if (session_backup) {
				fprintf(session_pointer, "%s", prompt);
			}
			sleep(1);
			sprintf(prompt, "\nLet's continue with the exercise.\n\n");
			printf("%s", prompt);
			if (session_backup) {
				fprintf(session_pointer, "%s", prompt);
			}
		} else {
			sleep(1);
			sprintf(prompt, "\nCorrect, we're done with the exercise.\n\n");
			printf("%s", prompt);
			if (session_backup) {
				fprintf(session_pointer, "%s", prompt);
			}
		}
		char *tmp_number = (char*) calloc(1, 1);
		sprintf(tmp_number, "%d", tmp_total % 10);
		prepend_to_line(&result_line, tmp_number);
		sleep(1);
		if (use_modified_minuend) {
			sprintf(prompt, build_line(modified_minuend_line));
			strcat(prompt, "<=== Modified minuend\n");
			printf("%s", prompt);
			if (session_backup) {
				fprintf(session_pointer, "%s", prompt);
			}
		}
		display_subtraction(operands, operands_size, session_pointer, session_backup);
		sprintf(prompt, build_line(result_line));
		printf("%s\n\n", prompt);
		if (session_backup) {
			fprintf(session_pointer, "%s\n\n", prompt);
		}

		max_length -= 1;
	}
}

int main_subtraction(FILE *session_pointer, bool session_backup) {
	char **operands;
	size_t operands_size = 0;
	const char *prompt = "\nIf you don't want to finish the exercise, you can get out entering \"s\". Please enter the operation without any whitespace, letters or special characters.\n\nExample:\n\t12345-6789-9685-12599\n";
	while (1) {
		char *operation = get_user_input_subtraction(prompt, "-", session_pointer, true);
		operands = split(operation, '-', &operands_size);

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
			if (operation[i] == '-') {
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

	size_t original_length = operands_size;
	if (operands_size > 2) {
		sleep(1);
		char prompt2[200];
		sprintf(prompt2, "\nLet's do the following exercise:");
		printf("%s\n", prompt2);
		if (session_backup) {
			fprintf(session_pointer, "%s\n", prompt2);
		}
		sleep(1);
		display_subtraction(operands, operands_size, session_pointer, session_backup);
		sleep(1);
		sprintf(prompt2, "\nBut first let's total the sustrahends");
		printf("%s\n", prompt2);
		if (session_backup) {
			fprintf(session_pointer, "%s\n", prompt2);
		}
		sleep(1);
		char **operands_for_sum = (char**) calloc(operands_size - 1, sizeof(char*));
		int sustrahend = 0;
		for (size_t i = 1; i < operands_size; i++) {
			operands_for_sum[i - 1] = operands[i];
			sustrahend += atoi(operands[i]);
		}
		sum_subtraction(operands_for_sum, operands_size - 1, session_pointer, session_backup);
		operands = (char**) realloc(operands, 2 * sizeof(char*));
		sprintf(operands[1], "%d", sustrahend);
	}
	
	subtraction(operands, 2, original_length, session_pointer, session_backup);
}
