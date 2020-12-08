/*
 * @Author: your name
 * @Date: 2020-12-04 23:23:27
 * @LastEditTime: 2020-12-08 15:11:52
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\dao.c
 */
#include "dao.h"
#include <io.h>
#include <direct.h>
#include <stdbool.h>
#include <stdlib.h>
#include "util.h"
#include "resource.h"
#include "model.h"

// user
trie *dict_user;
vector *users;
int user_id_inc;

// book
trie *dict_book;
vector *books;
int book_id_inc;

bool db_init() {
    if (_access(DB_FOLDER, 0) == -1) {
        _mkdir(DB_FOLDER);
    }
    vector *reserved = read_file(DB_RESERVED, sizeof(int));
    if (vector_size(reserved) > 0) {
        if (vector_size(reserved) != 2) return false;
        user_id_inc = vector_get_int(reserved, 0);
        book_id_inc = vector_get_int(reserved, 1);
    }

    users = read_file(DB_USER, sizeof(USER));
    books = read_file(DB_BOOK, sizeof(BOOK));

    // calculate borrowed books
    for (int i = 0; i < vector_size(books) ; i++) {
        BOOK *book = vector_get(books, i);
        book->borrowed = 0;
    }

    for (int i = 0 ; i < vector_size(users) ; i++) {
        USER *user = vector_get(users, i);

        char *filename;
        asprintf(&filename, DB_USERDATA_FORMAT, user->id);
        vector *borrowed = read_file(filename, sizeof(int));
        free(filename);
        
        user->borrowed = borrowed;
        for (int j = 0; j < vector_size(borrowed) ; j++) {
            int book_id = vector_get_int(borrowed, j);
            BOOK *book = vector_get(books, book_id);
            book->borrowed++;
        }
    }

    // build trie
    dict_user = new_trie();
    for (int i = 0 ; i < vector_size(users) ; i++) {
        USER *user = vector_get(users, i);
        trie_add(dict_user, user->name, user);
    }

    dict_book = new_trie();
    for (int i = 0; i < vector_size(books) ; i++) {
        BOOK *book = vector_get(books, i);
        trie_add(dict_book, book->name, book);
    }
}

bool db_close() {
    vector *v = new_vector();
    vector_push(v, &user_id_inc);
    vector_push(v, &book_id_inc);
    write_file_cover(DB_RESERVED, v, sizeof(int));
    free_vector(v);

    for (int i = 0 ; i < vector_size(users) ; i++) {
        USER *user = vector_get(users, i);

        char *filename;
        asprintf(&filename, DB_USERDATA_FORMAT, user->id);
        write_file_cover(filename, user->borrowed, sizeof(int));
        free(filename);
    }

    write_file_cover(DB_USER, users, sizeof(USER));
    write_file_cover(DB_BOOK, books, sizeof(BOOK));
    
    for (int i = 0 ; i < vector_size(users) ; i++) {
        USER *user = vector_get(users, i);
        free_user(user);
    }
    free_vector(users);
    
    for (int i = 0 ; i < vector_size(books) ; i++) {
        BOOK *book = vector_get(books, i);
        free_book(book);
    }
    free_vector(books);

    free_trie(dict_user);
    free_trie(dict_book);
    return true;
}

USER *get_user(char *name) {
    USER *user = trie_find(dict_user, name);
    return user;
}

USER *add_user(char *name, char *pwd, bool is_admin) {
    if (user_id_inc == 0) is_admin = true;
    USER *user = new_user(user_id_inc++, name, pwd, is_admin);
    trie_add(dict_user, name, user);
    vector_push(users, user);
    return user;
}

USER *modify_user(USER *user, char *pwd, bool is_admin) {
    if (pwd) strcpy(user->pwd, pwd);
    user->is_admin = is_admin;
    return user;
}

void delete_user(USER *user) {
    trie_delete(dict_user, user->name);
    vector_delete_elem(users, user);
    free_user(user);
}

BOOK *get_book(char *name) {
    BOOK *book = trie_find_fuzzy(dict_book, name);
    return book;
}

vector *list_book() {
    return books;
}