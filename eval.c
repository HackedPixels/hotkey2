#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __linux__
	#define MOD_ALT 0x01
	#define MOD_CTRL 0x02
#elif _WIN32
	#include <windows.h>

	#define MOD_CTRL MOD_CONTROL
	#define MOD_NOREPEAT 0x4000
#endif

/* TODO: Make this included in the header */
#define MAX_ACTION 2

#define FFW(x) while (*x != '\0' && *x != 0x1f) { x++; } x++

typedef struct header {
	int dsize;
	int version;
	int offset;
} header_t;

typedef struct keycomb {
	int mod;
	char key;
} keycomb_t;

typedef struct action {
	struct keycomb key; // what keycombination
	int action; //what to do
	char* file; // the file to read/write
} action_t;


struct header read_header(FILE* f);
int decode_action(char* msg);
struct keycomb decode_key(char* msg);

int
main(int argc, char** argv)
{
	FILE* f;
	struct header h;
	unsigned char buffer[1024];
	unsigned int count;

	count = 0;
/*
	if (argc != 2) {
		printf("No file given...\n");
		return 1;
	}
	f = fopen(argv[1], "rb");
	*/
	f = fopen("config.comp", "rb");
	h = read_header(f);
#ifdef _WIN32
	printf("__WIN32__\n");
#endif
	printf("VERSION:%d\nCOMPILED_ARCH:%d\nOFFSET:%d\n", h.version, h.dsize, h.offset);
	// main reading
	do {
		char c = '\0';
		unsigned char* buf = buffer;

		while ((c = fgetc(f)) != 0x1F && !feof(f)) {
			*buf = c;
			buf++;
		}
		/* catch last (false) entry */
		if (feof(f)) { break; }

		*buf='\0';
		buf=buffer;

		// decode it :D
		struct action a;
		a.action = decode_action(buf);
		FFW(buf);
		a.key = decode_key(buf);
		FFW(buf);
		FFW(buf);
		printf("a.key.key=%c\na.key.mod=%d\n", a.key.key, a.key.mod);
		a.file = malloc(strlen(buf));
		if (a.file == NULL) { return 1; }
		strcpy(a.file, buf);
		printf("file=%s\n", a.file);
		FFW(buf);
/* TODO: Make this prettier */
#ifdef _WIN32
		if (RegisterHotKey(NULL, count, a.key.mod | MOD_NOREPEAT, a.key.key)) {
			printf("Registered %c with mod %02x as %d.\n", a.key.key, a.key.mod, count);
			count++;
		}
#endif
		free(a.file);
	} while (!feof(f));
	fclose(f);

	printf("GO\n");
#ifdef _WIN32
		/* Poll for events */
		MSG msg = {0};
		while (GetMessage(&msg, NULL, 0, 0) != 0) {
			if (msg.message == WM_HOTKEY) {
				printf("Hotkey pressed: %d\n", msg.wParam);
				fflush(stdout);
			}
		}
#endif

	return 0;
}

struct header
read_header(FILE* f)
{
	struct header h;
	fread(&(h.dsize), 1, 1, f);
	fread(&(h.version), h.dsize, 1, f);
	fread(&(h.offset), h.dsize, 1, f);

	//XXX: read metadata in offset

	fseek(f, h.offset, SEEK_CUR);

	return h;
}

int
decode_action(char* msg)
{
	int action;
	char* buf;

	buf = msg;
	action = (int) (*buf);
	if (action >= 0 && action <= MAX_ACTION) { return action; }

	printf("[ERROR]: Undefined Action.\n");
	return -1;
}

struct keycomb
decode_key(char* msg)
{
	struct keycomb k;
	char* tok;
	char* m;

	k.key = 0;
	k.mod = 0;

	m = msg;

	while ((tok = strtok(m, "+")) != NULL) {
		if (strcmp(tok, "ALT") == 0) {
			k.mod |= MOD_ALT;
		} else if (strcmp(tok, "CTRL") == 0) {
			k.mod |= MOD_CTRL;
		} else if (strlen(tok) == 1) {
			k.key = *tok;
		} else {
			printf("ERROR: unknown key sequence\n");
		}
		m = NULL;
	}

	return k;
}
