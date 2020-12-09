/*
 * @Author: your name
 * @Date: 2020-12-04 21:58:23
 * @LastEditTime: 2020-12-09 13:21:12
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\controller.c
 */

#include "controller.h"
#include <string.h>
#include "md5.h"
#include "dao.h"
#include "resource.h"

INFO user_list() {
    vector *users = list_user();
    INFO info = {
        true,
        "",
        users,
    };
    return info;
}

INFO user_search(char *username) {
    USER *user = get_user(username);
    if (user == NULL) {
        INFO info = {
            false,
            "该用户不存在",
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

INFO user_modify(USER *user, char *password, bool is_admin) {
    char pwd[17];
    memset(pwd, 0, sizeof(pwd));
    if (strlen(password) == 0 || strcmp(password, "-1") == 0) {
        password = NULL;
    } else {
        MDString(password, pwd);
        password = pwd;
    }

    modify_user(user, password, is_admin);
    INFO info = {
        true,
        "",
        user,
    };
    return info;
}

INFO book_search(char *bookname, bool specific) {
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
            specific == false,
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

INFO book_borrow_name(USER *user, char *bookname) {
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
    add_user_borrowed(user, book->id);
    modify_book(book, NULL, NULL, NULL, -1, -1, book->borrowed + 1);
    INFO info = {
        true,
        "借阅成功",
        book,
    };
    return info;
}

INFO book_user_borrowed(USER *user) {
    vector *v = get_user_borrowed(user);
    INFO info = {
        true,
        "",
        v,
    };
    return info;
}

INFO book_return_id(USER *user, int book_id) {
    BOOK *book = get_book_by_id(book_id);
    if (book == NULL) {
        INFO info = {
            false,
            "找不到该图书",
            NULL,
        };
        return info;
    }
    if (delete_user_borrowed(user, book->id) == false) {
        INFO info = {
            false,
            "未借阅此书",
            book,
        };
        return info;
    }
    modify_book(book, NULL, NULL, NULL, -1, -1, book->borrowed - 1);
    INFO info = {
        true,
        "",
        book,
    };
    return info;
}

INFO book_add(char *bookname, char *pubname, char *author, int price, int total) {
    BOOK *book = get_book(bookname);
    if (book) {
        INFO info = {
            false,
            "同名图书已存在",
            book,
        };
        return info;
    }
    if (price < 0) {
        INFO info = {
            false,
            "价格不能小于0",
            NULL,
        };
        return info;
    }
    if (total < 0) {
        INFO info = {
            false,
            "数量不能小于0",
            NULL,
        };
        return info;
    }
    book = add_book(bookname, pubname, author, price, total);
    INFO info = {
        true,
        "",
        book,
    };
    return info;
}

INFO book_modify(BOOK *book, char *bookname, char *pubname, char *author, int price, int total) {
    if (strlen(bookname) == 0 || strcmp(bookname, "-1") == 0) bookname = NULL;
    if (strlen(pubname) == 0 || strcmp(pubname, "-1") == 0) pubname = NULL;
    if (strlen(author) == 0 || strcmp(author, "-1") == 0) author = NULL;
    if (price < -1) {
        INFO info = {
            false,
            "价格不能小于-1",
            NULL,
        };
        return info;
    }
    if (total < -1) {
        INFO info = {
            false,
            "数量不能小于-1",
            NULL,
        };
        return info;
    }
    modify_book(book, bookname, pubname, author, price, total, -1);
    INFO info = {
        true,
        "",
        book,
    };
    return info;
}

INFO book_delete_id(int book_id) {
    BOOK* book = get_book_by_id(book_id);
    if (book == NULL) {
        INFO info = {
            false,
            "该图书不存在",
            NULL,
        };
        return info;
    }
    delete_book(book);
    INFO info = {
        true,
        "",
        NULL,
    };
    return info;
}