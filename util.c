/*
 * @Author: your name
 * @Date: 2020-12-06 14:42:58
 * @LastEditTime: 2020-12-08 14:27:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\util.c
 */

#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

vector *new_vector() {
    vector *v = calloc(1, sizeof(vector));
    v->data = malloc(sizeof(void *) * 16);
    v->capacity = 16;
    v->len = 0;
    return v;
}

void vector_push(vector *v, void *elem) {
    if (v->len == v->capacity) {
        if (v->capacity <= 1024) {
            v->capacity *= 2;
        } else {
            v->capacity *= 1.25;
        }
        v->data = realloc(v->data, sizeof(void *) * v->capacity);
    }
    v->data[v->len++] = elem;
}

void vector_push_int(vector *v, int val) {
    int *tmp = malloc(sizeof(int));
    *tmp = val;
    vector_push(v, tmp);
}

void *vector_pop(vector *v) {
    assert(v->len);
    if (v->len < v->capacity / 2) {
        v->capacity /= 2;
        v->data = realloc(v->data, sizeof(void *) * v->capacity);
    }
    return v->data[--v->len];
}

void *vector_delete(vector *v, int i) {
    if (i < 0 || i >= v->len) {
        return NULL;
    }
    void *elem = v->data[i];
    memcpy(v->data[i], v->data[i + 1], sizeof(void *) * (v->len - i - 1));
    v->len--;
    return elem;
}

void *vector_delete_elem(vector *v, void *elem) {
    if ((intptr_t)elem < (intptr_t)v->data ||
        (intptr_t)elem >= (intptr_t)v->data[v->len]) {
        return NULL;
    }
    int i = ((intptr_t)elem - (intptr_t)v->data) / sizeof(void *);
    memcpy(v->data[i], v->data[i + 1], sizeof(void *) * (v->len - i - 1));
    v->len--;
    return elem;
}

void *vector_get(vector *v, int i) {
    assert(i < v->len);
    return v->data[i];
}

int vector_get_int(vector *v, int i) {
    assert(i < v->len);
    return *((int *)(v->data[i]));
}

void *vector_last(vector *v) {
    assert(v->len);
    return v->data[v->len - 1];
}

int vector_contain(vector *v, void *elem) {
    for (int i = 0; i < v->len; i++) {
        if (v->data[i] == elem) {
            return 1;
        }
    }
    return 0;
}

int vector_union(vector *v, void *elem) {
    if (vector_contain(v, elem)) return 1;
    vector_push(v, elem);
    return 0;
}

int vector_size(vector *v) {
    return v->len;
}

void vector_clear(vector *v) {
    v->len = 0;
    v->capacity = 16;
    v->data = realloc(v->data, sizeof(void *) * v->capacity);
}

void free_vector(vector *v) {
    free(v->data);
    free(v);
}

void free_vector_all(vector *v) {
    for (int i = 0; i < v->len; i++) {
        free(v->data[i]);
    }
    free_vector(v);
}

void vector_sort(vector *v) {
    
}

vector *read_file(char *filename, size_t size) {
    vector* v = new_vector();
    FILE *fp = fopen(filename, "r+b");
    if (fp == NULL) return v;
    while (1) {
        void *chunk = malloc(size);
        if (fread(chunk, size, 1, fp) != 1) {
            break;
        }
        vector_push(v, chunk);
    }
    fclose(fp);
    return v;
}

static void write_file(FILE *fp, vector *v, size_t size) {
    for (int i = 0; i < vector_size(v); i++) {
        void *ptr = vector_get(v, i);
        fwrite(ptr, size, 1, fp);
    }
}

void write_file_cover(char *filename, vector *v, size_t size) {
    FILE *fp = fopen(filename, "w+b");
    if (fp == NULL) return;
    write_file(fp, v, size);
    fclose(fp);
}

void write_file_append(char *filename, vector *v, size_t size) {
    FILE *fp = fopen(filename, "a+b");
    if (fp == NULL) return;
    write_file(fp, v, size);
    fclose(fp);
}

trie *new_trie() {
    trie *t = calloc(1, sizeof(trie));
    t->root = calloc(1, sizeof(struct node));
    return t;
}

void *trie_find(trie *t, char *name) {
    struct node *n = t->root;
    int i = 0;
    while (i < strlen(name) && n) {
        n = n->child[name[i++]];
    }
    if (i == strlen(name) && n) {
        return n->data;
    } else {
        return NULL;
    }
}

static void *trie_find_fuzzy_dfs(struct node *n) {
    if (n == NULL) return NULL;
    for (int i = 0; i < 256; i++) {
        struct node* x = n->child[i];
        if (x && x->data) {
            return x;
        }
        if (x) {
            void *data = trie_find_fuzzy_dfs(x);
            if (data) return data;
        }
    }
}

void *trie_find_fuzzy(trie *t, char *name) {
    struct node *n = t->root;
    int i = 0;
    while (i < strlen(name) && n) {
        n = n->child[name[i++]];
    }
    if (i == strlen(name) && n && n->data) {
        return n->data;
    }
    void *data = trie_find_fuzzy_dfs(n);
    return data;
}

void trie_add(trie *t, char *name, void *data) {
    struct node *n = t->root;
    for (int i = 0; i < strlen(name); i++) {
        if (n->child[name[i]] == NULL) {
            n->child[name[i]] = calloc(1, sizeof(struct node));
        }
        n = n->child[name[i]];
    }
    n->data = data;
}

void trie_delete(trie *t, char *name) {
    struct node *n = t->root;
    int i = 0;
    while (i < strlen(name) && n) {
        n = n->child[name[i++]];
    }
    if (i == strlen(name) && n) {
        n->data = NULL;
    }
}

static void free_node(struct node *n) {
    for (int i = 0; i < 256; i++) {
        if (n->child[i] != NULL) {
            free_node(n->child[i]);
        }
    }
    free(n);
}

void free_trie(trie *t) {
    free_node(t->root);
    free(t);
}

static int _vscprintf_so(const char * format, va_list pargs) {
    int retval;
    va_list argcopy;
    va_copy(argcopy, pargs);
    retval = vsnprintf(NULL, 0, format, argcopy);
    va_end(argcopy);
    return retval;
}

static int vasprintf(char **strp, const char *fmt, va_list ap) {
    size_t len = _vscprintf_so(fmt, ap);
    if (len == -1) return -1;
    char *str = malloc((size_t) len + 1);
    if (!str) return -1;
    int r = vsnprintf(str, len + 1, fmt, ap);
    if (r == -1) return free(str), -1;
    *strp = str;
    return r;
}

int asprintf(char *strp[], const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int r = vasprintf(strp, fmt, ap);
    va_end(ap);
    return r;
}