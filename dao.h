/*
 * @Author: your name
 * @Date: 2020-12-05 22:37:21
 * @LastEditTime: 2020-12-09 08:54:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\dao.h
 */
#ifndef _DAO_H_
#define _DAO_H_

#include "util.h"
#include <stdbool.h>
#include "model.h"

bool db_init();
bool db_close();
vector *list_user();
USER *get_user(char *name);
USER *get_user_by_id(int id);
USER *add_user(char *name, char *pwd, bool is_admin);
USER *modify_user(USER *user, char *pwd, bool is_admin);
void delete_user(USER *user);

vector *list_book();
BOOK *get_book(char *name);
BOOK *get_book_by_id(int id);
BOOK *add_book(char *name, char *public, char *author, int price, int total);
BOOK *modify_book(BOOK *book, char *name, char *public, char *author, int price, int total, int borrowed);
void delete_book(BOOK *book);
void add_user_borrowed(USER *user, int book_id);
bool delete_user_borrowed(USER *user, int book_id);
vector *get_user_borrowed(USER *user);

#endif
