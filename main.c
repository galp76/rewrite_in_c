#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>
#include "lib.c"

int main(void) {
	char *user = validate_user();
	if (user != NULL) {
		fprintf(stdout, "User %s validated.\n", user);
	} else {
		puts("The user could not be validated.\n");
		exit(0);
	}

	time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	if ((tm.tm_year + 1900 > 2024 ) || (tm.tm_mon + 1 > 10)) {
		puts("Your trial period has expired.");
		exit(1);
	}

	if (strcmp(user, "admin") == 0) {
		puts("Let's continue to the users administration module");
		// OJO HAY QUE IMPLEMENTAR ESTE MODULO
		exit(0);
	}

	bool session_backup = false;
	size_t config_lines_size = 0;
	char **config_lines = file_to_array_of_lines("configuration.txt", '\n', &config_lines_size);
	for (size_t i = 0; i < config_lines_size; i++) {
		size_t parts_size = 0;
		char **parts = split(config_lines[i], '=', &parts_size);
		if ((strcmp(parts[0], "session_backup") == 0) && (strcmp(parts[1], "true") == 0)) {
			session_backup = true;
			break;
		}
	}

	// if session_backup is set to true in configuration.txt, proceed to create the file to store the interactions of the session
	char *session_string = (char*) calloc(100, 1);
	int session_check = 0;
	if (session_backup) {
		session_check = snprintf(session_string, 100, "users/%s/sessions/%d-%02d-%02d_%02d:%02d.txt", user, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);
		if (session_check < 0 || session_check > 100) {
			puts("An error occurs trying to create the session file.\n");
			exit(2);
		// OJO ESTE ELSE HAY QUE BORRARLO
		}
	}
	FILE *session_pointer = fopen(session_string, "w");
	//check for error creating the file
	if (session_pointer == NULL) {
		puts("An error occurs trying to create the session file.\n");
		exit(2);
	}

	// ask the user for an option to redirect to the proper module
	const char *prompt = "\nPor favor indica una de las siguientes opciones:\n\n  1. Práctica.\n  2. Tarea.\n  3. Salir del sistema.\n\nOpción:\n";
	size_t user_option = 0;
	do {
		printf("%s", prompt);
		if (session_backup) {
			fputs(prompt, session_pointer);
		}
		user_option = get_user_option(session_pointer, session_backup, 1, 3);
		printf("You selected option %d.\n", user_option);
	} while (user_option == 0);
}
