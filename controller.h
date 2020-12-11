/*
 * @Author: your name
 * @Date: 2020-12-05 19:23:31
 * @LastEditTime: 2020-12-11 22:01:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\controller.h
 */
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "model.h"

INFO user_list();
INFO user_search(char *username);
INFO user_login(char *username, char *password);
INFO user_register(char *username, char *password, bool is_admin);
INFO user_change_pwd(char *username, char *password);
INFO user_modify(USER *user, char *password, bool is_admin);
INFO user_delete_id(int user_id);

INFO book_search(char *bookname, bool specific);
INFO book_list();
INFO book_borrow_name(USER *user, char *bookname);
INFO book_user_borrowed(USER *user);
INFO book_return_id(USER *user, int book_id);
INFO book_add(char *bookname, char *pubname, char *author, int price, int total);
INFO book_modify(BOOK *book, char *bookname, char *pubname, char *author, int price, int total);
INFO book_delete_id(int book_id);

#endif