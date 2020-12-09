/*
 * @Author: your name
 * @Date: 2020-12-04 23:23:27
 * @LastEditTime: 2020-12-09 09:23:43
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
trie *dict_user_id;
trie *dict_user;
vector *users;
int user_id_inc;

// book
trie *dict_book_id;
trie *dict_book;
vector *roll_vector;
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
    roll_vector = read_file(DB_BOOK, sizeof(BOOK));

    // calculate borrowed books
    for (int i = 0; i < vector_size(roll_vector) ; i++) {
        BOOK *book = vector_get(roll_vector, i);
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
            BOOK *book = vector_get(roll_vector, book_id);
            book->borrowed++;
        }
    }

    // build trie
    dict_user = new_trie();
    dict_user_id = new_trie();
    for (int i = 0 ; i < vector_size(users) ; i++) {
        USER *user = vector_get(users, i);
        trie_add(dict_user, user->name, user);
        trie_add_int(dict_user_id, user->id, user);
    }

    dict_book = new_trie();
    dict_book_id = new_trie();
    for (int i = 0; i < vector_size(roll_vector) ; i++) {
        BOOK *book = vector_get(roll_vector, i);
        trie_add(dict_book, book->name, book);
        trie_add_int(dict_book_id, book->id, book);
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
    write_file_cover(DB_BOOK, v, sizeof(BOOK));
    
    for (int i = 0 ; i < vector_size(users) ; i++) {
        USER *user = vector_get(users, i);
        free_user(user);
    }
    free_vector(users);
    
    for (int i = 0 ; i < vector_size(v) ; i++) {
        BOOK *book = vector_get(v, i);
        free_book(book);
    }
    free_vector(v);

    free_trie(dict_user);
    free_trie(dict_user_id);
    free_trie(dict_book);
    free_trie(dict_book_id);
    return true;
}

vector *list_user() {
    return users;
}

USER *get_user(char *name) {
    USER *user = trie_find(dict_user, name);
    return user;
}

USER *get_user_by_id(int id) {
    USER *user = trie_find_int(dict_user_id, id);
    return user;
}

USER *add_user(char *name, char *pwd, bool is_admin) {
    if (user_id_inc == 0) is_admin = true;
    USER *user = new_user(user_id_inc++, name, pwd, is_admin);
    trie_add(dict_user, user->name, user);
    trie_add_int(dict_user_id, user->id, user);
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
    trie_delete_int(dict_user_id, user->id);
    vector_delete_elem(users, user);
    free_user(user);
}

BOOK *get_book(char *name) {
    BOOK *book = trie_find_fuzzy(dict_book, name);
    return book;
}

BOOK *add_book(char *name, char *public, char *author, int price, int total) {
    BOOK *book = new_book(book_id_inc++, name, public, author, price, total);
    trie_add(dict_book, book->name, book);
    trie_add_int(dict_book_id, book->id, book);
    vector_push(roll_vector, book);
    return book;
}

BOOK *get_book_by_id(int id) {
    BOOK *book = trie_find_int(dict_book_id, id);
    return book;
}

vector *list_book() {
    return roll_vector;
}

BOOK *modify_book(BOOK *book, char *name, char *public, char *author, int price, int total, int borrowed) {
    if (name) strcpy(book->name, name);
    if (public) strcpy(book->public, public);
    if (author) strcpy(book->author, author);
    if (price != -1) book->price = price;
    if (total != -1) book->total = total;
    if (borrowed != -1) book->borrowed = borrowed;
    return book;
}

void delete_book(BOOK *book) {
    trie_delete(dict_book, book->name);
    trie_delete_int(dict_book_id, book->id);
    vector_delete_elem(roll_vector, book);
    free_book(book);
}

void add_user_borrowed(USER *user, int book_id) {
    vector_push_int(user->borrowed, book_id);
}

bool delete_user_borrowed(USER *user, int book_id) {
    vector *v = user->borrowed;
    for (int i = 0 ; i < vector_size(v) ; i++) {
        BOOK *book = vector_get(v, i);
        if (book_id == book->id) {
            vector_delete(v, i);
            return true;
        }
    }
    return false;
}

vector *get_user_borrowed(USER *user) {
    return user->borrowed;
}