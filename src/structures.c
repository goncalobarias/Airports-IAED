/*
 *		File: structures.c
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: All of the data structures used for the 2nd project.
 */

#include <stdlib.h>
#include <string.h>

struct hash_elem_t {
	void* data;
	unsigned int hash_1;
	unsigned int hash_2;
};

struct hashtable_t {
	int size;
	int elem_num;
	hash_elem** table;
};

/*		hashtables		*/

/**
 * Internal function to calculate the hash for a key.
 * It's based on the DJB algorithm.
 */
unsigned int calc_hash(char* key) {
	unsigned int hash = 5381;
	int c;

	while((c = *(key++)) != 0) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}

/**
 * Internal function to calculate the hash step for a key.
 * It's based on the Jenkins hash function.
 */
unsigned int calc_hash_step(char* key) {
	unsigned hash = 0;
	int i = 0;

	while (key[i] != '\0') {
		hash += key[i++];
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;

	return hash;
}

/**
 *
 */
unsigned int hashtable_calc_hashes(hash_elem* elem, char* key, int size) {
	elem->hash_1 = calc_hash(key);
	elem->hash_2 = calc_hash_step(key);

	return (elem->hash_1 % size);
}

/**
 * Creates a hashtable with the specified size and starting information
 * and returns a pointer to it.
 */
hashtable* hashtable_create(int size) {
	int i;
	hashtable* hash_t = (hashtable*)secure_malloc(sizeof(hashtable_t));

	hash_t->table = (hash_elem**)secure_malloc(size * sizeof(hash_elem*));
	hash_t->size = size;
	hash_t->elem_num = 0;

	for(i = 0; i < size; i++) {
		hash_t->table[i] = NULL;
	}

	return hash_t;
}

/**
 *
 */
hashtable* hashtable_insert(hashtable* hash_t, void* data, char* key) {
	hash_elem* elem = {data, 0, 0};
	int h = hashtable_calc_hashes(elem, key, hash_t->size);
	int phi = elem->hash_2 % hash_t->size, i = 1;

	if (phi == 0) {
		phi = 1;
	}

	while (hash_t->table[h] != NULL) {
		h = (elem->hash_1 + i * phi) % hash_t->size;
		i++;
	}
	hash_t->table[h] = elem;

	if (++hash_t->elem_num >= hash_t->size * MAX_LOAD) {
		hash_t = hashtable_expand(hash_t);
	}

	return hash_t;
}

/**
 *
 */
void* hashtable_get(hashtable_t* hash_t, char* key) {
}

/**
 *
 */
void hashtable_remove(hashtable_t* hash_t, char* key) {
}

/**
 *
 */
hashtable* hashtable_expand(hastable* hash_t) {
}

/**
 *
 */
void hashtable_destroy(hashtable* hash_t) {
	for (i = 0; i < hash_t->size; i++) {
		free(hash_t->table[i]);
	}
	free(hash_t->table);
	free(hash_t);
}

/*		utilities		*/

/**
 *
 */
void* secure_malloc(unsigned int allocation) {
	void* ptr = malloc(allocation);

	if (ptr != NULL) {
		return ptr;
	} else {
		printf(MEMORY_ERR);
		exit(MEMORY_ERR_CODE);
	}

	return NULL;
}

/**
 *
 */
int isPrime(int x) {
	for (i = 2; i * i <= x; i++) {
		if (x % i == 0) {
			return 0;
		}
	}

	return 1;
}

/**
 *
 */
int getPrime(int minNum) {
	int prime = minNum;

	while (!isPrime(prime)) {
		prime++;
	}

	return prime;
}
