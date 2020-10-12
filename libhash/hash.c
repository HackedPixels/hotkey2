#include "hash.h"

#define HASHSIZE 101

unsigned int
hash(char* s)
{
	unsigned int hashval;
	for (hashval = 0; *s != '\0'; s++) {
		hashval = *s + 31 * hashval;
	}
	return hashval % HASHSIZE;
}

struct nlist*
hash_lookup(struct nlist** tbl, char* s)
{
	struct nlist* np;

	for (np = tbl[hash(s)]; np != NULL; np = np->next) {
		if (strcmp(s, np->name) == 0) {
			return np;
		}
	}
	return NULL;
}

struct nlist*
hash_install(struct nlist** tbl, char* name, char* defn)
{
	struct nlist* np;
	unsigned int hashval;

	if ((np = hash_lookup(tbl, name)) == NULL) { /* not found */
		np = (struct nlist*) (malloc(sizeof(*np)));
		// could not allocate np OR strdup on name failed
		if (np == NULL || (np->name = strdup(name)) == NULL) {
			return NULL;
		}
		hashval = hash(name);
		np->next = tbl[hashval];
		tbl[hashval] = np;
	} else {
		free((void*) np->defn);
	}

	if ((np->defn = strdup(defn)) == NULL) {
		return NULL;
	}

	return np;
}
