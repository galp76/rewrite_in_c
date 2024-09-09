#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>

// receives a prompt and a max length (size) for the buffer that will be returned
char *get_user_input(const char *prompt, size_t size) {
	char *buffer = (char*) calloc(size, 1);
	while (1) {
		fprintf(stdout, "%s", prompt);
		fscanf(stdin, "%s", buffer);
		if (buffer == NULL) {
			puts("There was an error reading input.\n");
		} else {
			break;
		}
	}

	return buffer;
}

// receives a file path and returns the file content as a string
// similar to std::fs::read_to_string in Rust
char *readToString(const char *filename) {
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
	puts("\nNo fue posible abrir el archivo.\n");
	exit(1);
    }

    if (fseek(fptr, 0, SEEK_END) != 0) {
	fputs("\nNo fue posible buscar el final del archivo.\n", stderr);
	exit(2);
    }
    long fileSize = ftell(fptr);
    rewind(fptr);
    char *content = (char*) calloc(fileSize, 1);
    fread(content, 1, fileSize - 1, fptr);
    fclose(fptr);

    return content;
}

// receives a string and a separator and returns an array of strings
char **split(const char *string, char separator, size_t *answer_size) {
    size_t len = 0;
    size_t separatorCounter = 0;
    while (string[len] != '\0') {
		len++;
		if (string[len] == separator) {
			separatorCounter++;
		}
    }
    len--; // para ajustar len a la longitud de string sin contar el caracter '\0'

    if (string[len] != separator) {
		separatorCounter++;
    }

    // definimos el array de pointers con 'separatorCounter' elementos
    char **result = (char**) calloc(separatorCounter, sizeof(char*));
    *answer_size = separatorCounter;
    
    // empezamos a separar los elementos y a incluirlos en result
    len = 0;
    size_t resultIndex = 0; // contador sobre la posicion actual de result
    char actualElement[1000];
    size_t actualCounter = 0; // contador sobre la posicion actual de actualElement
    while (string[len] != '\0') {
		if (string[len] == separator) {
			actualElement[actualCounter] = '\0';
			char *tmpElement = (char*) calloc(actualCounter, 1);
			strcpy(tmpElement, actualElement);
			actualCounter = 0;
			result[resultIndex] = tmpElement;
			resultIndex++;
		} else {
			actualElement[actualCounter] = string[len];
			actualCounter++;
		}
		len++;
    }

    // en caso de que quede un ultimo elemento sin incluir
    if (string[len - 1] != separator) {
		actualElement[actualCounter] = '\0';
		char *tmpElement = (char*) calloc(actualCounter, 1);
		strcpy(tmpElement, actualElement);
		actualCounter = 0;
		result[resultIndex] = tmpElement;
    }

    return result;
}

// receives a file path, and a separator, returns an array of strings with the content
char **file_to_array_of_lines(const char *filename, char separator, size_t *resultSize) {
    char *string = readToString(filename);
    size_t len = 0;
    size_t separatorCounter = 0;
    while (string[len] != '\0') {
		len++;
		if (string[len] == separator) {
			separatorCounter++;
		}
    }
    len--; // para ajustar len a la longitud de string sin contar el caracter '\0'

    if (string[len] != separator) {
		separatorCounter++;
    }

    // definimos el array de pointers con 'separatorCounter' elementos
    char **result = (char**) calloc(separatorCounter, sizeof(char*));
    *resultSize = separatorCounter;
    
    // empezamos a separar los elementos y a incluirlos en result
    len = 0;
    size_t resultIndex = 0; // contador sobre la posicion actual de result
    char actualElement[1000];
    size_t actualCounter = 0; // contador sobre la posicion actual de actualElement
    while (string[len] != '\0') {
		if (string[len] == separator) {
			actualElement[actualCounter] = '\0';
			char *tmpElement = (char*) calloc(actualCounter, 1);
			strcpy(tmpElement, actualElement);
			actualCounter = 0;
			result[resultIndex] = tmpElement;
			resultIndex++;
		} else {
			actualElement[actualCounter] = string[len];
			actualCounter++;
		}
		len++;
    }

    // en caso de que quede un ultimo elemento sin incluir
    if (string[len - 1] != separator) {
		actualElement[actualCounter] = '\0';
		char *tmpElement = (char*) calloc(actualCounter, 1);
		strcpy(tmpElement, actualElement);
		actualCounter = 0;
		result[resultIndex] = tmpElement;
    }

    return result;
}

// validates a user using the information from users.txt
char *validate_user(void) {
	char *user = get_user_input("Enter the username: ", 10);
	char *password = get_user_input("Enter the password: ", 10);
	size_t users_lines_size = 0;
	char **users_lines = file_to_array_of_lines("users.txt", '\n', &users_lines_size);
	for (size_t i = 0; i < users_lines_size; i++) {
		size_t parts_size = 0;
		char **parts = split(users_lines[i], ';', &parts_size);
		if ((strcmp(user, parts[0]) == 0) && (strcmp(password, parts[1]) == 0)) {
			return user;
		}
	}

	return NULL;
}

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
	char *session = (char*) calloc(100, 1);
	int session_check = 0;
	if (session_backup) {
		session_check = snprintf(session, 100, "users/%s/sessions/%d-%02d-%02d_%02d:%02d", user, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);
		if (session_check < 0 || session_check > 100) {
			puts("An error occurs trying to create the session file.\n");
			exit(2);
		// OJO ESTE ELSE HAY QUE BORRARLO
		} else {
			printf("%s\n", session);
		}
	}
}
