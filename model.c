/*
 * @Author: your name
 * @Date: 2020-12-04 21:58:39
 * @LastEditTime: 2020-12-09 08:35:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\model.c
 */
#include "model.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

USER *new_user(int id, char *name, char *pwd, bool is_admin) {
    USER *user = calloc(1, sizeof(USER));
    user->id = id;
    strcpy(user->name, name);
    strcpy(user->pwd, pwd);
    user->is_admin = is_admin;
    user->borrowed = new_vector();
    return user;
}

void free_user(USER *user) {
    free_vector_all(user->borrowed);
    free(user);
}

BOOK *new_book(int id, char *name, char *public, char *author, int price, int total) {
    BOOK *book = calloc(1, sizeof(BOOK));
    book->id = id;
    strcpy(book->name, name);
    strcpy(book->public, public);
    strcpy(book->author, author);
    book->price = price;
    book->total = total;
    return book;
}

void free_book(BOOK *book) {
    free(book);
}