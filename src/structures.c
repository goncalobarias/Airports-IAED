/*
 *		File: structures.c
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: All of the data structures used for the 2nd project.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

struct hash_elem_t {
	void* data;
	unsigned int hash_1, hash_2;
	int state;
};

struct hashtable_t {
	int size;
	int elem_num;
	hash_elem** table;
};

struct node {
	void* data;
	node_t *prev, *next;
};

struct list {
	node_t *first, *last;
};

/*		Hashtables		*/

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
int* hashtable_calc_hashes(char* key, int size) {
	int* hashing = (int*)secure_malloc(sizeof(int) * 3);

	hashing[0] = calc_hash(key);
	hashing[1] = calc_hash_step(key);
	hashing[2] = hashing[1] % size;

	if (hashing[2] == 0) {
		hashing[2] = 1;
	}

	return hashing;
}

/**
 * Creates a hashtable with the specified size and starting information
 * and returns a pointer to it.
 */
hashtable* hashtable_create(int size) {
	int i;
	hashtable* new_hash_t = (hashtable*)secure_malloc(sizeof(hashtable));

	new_hash_t->table = (hash_elem**)secure_malloc(size * sizeof(hash_elem*));
	new_hash_t->size = size;
	new_hash_t->elem_num = 0;

	for(i = 0; i < size; i++) {
		new_hash_t->table[i] = NULL;
	}

	return new_hash_t;
}

/**
 *
 */
hashtable* hashtable_insert(hashtable* hash_t, void* data, char* key) {
	int* hashing = hashtable_calc_hashes(key, hash_t->size);
	int h = hashing[0], phi = hashing[2], i = 1;
	hash_elem* elem = (hash_elem*)secure_malloc(sizeof(hash_elem));

	elem->data = data;
	elem->hash_1 = hashing[0];
	elem->hash_2 = hashing[1];
	elem->state = HASHTABLE_TAKEN;

	free(hashing);

	while (hash_t->table[h] != NULL) {
		if (hash_t->table[h]->state == HASHTABLE_DELETED) {
			free(hash_t->table[h]);
			break;
		} else {
			h = (elem->hash_1 + i * phi) % hash_t->size;
			i++;
		}
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
void* hashtable_get(hashtable* hash_t, char* key, char*(*get_key)(void*)) {
	int* hashing = hashtable_calc_hashes(key, hash_t->size);
	int h = hashing[0], hash_1 = hashing[0], phi = hashing[2], i = 1;

	free(hashing);

	while (hash_t->table[h] != NULL) {
		if (hash_t->table[h]->state == HASHTABLE_DELETED
			|| strcmp(get_key(hash_t->table[h]), key) != 0) {
			h = (hash_1 + i * phi) % hash_t->size;
			i++;
		} else {
			return hash_t->table[h]->data;
		}
	}

	return NULL;
}

/**
 *
 */
void hashtable_remove(hashtable* hash_t, char* key, char*(*get_key)(void*)) {
	hash_elem* elem_remove = NULL;
	int* hashing = hashtable_calc_hashes(key, hash_t->size);
	int h = hashing[0], hash_1 = hashing[0], phi = hashing[2], i = 1;

	free(hashing);

	while (hash_t->table[h] != NULL) {
		if (hash_t->table[h]->state == HASHTABLE_DELETED
			|| strcmp(get_key(hash_t->table[h]), key) != 0) {
			h = (hash_1 + i * phi) % hash_t->size;
			i++;
		} else {
			elem_remove = hash_t->table[h];
		}
	}

	if (elem_remove != NULL) {
		elem_remove->state = HASHTABLE_DELETED;
		--hash_t->elem_num;
	}
}

/**
 *
 */
void hashtable_reinsert(hash_elem* elem, hashtable* new_hash_t) {
	int h = elem->hash_1, i = 1;
	int phi = elem->hash_2 % new_hash_t->size;

	if (phi == 0) {
		phi = 1;
	}

	while (new_hash_t->table[h] != NULL) {
		h = (elem->hash_1 + i * phi) % new_hash_t->size;
		i++;
	}
	new_hash_t->table[h] = elem;
}

/**
 *
 */
hashtable* hashtable_expand(hashtable* hash_t) {
	int i;
	hashtable* new_hash_t = hashtable_create(getPrime(hash_t->size * 2));

	for (i = 0; i < hash_t->size; i++) {
		if (hash_t->table[i] != NULL
			&& hash_t->table[i]->state != HASHTABLE_DELETED) {
			hashtable_reinsert(hash_t->table[i], new_hash_t);
		}
	}

	hashtable_destroy(hash_t);

	return new_hash_t;
}

/**
 *
 */
void hashtable_destroy(hashtable* hash_t) {
	int i;

	for (i = 0; i < hash_t->size; i++) {
		free(hash_t->table[i]);
	}
	free(hash_t->table);
	free(hash_t);
}

/*		Linked Lists		*/

/**
 *
 */
list_t* list_create() {
	list_t* new_list = (list_t*)secure_malloc(sizeof(list_t));
	new_list->first = NULL;
	new_list->last = NULL;

	return new_list;
}

/**
 *
 */
void list_insert(list_t* list, void* data) {
	node_t* new_node = (node_t*)secure_malloc(sizeof(node_t));

	new_node->data = data;

	if (list->first == NULL) {
		list->last = new_node;
	} else {
		new_node->prev = NULL;
		new_node->next = list->first;
		list->first->prev = new_node;
	}

	list->first = new_node;
}

/**
 *
 */
void list_remove(list_t* list, hashtable* hash_t, void* key,
				 char*(*get_key)(void*), void(*clear_data)(node_t*)) {
	node_t* node_removal = hashtable_get(hash_t, key, get_key);

	if (node_removal == NULL) {
		return;
	} else if (node_removal->prev != NULL) {
		node_removal->prev->next = node_removal->next;
	} else if (node_removal->prev == NULL) {
		list->first = node_removal->next;
	} else if (node_removal->next != NULL) {
		node_removal->next->prev = node_removal->prev;
	} else if (node_removal->next == NULL) {
		list->last = node_removal->prev;
	}

	clear_data(node_removal);
	free(node_removal);
}

/**
 *
 */
void list_destroy(list_t* list, void(*clear_data)(node_t*)) {
	node_t *p1, *p2 = list->first;

	do {
		p1 = p2;
		p2 = p2->next;
		clear_data(p1);
		free(p1);
	} while (p2 != NULL);

	free(list);
}

/*		Utilities		*/

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
	int i;

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
