#include <string.h>

#define HASHSIZE 101

struct nlist {
	struct nlist* next;
	char* name;
	char* defn;
};

unsigned int hash(char* s);
struct nlist* hash_lookup(struct nlist** tbl, char* s);
struct nlist* hash_install(struct nlist** tbl, char* name, char* defn);
