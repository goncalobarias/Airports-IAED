/*
 *		File: structures.h
 * 		Author: Gonçalo Sampaio Bárias (ist1103124)
 *		Description: Header file for the data strucutes used in the
 *					project.
 */

#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

/**
 * Constants
 */

#define MEMORY_ERR "No memory.\n"
#define MEMORY_ERR_CODE -1
#define HASHTABLE_MAX_LOAD 0.5
#define HASHTABLE_START_SIZE 20047
#define HASHTABLE_DELETED -8
#define HASHTABLE_TAKEN -9
#define LIST_SORTED -10
#define LIST_UNSORTED -11

/**
 * Data structures
 */

typedef struct hash_elem_t {
    void *data;
    int state;
} hash_elem;

typedef struct hashtable_t {
    int size;
    int elem_num;
    struct hash_elem_t **table;
} hashtable;

typedef struct list {
    struct node *first, *last;
    int sorted;
} list_t;

typedef struct node {
    void *data;
    struct node *prev, *next;
} node_t;

/**
 * Prototypes
 */

/* hashtables */

unsigned int calc_hash(char *key);

unsigned int calc_hash_step(char *key);

unsigned int *hashtable_calc_hashes(char *key, int size);

hashtable *hashtable_create(int size);

hash_elem *hashtable_element_create(void *data);

hashtable *hashtable_insert(hashtable *hash_t, void *data, char *key,
                            char *(*get_key)(void *));

hash_elem *hashtable_get(hashtable *hash_t, char *key, char *cmp_val,
                         char *(*get_cmp_val)(void *));

list_t *hashtable_get_all(hashtable *hash_t, char *key, char *cmp_val,
                          char *(*get_cmp_val)(void *));

void hashtable_remove(hashtable *hash_t, char *key, char *cmp_val,
                      char *(*get_cmp_val)(void *));

int hash_elem_dead(hash_elem *elem);

hashtable *hashtable_expand(hashtable *hash_t, char *(*get_key)(void *));

void hashtable_destroy(hashtable *hash_t);

/* linked lists */

list_t *list_create();

node_t *list_insert(list_t *list, void *data);

void list_remove(list_t *list, node_t *node_removal);

void list_destroy(list_t *list);

/* merge sort */

void sort_list(list_t *list, int (*cmp)(void *, void *));

node_t *merge(node_t *first, node_t *second, int (*cmp)(void *, void *));

node_t *list_mergesort(node_t *head, int (*cmp)(void *, void *));

node_t *split(node_t *head);

#endif /* _STRUCTURES_H_ */
