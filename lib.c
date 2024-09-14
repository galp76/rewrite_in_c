#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>

// receives a prompt and a max length (size) for the buffer that will be returned
char *get_user_input(const char *prompt, size_t size) {
	char *buffer = (char*) calloc(size, 1);
	while (1) {
		fprintf(stdout, "%s", prompt);
		int counter = fscanf(stdin, "%s", buffer);
		if (counter != 1) {
			puts("There was an error reading input.\n");
			sleep(1);
			continue;
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
	char *password = get_user_input("\nEnter the password: ", 10);
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

// receives the session options and the boundaries of the return value
size_t get_user_option(FILE *session_pointer, bool session_backup) {
	int option = 0;
	char user_string[100];
	while (1) {
		int counter = fscanf(stdin, "%s", user_string);
		while (getchar() != '\n');
		fprintf(session_pointer, "%s\n", user_string);
		option = atoi(user_string);
		char *prompt = (char*) calloc(100, 1);
		// check for error reading the option from stdin
		if (counter != 1) {
			prompt = "There was an error reading input.\n";
			puts(prompt);
			if (session_backup) {
				fputs(prompt, session_pointer);
			}
			return 0;
			sleep(1);
			prompt = "Enter your option again:";
			puts(prompt);
			if (session_backup) {
				fputs(prompt, session_pointer);
			}
			continue;
		} 
		break;
	}

	return option;
}

//   *********  AQUI EMPIEZAN LAS FUNCIONES UTILIZADAS EN PRACTICE.C   **************

void display_options(char **options, size_t options_size, bool display_prompt, FILE *session_pointer, bool session_backup) {
	char *prompt = (char*) calloc(100, 1);
	if (display_prompt) {
		sprintf(prompt, "\nPlease type one of the options and press ENTER:\n\n");
		printf("%s", prompt);
		if (session_backup) {
			fputs(prompt, session_pointer);
		}
	}
	for (size_t i = 0; i < options_size; i++) {
		if (i < 9) {
			sprintf(prompt, "  %d. %s\n", i + 1, options[i]);
		} else {
			sprintf(prompt, " %d. %s\n", i + 1, options[i]);
		}
		printf("%s", prompt);
		if (session_backup) {
			fputs(prompt, session_pointer);
		}
	}
	sprintf(prompt, "\nOption:\n");
	printf("%s", prompt);
	if (session_backup) {
		fputs(prompt, session_pointer);
	}
	free(prompt); prompt = NULL;
}
