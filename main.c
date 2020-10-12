#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "libini/ini.h"
//#include "libhash/hash.h"

#ifndef bool
	#define bool char
	#define true 1
	#define false 0
#endif

#define MAX_FILEPATH_LEN 256
#define MAX_SECTION_NAME 128
#define MAX_KEYBIND_NAME 16
#define MAX_KEYBINDS     101

#define MATCHN(sp, s, n) strncmp(sp, s, n) == 0
#define MATCH(sp, s) strcmp(sp, s) == 0


enum actions {
	UNDEF = 0, DEC, INC
};

enum arg_flags {
	FLAG_KEY = 1,
	FLAG_FILE = 2,
	FLAG_ACTION = 4
};

struct handler_args {
	char* key;
	char* file;
	int action;
	char* section_name; // helper variable
	char* file_name;
	FILE* output_file;
	int flags;
};

static int handler(void* user, const char* section, const char* name, const char* value);
int writeout(struct handler_args* args);

const int VERSION = 0x01;
const int OFFSET  = 0x03;

int
main(int argc, char** argv)
{
	if (argc != 2) {
		printf("No file name given\n");
		return 1;
	}
	struct handler_args config;
	config.section_name = malloc(MAX_SECTION_NAME); // max macro name
	config.key          = malloc(MAX_KEYBINDS); //max keycombo
	config.file         = malloc(MAX_FILEPATH_LEN);
	config.file_name    = malloc(MAX_FILEPATH_LEN);

	if (config.section_name == NULL) {
		printf("Could not allocate buffer\n");
		return 1;
	}
	*config.section_name = '\0';

	if (ini_parse(argv[1], handler, &config) < 0) {
		printf("Can't load 'config.ini'\n");
		return 1;
	}
	writeout(&config);

	if (config.output_file != -1) {
		printf("written to: %s\n", config.file_name);
		fclose(config.output_file);
	}

	free(config.section_name);
	free(config.key);
	free(config.file);
	free(config.file_name);

	return 0;
}

int
writeout(struct handler_args* args)
{
	char buffer[1024];

	int len = strlen(args->file)+strlen(args->key)+5;
	snprintf(buffer, len, "%c%c%s%c%s%c", (char) args->action, '\0', args->key, '\0', args->file, 0x1F);

	// clear config
	memset(args->key, 0, MAX_KEYBIND_NAME);
	memset(args->file, 0, MAX_FILEPATH_LEN);
	return fwrite(buffer, 1, len, args->output_file);
}

static int
handler(void* dstruct, const char* section, const char* name, const char* value)
{
	struct handler_args* args;
	char buffer[1024];

	args = (struct handler_args*) dstruct;


	memset(buffer, 0, 1024);
	if (strcmp(args->section_name, section) == 0) {
	} else if(args->flags == 7 && !MATCH(section, "output-file")) {
//		printf("%s => %s => %x\n", args->key, args->file, args->action);
		// we have reached a new bind. write config
		printf("WRITING..\n");
		writeout(args);
		strcpy(args->section_name, section);
		args->flags = 0;
	}

	// configure new keybind
	if (MATCHN(section, "bind", 4)) {
		if (MATCH(name, "key")) {
			char* key_tok;
			char* buf = buffer;
			char* tval = (char*) value;

			while ((key_tok = strtok(tval, ",")) != NULL) {
				buf = strcat(buf, key_tok);
				buf = strcat(buf, "+");
				tval = NULL;
			}
			buffer[strlen(buf)-1] = '\0';
			strcpy(args->key, buffer);
			memset(buffer, 0, 1024);
//			printf("BIND: %s\n", buffer);
			args->flags |= FLAG_KEY;
		}

		if (MATCH(name, "file")) {
			strcpy(args->file, value);
//			printf("FILE: %s\n", args->file);
			args->flags |= FLAG_FILE;
		}

		if (MATCH(name, "action")) {
			if (MATCH(value, "INC")) {
				args->action = INC;
			} else if (MATCH(value, "DEC")) {
				args->action = DEC;
			} else {
				args->action = UNDEF;
				printf("unrecognized action: %s\n", value);
			}
			args->flags |= FLAG_ACTION;
		}
	} else if (MATCH(section, "output-file")) {
		if (MATCH(name, "file")) {
			strcpy(args->file_name, value);
			args->output_file = fopen(value, "wb");

			
			// warning: this is unsafe
			// TODO: Fix this, pleb.
			fwrite(&VERSION, sizeof(int), 1, args->output_file);
			fwrite(&OFFSET, sizeof(int), 1, args->output_file);
			return 2;
		}
	}
}
