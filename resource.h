/*
 * @Author: your name
 * @Date: 2020-12-05 12:21:25
 * @LastEditTime: 2020-12-09 13:29:58
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\resources.h
 */
#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#define DEFAULT_SHOW_ENTRIES 10
#define MAX_BORROWED_BOOKS 5

#define DB_FOLDER "data"
#define DB_RESERVED "data\\reserved.bin"
#define DB_USER "data\\user.bin"
#define DB_USERDATA_FORMAT "data\\userdata_%d.bin"
#define DB_BOOK "data\\book.bin"

#define VERSION "0.0.1"
#define AUTHOR "xaxys"
#define DESCRIPTION "A Simple Library Management System"
#define COPYRIGHT "Copyright 2020 xaxys All Rights Reserved."

#define VIEW_SEPERATOR "----------------------------------------\n"
#define VIEW_USER_INFO_FORMAT "\
|编号：%d\t|用户名：%s\t|密码MD5：%s\t|管理员：%s\t|在借图书 %d 本\t|\n"
#define VIEW_BOOK_INFO_FORMAT "\
|编号：%d\t|书名：%s\t|出版社：%s\t|作者：%s\t|售价：%d\t|馆藏 %d 本\t|剩余 % d本\t|\n"

#define VIEW_PAGE_MAIN_WELCOME \
"Welcome to Library Management System\n"
#define VIEW_PAGE_MAIN_OPTIONS "\
[1]\t登录\n\
[2]\t注册\n\
[3]\t退出\n"
#define VIEW_PAGE_MAIN_ABOUT "\
It's a experimental project developed for learning purpose.\n\
Please expect breaking changes and unstable APIs. \n\
Most of them are currently at an early, experimental stage.\n"

#define VIEW_PAGE_SWITCH_WELCOME_FORMAT_USER "\
%s, 欢迎登陆 %s！\n"
#define VIEW_PAGE_SWITCH_WELCOME_FORMAT_ADMIN "\
%s, 欢迎登陆 管理员 %s！\n"
#define VIEW_PAGE_SWITCH_OPTIONS_ADMIN "\
[1]\t管理系统\n\
[2]\t图书系统\n\
[3]\t修改密码\n\
[4]\t退出登陆\n"
#define VIEW_PAGE_SWITCH_OPTIONS_USER "\
[1]\t图书系统\n\
[2]\t修改密码\n\
[3]\t退出登陆\n"

#define VIEW_PAGE_ADMIN_TITLE "\
管理系统\n"

#define VIEW_PAGE_ADMIN_OPTIONS "\
[1]\t添加/修改图书\n\
[2]\t删除图书\n\
[3]\t用户列表\n\
[4]\t修改用户\n\
[5]\t删除用户\n\
[6]\t返回\n"

#define VIEW_PAGE_BOOK_TITLE "\
图书系统\n"
#define VIEW_PAGE_BOOK_OPTIONS "\
[1]\t查询图书\n\
[2]\t浏览图书\n\
[3]\t借阅图书\n\
[4]\t归还图书\n\
[5]\t返回\n"

#endif