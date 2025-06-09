#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

bool string_delimiter_find(const char* string, size_t string_length, const char* delimiter, size_t delimiter_length,
	size_t start_index, size_t* delimiter_startindex, size_t* delimiter_endindex) {
	if(start_index + delimiter_length > string_length) {
		return false;
	}
	bool found = false;
	size_t j = 0;
	for(size_t i=start_index;i<start_index+delimiter_length;i++) {
		if(string[i] != delimiter[j]) {
			return false;
		}
		j++;
	}
	*delimiter_startindex = start_index, *delimiter_endindex = start_index+delimiter_length;
	return true;
}

void string_appendchar(char** string, char character, size_t* string_bufferlen) {
	char* new_ptr = NULL;
	size_t old_size = 0, new_size = 0;
	if(!(*string)) {
		new_ptr = (char*)calloc(2, sizeof(char));
	} else {
		old_size = *string_bufferlen;
		new_ptr = (char*)realloc(*string, old_size+2);
		new_size = old_size;
	}
	if(!new_ptr) {
		return;
	}
	new_ptr[new_size++] = character;
	new_ptr[new_size] = '\0';
	*string = new_ptr;
	*string_bufferlen = new_size;
}

void string_list_addstring(char*** string_list, char* string_bufferptr, size_t* string_count) {
	char** new_listptr = NULL;
	size_t new_listcount = 0;
	if(!(*string_list)) {
		new_listptr = (char**)malloc(sizeof(char*));
	} else {
		new_listcount = *string_count;
		new_listptr = (char**)realloc(*string_list , (new_listcount+1) * sizeof(char*));
	}
	if(!new_listptr) {
		return;
	}
	new_listptr[new_listcount++] = string_bufferptr;
	*string_list = new_listptr;
	*string_count = new_listcount;
}

char** string_splitbydelim(const char* string, size_t string_length, const char* delimiter,
	size_t delimiter_length, size_t* split_countptr) {
	char** string_list = NULL, *string_buffer = NULL;
	size_t split_count = 0, string_bufferlen = 0;
	for(size_t i=0;i<string_length;i++) {
		size_t start_delimindex = 0, end_delimindex = 0;
		bool found = string_delimiter_find(string, string_length, delimiter,
				delimiter_length, i, &start_delimindex, &end_delimindex);
		if(found) {
			string_list_addstring(&string_list, string_buffer, &split_count);
			string_buffer = NULL;
			i = end_delimindex-1;
			continue;
		}
		string_appendchar(&string_buffer, string[i], &string_bufferlen);
	}
	if(string_buffer) {
		string_list_addstring(&string_list, string_buffer, &split_count);
		string_buffer = NULL, string_bufferlen = 0;
	}
	*split_countptr = split_count;
	return string_list;
}

void print_string_list(char** string_list, size_t string_count) {
	printf("[");
	for(size_t i=0;i<string_count;i++) {
		printf("\"%s\"", string_list[i]);
		if(i != string_count-1) {
			printf(", ");
		}
	}
	printf("]\n");
}

void destroy_string_list(char*** string_list, size_t* string_count) {
	for(size_t i=0;i<*string_count;i++) {
		free((*string_list)[i]);
	}
	free(*string_list); *string_list = NULL;
	*string_count = 0;
}

void usage(char* program_name) {
	fprintf(stderr, "Usage: %s [STRINGTOSPLIT]:[DELIMITERSTRING]...\n"
			"String Slicer is a command-line tool that splits a string into substrings using a delimiter, inspired by Python’s split()\n", program_name);
}

char** parse_commandline_argument(char* cmd_argument, size_t* parsed_resultcount) {
	char** parsed_result = NULL, *string_buffer = NULL;
	bool found_delimiterchar = false;
	size_t cmd_argumentlen = strlen(cmd_argument), string_bufferlen = 0;
	for(size_t i=0;i<cmd_argumentlen;i++) {
		if(cmd_argument[i] == ':' && !found_delimiterchar) {
			if(i != cmd_argumentlen-1) {
				string_list_addstring(&parsed_result, string_buffer, parsed_resultcount);
			} else {
				printf("No delimiter specified for argument string \033[32m\"%s\"\033[0m. Therefore cannot split\n", string_buffer);
				destroy_string_list(&parsed_result, parsed_resultcount);
				return NULL;
			}
			string_buffer = NULL;
			found_delimiterchar = true;
			continue;
		}
		string_appendchar(&string_buffer, cmd_argument[i], &string_bufferlen);
	}
	if(!found_delimiterchar) {
		return NULL;
	}
	if(string_buffer) {
		string_list_addstring(&parsed_result, string_buffer, parsed_resultcount);
		string_buffer = NULL;
	}
	return parsed_result;
}

void print_parsed_result(char** parsed_result) {
	if(!parsed_result) {
		return;
	}
	printf("[\033[32m\"String to split\"\033[0m: \"%s\", \033[32m\"String to split by\"\033[0m: \"%s\"]\n", parsed_result[0], parsed_result[1]);
}

int main(int argc, char** argv) {
	if(argc < 2) {
		usage(argv[0]);
		return -1;
	}
	for(int i=1;i<argc;i++) {
		printf("\033[34m[\033[32m*\033[34m]\033[0m Performing split sequence for argument \033[32m%d\033[0m of \033[32m%d -> \033[31m\"%s\"\033[0m:\n", i, argc-1, argv[i]);
		size_t parsed_resultcount = 0, split_count = 0,
		       string_len = 0, delimiter_len = 0;
		char **parsed_result = parse_commandline_argument(argv[i], &parsed_resultcount),
		     **splitted_strings = NULL, *string = NULL, *delimiter = NULL;;
		print_parsed_result(parsed_result);
		if(parsed_result) {
			// Initialize the string pointers for the string we want to split and the delimiter
			string = parsed_result[0], delimiter = parsed_result[1];
			string_len = strlen(string), delimiter_len = strlen(delimiter);

			splitted_strings = string_splitbydelim(string, string_len, delimiter, delimiter_len, &split_count);
			print_string_list(splitted_strings, split_count);
			destroy_string_list(&splitted_strings, &split_count);
		}
		destroy_string_list(&parsed_result, &parsed_resultcount);
	}
	return 0;
}
