#include <stdio.h>
#include <string.h>

#define MOD_ALT 0x01
#define MOD_CTRL 0x02

#define FFW(x) while (*x != '\0') { x++; } x++

typedef struct header {
	int version;
	int offset;
} header_t;

typedef struct key {
	int mod;
	char key;
} key_t;

struct header read_header(FILE* f);
struct key decode_key(char* msg);

int
main(int argc, char** argv)
{
	FILE* f;
	struct header h;
	char buffer[1024];

	if (argc != 2) {
		printf("No file given\n");
		return 1;
	}
	f = fopen(argv[1], "rb");
	h = read_header(f);

	printf("V:%d\nO:%d\n", h.version, h.offset);
	// main reading
	do {
		char c = '\0';
		char* buf = buffer;

		while (!feof(f) && (c = fgetc(f)) != 0x1F) {
			*buf = c;
			buf++;
		}
		*buf='\0';
		buf=buffer;

		// decode it :D
		struct key k;
		k = decode_key(buf);
		FFW(buf);
		FFW(buf);
		printf("k.key=%c\nk.mod=%d\n", k.key, k.mod);
		printf("%s\n", buf);

	} while (!feof(f));
	fclose(f);

	return 0;
}

struct header
read_header(FILE* f)
{
	struct header h;
	fread(&(h.version), 4, 1, f);
	fread(&(h.offset), 4, 1, f);
	fseek(f, h.offset, SEEK_CUR);

	return h;
}

struct key
decode_key(char* msg)
{
	struct key k;
	char* tok;
	char* m;

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
