/*
 * @Author: your name
 * @Date: 2020-12-05 12:21:25
 * @LastEditTime: 2020-12-08 14:56:31
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\resources.h
 */
#ifndef _RESOURCES_H_
#define _RESOURCES_H_

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

#define VIEW_BOOK_INFO_FORMAT "\
|编号：%d\t|书名：%s\t|出版社：%s\t|作者：%s\t|售价：%d\t|馆藏%d本|剩余%d本|"

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

#define VIEW_PAGE_SWITCH_WELCOME_FORMAT "\
欢迎登陆 %s！\n"
#define VIEW_PAGE_SWITCH_OPTIONS_ADMIN "\
[1]\t管理系统\n\
[2]\t图书系统\n\
[3]\t修改密码\n\
[4]\t退出登陆\n"
#define VIEW_PAGE_SWITCH_OPTIONS_USER "\
[1]\t图书系统\n\
[2]\t修改密码\n\
[3]\t退出登陆\n"

#define VIEW_PAGE_BOOK_TITLE "\
图书系统\n"
#define VIEW_PAGE_BOOK_OPTIONS "\
[1]\t查询图书\n\
[2]\t浏览图书\n\
[3]\t借阅图书\n\
[4]\t归还图书\n\
[5]\t返回\n"

#endif