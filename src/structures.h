/*
 *		File: structures.h
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: Header file for the data strucutes used in the project.
 */

#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

/**
 * Constants
 */

#define MEMORY_ERR "No memory.\n"
#define MEMORY_ERR_CODE -1
#define HASHTABLE_MAX_LOAD 0.5
#define HASHTABLE_START_SIZE 11
#define HASHTABLE_DELETED -2
#define HASHTABLE_TAKEN -3

/**
 * Data structures
 */

typedef struct hash_elem_t {
	void* data;
	unsigned int hash_1, hash_2;
	int state;
} hash_elem;

typedef struct hashtable_t {
	int size;
	int elem_num;
	struct hash_elem_t** table;
} hashtable;

typedef struct list {
	struct node *first, *last;
} list_t;

typedef struct node {
	void* data;
	struct node *prev, *next;
} node_t;

/**
 * Prototypes
 */

/* hashtables */

unsigned int calc_hash(char* key);

unsigned int calc_hash_step(char* key);

unsigned int* hashtable_calc_hashes(char* key, int size);

hashtable* hashtable_create(int size);

hashtable* hashtable_insert(hashtable* hash_t, void* data, char* key, void(*clear_data)(void*));

void* hashtable_get(hashtable* hash_t, char* key, char*(*get_key)(void*));

void hashtable_remove(hashtable* hash_t, char* key, char*(*get_key)(void*), void(*clear_data)(void*));

hashtable* hashtable_expand(hashtable* hash_t, void(*clear_data)(void*));

void hashtable_destroy(hashtable* hash_t, void(*clear_data)(void*));

/* linked lists */

list_t* list_create();

void list_insert(list_t* list, void* data);

void list_remove(list_t* list, void* key, char*(*get_key)(void*), void(*clear_data)(void*));

void list_destroy(list_t* list, void(*clear_data)(void*));

/* merge sort */

node_t* merge(node_t* first, node_t* second, int(*cmp)(void*, void*));

node_t* list_mergesort(node_t* head, int(*cmp)(void*, void*));

node_t* split(node_t* head);

#endif
