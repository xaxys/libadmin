/*
 * @Author: your name
 * @Date: 2020-12-04 21:58:23
 * @LastEditTime: 2020-12-08 14:42:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\controller.c
 */

#include "controller.h"
#include <string.h>
#include "md5.h"
#include "dao.h"
#include "resource.h"

INFO user_login(char *username, char *password) {
    char pwd[17];
    memset(pwd, 0, sizeof(pwd));
    MDString(password, pwd);
    USER *user = get_user(username);
    if (user == NULL) {
        INFO info = {
            false,
            "该用户不存在",
            NULL,
        };
        return info;
    }
    if (strcmp(pwd, user->pwd) != 0) {
        INFO info = {
            false,
            "密码错误",
            NULL,
        };
        return info;
    }
    INFO info = {
        true,
        "",
        user,
    };
    return info;
}

INFO user_register(char *username, char *password, bool is_admin) {
    USER *user = get_user(username);
    if (user) {
        INFO info = {
            false,
            "该用户名已存在",
            NULL,
        };
        return info;
    }

    char pwd[17];
    memset(pwd, 0, sizeof(pwd));
    MDString(password, pwd);

    user = add_user(username, pwd, is_admin);
    INFO info = {
        true,
        "",
        user,
    };
    return info;
}

INFO user_change_pwd(char *username, char *password) {
    USER *user = get_user(username);
    if (user == NULL) {
        INFO info = {
            false,
            "该用户不存在",
            NULL,
        };
        return info;
    }

    char pwd[17];
    memset(pwd, 0, sizeof(pwd));
    MDString(password, pwd);

    modify_user(user, pwd, user->is_admin);
    INFO info = {
        true,
        "",
        user,
    };
    return info;
}

INFO book_search(char *bookname) {
    BOOK *book = get_book(bookname);
    if (book == NULL) {
        INFO info = {
            false,
            "找不到符合要求的图书",
            NULL,
        };
        return info;
    }

    if (strcmp(bookname, book->name) != 0) {
        INFO info = {
            true,
            "找到了相近的的图书",
            book,
        };
        return info;
    }

    INFO info = {
        true,
        "找到了该图书",
        book,
    };
    return info;
}

INFO book_list() {
    vector *books = list_book();
    INFO info = {
        true,
        "",
        books,
    };
    return info;
}

INFO book_borrow(USER *user, char *bookname) {
    BOOK *book = get_book(bookname);
    if (book == NULL) {
        INFO info = {
            false,
            "找不到该图书",
            NULL,
        };
        return info;
    }
    if (book->total - book->borrowed < 1) {
        INFO info = {
            false,
            "该图书没有剩余",
            NULL,
        };
        return info;
    }
    if (vector_size(user->borrowed) > MAX_BORROWED_BOOKS) {
        INFO info = {
            false,
            "超出您可同时借阅上限",
            NULL,
        };
        return info;
    }
    book->borrowed++;
    vector_push_int(user->borrowed, book->id);
    INFO info = {
        true,
        "借阅成功",
        book,
    };
    return info;
}
