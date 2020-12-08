/*
 * @Author: your name
 * @Date: 2020-12-05 19:30:30
 * @LastEditTime: 2020-12-08 11:01:08
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\model.h
 */
#ifndef _MODEL_H_
#define _MODEL_H_

#include "util.h"
#include <stdbool.h>

typedef struct {
    int  id;
    char name[256];
    char pwd[17];
    bool is_admin;
    vector *borrowed;
} USER;

USER *new_user(int id, char *name, char *pwd, bool is_admin);
void free_user(USER *user);

typedef struct {
    int  id;
    char name[256];
    char public[256];
    char author[256];
    int  price;
    int  total;
    int  borrowed;
} BOOK;

BOOK *new_book(int id, char *name, char *public, char *author, int price, int total);
void free_book(BOOK *book);

typedef struct {
    bool state;
    char msg[256];
    void *ptr;
} INFO;

#endif 
