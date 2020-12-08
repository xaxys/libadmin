/*
 * @Author: your name
 * @Date: 2020-12-05 19:23:31
 * @LastEditTime: 2020-12-05 20:19:13
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\controller.h
 */
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "model.h"

INFO user_login(char *username, char *password);
INFO user_register(char *username, char *password, bool is_admin);
INFO user_change_pwd(char *username, char *password);

INFO book_search(char *bookname);
INFO book_list();
INFO book_borrow(USER *user, char *bookname);

#endif