/*
 * @Author: your name
 * @Date: 2020-12-05 22:37:21
 * @LastEditTime: 2020-12-08 14:44:07
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
USER *get_user(char *name);
USER *add_user(char *name, char *pwd, bool is_admin);
USER *modify_user(USER *user, char *pwd, bool is_admin);
void delete_user(USER *user);

BOOK *get_book(char *name);
vector* list_book();

#endif
