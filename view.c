/*
 * @Author: your name
 * @Date: 2020-12-04 21:58:01
 * @LastEditTime: 2020-12-08 21:44:22
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\view.c
 */
#include "view.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "model.h"
#include "resource.h"
#include "controller.h"

PAGE current_page;
USER *current_user;
bool exited = false;
bool wait_confirm;

bool roll_mode;
void (*roll_update_func)();
vector *books;
int book_idx;

static void print_book_info(BOOK *book) {
    printf(VIEW_BOOK_INFO_FORMAT,
        book->id,
        book->name,
        book->public,
        book->author,
        book->price,
        book->total,
        book->total - book->borrowed);
}

static void roll_books() {
    book_idx++;
    if (book_idx < vector_size(books)) {
        print_book_info(vector_get(books, book_idx));
    } else {
        printf("翻到底啦！按q退出\n");
    }
}

static void print_page_main() {
    printf(VIEW_PAGE_MAIN_WELCOME);
    printf("Version: %s\n", VERSION);
    printf("Author: %s\n", AUTHOR);
    printf("Description: %s\n", DESCRIPTION);
    printf(VIEW_SEPERATOR);
    printf(VIEW_PAGE_MAIN_OPTIONS);
    printf(VIEW_SEPERATOR);
    printf(VIEW_PAGE_MAIN_ABOUT);
    printf("%s\n", COPYRIGHT);
}

static void print_page_switch() {
    printf(VIEW_PAGE_SWITCH_WELCOME_FORMAT, current_user->name);
    printf(VIEW_SEPERATOR);
    if (current_user->is_admin) {
        printf(VIEW_PAGE_SWITCH_OPTIONS_ADMIN);
    } else {
        printf(VIEW_PAGE_SWITCH_OPTIONS_USER);
    }
    printf(VIEW_SEPERATOR);
    printf("%s\n", COPYRIGHT);
}

static void print_page_admin() {

}

static void print_page_book() {
    printf(VIEW_PAGE_BOOK_TITLE);
    printf(VIEW_SEPERATOR);
    printf(VIEW_PAGE_BOOK_OPTIONS);
    printf(VIEW_SEPERATOR);
    printf("%s\n", COPYRIGHT);
}

static refresh_page() {
    switch_page(current_page);
}

void switch_page_wait(PAGE page) {
    current_page = page;
    wait_confirm = true;
    printf("按下任意键继续...\n");
}

void switch_page(PAGE page) {
    system("cls");
    switch (page) {
    case PAGE_MAIN:
        print_page_main();
        break;
    case PAGE_SWITCH:
        print_page_switch();
        break;
    case PAGE_ADMIN:
        print_page_admin();
        break;
    case PAGE_BOOK:
        print_page_book();
        break;
    default:
        fprintf(stderr, "Internal Error! Please contact the author.\n");
    }
    current_page = page;
}

static void handle_page_main(char ch) {
    switch (ch) {
    case '1': {
        char username[256];
        char password[256];
        printf("请输入用户名：");
        scanf("%s", username);
        printf("请输入密码：");
        scanf("%s", password);
        INFO info = user_login(username, password);
        if (info.state == false) {
            printf("登录失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_MAIN);
        } else {
            current_user = info.ptr;
            switch_page(PAGE_SWITCH);
        }
        break;
    }
    case '2': {
        char username[256];
        char password[256];
        char password2[256];
        printf("请输入用户名：");
        scanf("%s", username);
        printf("请输入密码：");
        scanf("%s", password);
        printf("请再次输入密码：");
        scanf("%s", password2);
        if (strcmp(password, password2) != 0) {
            printf("两次输入密码不一致，请重新注册！\n");
            switch_page_wait(PAGE_MAIN);
            break;
        }
        INFO info = user_register(username, password, false);
        if (info.state == false) {
            printf("注册失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_MAIN);
        } else {
            current_user = info.ptr;
            switch_page(PAGE_SWITCH);
        }
        break;
    }
    case '3':
        exited = true;
        break;
    }
}

static void handle_page_switch(char ch) {
    switch (ch) {
    case '1': 
        if (current_user->is_admin == false) ch++;
        else {
            switch_page(PAGE_ADMIN);
            break;
        }
    case '2':
        switch_page(PAGE_BOOK);
        break;
    case '3': {
        char password[256];
        char password2[256];
        printf("请输入新密码：");
        scanf("%s", password);
        printf("请再次输入密码：");
        scanf("%s", password2);
        if (strcmp(password, password2) != 0) {
            printf("两次输入密码不一致，请重新输入！\n");
            switch_page_wait(PAGE_SWITCH);
            break;
        }
        INFO info = user_change_pwd(current_user->name, password);
        if (info.state == false) {
            printf("修改密码失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_SWITCH);
        } else {
            printf("修改密码成功！\n");
            switch_page_wait(PAGE_SWITCH);
        }
        break;
    }
    case '4':
        current_user = NULL;
        switch_page(PAGE_MAIN);
        break;
    }
}

static void handle_page_book(char ch) {
    switch (ch) {
    case '1': {
        char bookname[256];
        printf("请输入图书名：");
        scanf("%s", bookname);
        INFO info = book_search(bookname);
        if (info.state == false) {
            printf("查询失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_BOOK);
        }
        else {
            printf("查询成功！%s\n", info.msg);
            print_book_info(info.ptr);
            switch_page_wait(PAGE_BOOK);
        }
        break;
    }
    case '2': {
        INFO info = book_list();
        if (info.state == false) {
            printf("查看失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_BOOK);
        }
        books = info.ptr;
        printf("按任意键滚动，按q退出查看\n");
        for (book_idx = 0; book_idx < 10 && book_idx < vector_size(books); book_idx++) {
            print_book_info(vector_get(books, book_idx));
        }
        roll_mode = true;
        roll_update_func = roll_books;
        break;
    }
    case '3': {
        char bookname[256];
        printf("请输入要借阅图书名：");
        scanf("%s", bookname);
        INFO info = book_borrow(current_user, bookname);
        if (info.state == false) {
            printf("借阅失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_BOOK);
        }
        else {
            printf("借阅成功！\n");
            print_book_info(info.ptr);
            switch_page_wait(PAGE_BOOK);
        }
    }
    case '4':
    }
}

void message_loop() {
    while (!exited) {
        char ch = getch();
        if (roll_mode) {
            if (ch == 'q') {
                roll_mode = false;
                roll_update_func = NULL;
                refresh_page();
            }
            if (roll_update_func) (*roll_update_func)();
            continue;
        }
        if (wait_confirm) {
            wait_confirm = false;
            refresh_page();
            continue;
        }
        switch (current_page) {
        case PAGE_MAIN:
            handle_page_main(ch);
            break;
        case PAGE_SWITCH:
            handle_page_switch(ch);
            break;
        case PAGE_BOOK:
            handle_page_book(ch);
            break;
        }
    }
}