#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include "lib.c"

bool contains_multiplication(char *string, char ch) {
	size_t i = 0;
	while (string[i] != '\0') {
		if (string[i] == ch) {
			return true;
		}
		i++;
	}

	return false;
}

char *get_user_input_multiplication(const char *prompt, const char *operator, FILE *session_pointer, bool session_backup) {
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
			if (contains_multiplication(string, user_input[i]) == false) {
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

void equal_or_not_multiplication(int tmp_total, const char *prompt, FILE *session_pointer, bool session_backup) {
	while (1) {
		char *user_input = get_user_input_multiplication(prompt, "", session_pointer, session_backup);
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

void display_multiplication(char **operands, size_t operands_size, FILE *session_pointer, bool session_backup) {
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

// **********  The followimg functions are necessary for the final sum  ****************************

char *get_user_input_sum_multiplication(const char *prompt, const char *operator, FILE *session_pointer, bool session_backup) {
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
			if (contains_multiplication(string, user_input[i]) == false) {
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

void equal_or_not_sum_multiplication(int tmp_total, const char *prompt, FILE *session_pointer, bool session_backup) {
	while (1) {
		char *user_input = get_user_input_sum_multiplication(prompt, "", session_pointer, session_backup);
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

void display_sum_multiplication(char **operands, size_t operands_size, FILE *session_pointer, bool session_backup) {
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
		fprintf(session_pointer, "\n%s\n", prompt);
	}
	free(prompt); prompt = NULL;
}

void sum_multiplication(char **operands, size_t operands_size, FILE *session_pointer, bool session_backup) {
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
	display_sum_multiplication(operands, operands_size, session_pointer, session_backup);
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
		equal_or_not_sum_multiplication(tmp_total, prompt2, session_pointer, session_backup);
		if (use_carry) {
			sleep(1);
			puts("\nCorrect.");
			if (session_backup) {
				fprintf(session_pointer, "%s\n", "\nCorrect.");
			}
			sleep(1);
			sprintf(prompt2, "And with %d that we carry, how much is it?", carry);
			tmp_total += carry;
			equal_or_not_sum_multiplication(tmp_total, prompt2, session_pointer, session_backup);
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
				puts(prompt);
				if (session_backup) {
					fputs(prompt, session_pointer);
				}
				display_sum_multiplication(operands, operands_size, session_pointer, session_backup);
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
		puts(prompt);
		if (session_backup) {
			fputs(prompt, session_pointer);
		}
		display_sum_multiplication(operands, operands_size, session_pointer, session_backup);
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

// ***************** Here finishes the functions for the final sum ************************

void multiplication(char **operands, size_t operands_size, FILE *session_pointer, bool session_backup) {
	int numbers[2];
	numbers[0] = atoi(operands[0]);
	numbers[1] = atoi(operands[1]);
	size_t second_operand_length = strlen(operands[1]);
	size_t counter = 0;
	Line result_line = new_line(' ', 15, "", 5, ' ');
	Line *result_vector = (Line*) calloc(second_operand_length, sizeof(Line));
	size_t result_vector_size = 0;
	Line carry_line = new_line(' ', 15, "", 5, ' ');
	int carry_value = ((numbers[0]%10) * (numbers[1]%10)) / 10;
	prepend_to_line(&carry_line, " ");
	sleep(1);
	char prompt[300];
	sprintf(prompt, "%s", "\nLet's do the following exercise:\n");
	printf("%s", prompt);
	if (session_backup) {
		fprintf(session_pointer, "%s", prompt);
	}
	sleep(1);
	display_multiplication(operands, 2, session_pointer, session_backup);
	sprintf(prompt, "%s", build_line(result_line));
	printf("%s\n", prompt);
	if (session_backup) {
		fprintf(session_pointer, "%s\n", prompt);
	}
	while (second_operand_length - counter != 0) {
		bool use_carry = false;
		int fixed = numbers[1] % 10;
		int first_operand = numbers[0];
		while (first_operand != 0) {
			int tmp_total = (first_operand % 10) * fixed;
			char tmp_string[100];
			sprintf(tmp_string, "%d*%d", first_operand % 10, fixed);
			first_operand /= 10;
			sprintf(prompt, "\nHow much is %s?", tmp_string);
			sleep(1);
			equal_or_not_multiplication(tmp_total, prompt, session_pointer, session_backup);
			if (use_carry) {
				sleep(1);
				printf("%s", "\nCorrect.\n");
				if (session_backup) {
					fprintf(session_pointer, "%s", "\nCorrect.\n");
				}
				sleep(1);
				sprintf(prompt, "And with %d that we carry how much is it?", carry_value);
				tmp_total += carry_value;
				equal_or_not_multiplication(tmp_total, prompt, session_pointer, session_backup);
			}
			carry_value = tmp_total / 10;
			if (!(second_operand_length-counter == 0 && first_operand == 0)) {
				char tmp_value[10];
				sprintf(tmp_value, "%d", carry_value);
				prepend_to_line(&carry_line, tmp_value);
				sleep(1);
				printf("%s", "\nCorrect.\n");
				if (session_backup) {
					fprintf(session_pointer, "%s", "\nCorrect.\n");
				}
				sleep(1);
                if (first_operand != 0) {
                    sprintf(prompt, "We put the %d and carry %d.\n", tmp_total % 10, tmp_total / 10);
                    printf("%s", prompt);
                    if (session_backup) {
                        fprintf(session_pointer, "%s", prompt);
                    }
                } else {
                    sprintf(prompt, "We put the %d, and continue with multiplications corresponding to the following number: %d.\n", tmp_total, (numbers[0] / 10) % 10);
                    printf("%s", prompt);
                    if (session_backup) {
                        fprintf(session_pointer, "%s", prompt);
                    }
                }
                sleep(1);
                sprintf(prompt, "%s", "Let's continue with the exercise.\n");
                printf("%s", prompt);
                if (session_backup) {
                    fprintf(session_pointer, "%s", prompt);
                }
			} else {
				sleep(1);
				sprintf(prompt, "%s", "Correct, we have finished with the multiplications.\n");
				printf("%s", prompt);
				if (session_pointer) {
					fprintf(session_pointer, "%s", prompt);
				}
            }
			sleep(1);
			if (first_operand != 0) {
				sprintf(prompt, "\n%s%s\n", build_line(carry_line), "<--- Carry");
				printf("%s", prompt);
				if (session_backup) {
					fprintf(session_pointer, "%s", prompt);
				}
				char tmp_string[1];
				sprintf(tmp_string, "%d", tmp_total % 10);
				prepend_to_line(&result_line, tmp_string);
			} else {
				char tmp_string[2];
				sprintf(tmp_string, "%d", tmp_total);
				prepend_to_line(&result_line, tmp_string);
			}
			display_multiplication(operands, operands_size, session_pointer, session_backup);
			if (result_vector_size != 0) {
				for (size_t i = 0; i < result_vector_size; i++) {
					sprintf(prompt, "%s\n", build_line(result_vector[i]));
					printf("%s", prompt);
					if (session_backup) {
						fprintf(session_pointer, "%s", prompt);
					}
				}
			}
			sprintf(prompt, "%s\n", build_line(result_line));
			printf("%s", prompt);
			if (session_backup) {
				fprintf(session_pointer, "%s", prompt);
			}
			use_carry = true;
		}
		
		result_vector[result_vector_size] = result_line;
		result_vector_size++;
		result_line = new_line(' ', 15, "", 5, ' ');
		carry_line = new_line(' ', 15, "", 5, ' ');
		prepend_to_line(&carry_line, " ");
		counter += 1;
		for (size_t i = 0; i < counter; i++) {
			prepend_to_line(&result_line, " ");
		}
		numbers[1] /= 10;
	}
	
	sleep(1);
	if (result_vector_size == 1) {
		printf("%s", "\nWe've finished with the exercise.\n");
		if (session_backup) {
			fprintf(session_pointer, "%s", "\nWe've finished with the exercise.\n");
		}
		exit(0);
	}

	char **sum_operands = (char**) calloc(result_vector_size, sizeof(char*));
	for (size_t i = 0; i < result_vector_size; i++) {
		char *tmp_string = (char*) calloc(50, 1);
		tmp_string = result_vector[i].content;
		size_t k = 0;
		while (tmp_string[k] != '\0') {
			if (tmp_string[k] == ' ') {
				tmp_string[k] == '0';
			}
			k++;
		}
		sum_operands[i] = tmp_string;
	}

	sprintf(prompt, "%s", "\nNow we have to sum all the partial results.'n");
	printf("%s", prompt);
	if (session_backup) {
		fprintf(session_pointer, "%s", prompt);
	}
	sum_multiplication(sum_operands, result_vector_size, session_pointer, session_backup);
	


// *********** OJO LLEGAMOS HASTA LA LINEA 196 EN RUST/multiplication_rust/lib.rs ***********
}

int main(/*FILE *session_pointer, bool session_backup*/) {
	// OJO ESTAS DOS LINEAS QUE SIGUEN HAY QUE BORRARLAS, TIENEN QUE SER ARGUMENTOS
	FILE *session_pointer = fopen("prueba.txt", "w");
	bool session_backup = true;


	char **operands;
	size_t operands_size = 0;
	sleep(1);
	const char *prompt = "\nIf you don't want to finish the exercise, you can get out entering \"s\". Please enter the operation without any whitespace, letters or special characters.\n\nExample:\n\t12345*968\n";
	while (1) {
		char *operation = get_user_input_multiplication(prompt, "*", session_pointer, true);
		operands = split(operation, '*', &operands_size);

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
			if (operation[i] == '*') {
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

	multiplication(operands, operands_size, session_pointer, session_backup);
}
