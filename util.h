/*
 * @Author: your name
 * @Date: 2020-12-06 14:42:48
 * @LastEditTime: 2020-12-08 14:49:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\util.h
 */
#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdbool.h>

// vector
typedef struct {
    void **data;
    int capacity;
    int len;
} vector;

vector *new_vector();
void free_vector(vector *v);
void free_vector_all(vector *v);
void vector_push(vector *v, void *elem);
void vector_push_int(vector *v, int val);
void *vector_pop(vector *v);
void *vector_delete(vector *v, int i);
void *vector_delete_elem(vector *v, void *elem);
void *vector_get(vector *v, int i);
int vector_get_int(vector *v, int i);
void *vector_last(vector *v);
int vector_contain(vector *v, void *elem);
int vector_union(vector *v, void *elem);
int vector_size(vector *v);
void vector_clear(vector *v);
vector *read_file(char *filename, size_t size);
void write_file_cover(char *filename, vector *v, size_t size);
void write_file_append(char *filename, vector *v, size_t size);

// trie
typedef struct {
    struct node {
        struct node *child[256];
        void *data;
    } *root;
} trie;

trie *new_trie();
void free_trie(trie *t);
void *trie_find(trie *t, char *name);
void *trie_find_fuzzy(trie *t, char *name);
void trie_add(trie *t, char *name, void *data);
void trie_delete(trie *t, char *name);

// utils
int asprintf(char *strp[], const char *fmt, ...);

#endif