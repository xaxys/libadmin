/*
 * @Author: your name
 * @Date: 2020-12-04 21:58:01
 * @LastEditTime: 2020-12-11 22:02:01
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\view.c
 */
#include "view.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "model.h"
#include "resource.h"
#include "controller.h"

#define UNDEF -0x11037919

PAGE current_page;
USER *current_user;
bool exited = false;
bool wait_confirm;

bool roll_mode;
void (*roll_update_func)();
vector *roll_vector;
int roll_idx;

static void print_user_info(USER *user) {
    printf(VIEW_USER_INFO_FORMAT,
        user->id,
        user->name,
        user->pwd,
        user->is_admin ? "是" : "否",
        vector_size(user->borrowed));
}

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

static void roll_users() {
    roll_idx++;
    if (roll_idx < vector_size(roll_vector)) {
        print_user_info(vector_get(roll_vector, roll_idx));
    } else {
        printf("翻到底啦！按q退出\n");
    }
}

static void roll_books() {
    roll_idx++;
    if (roll_idx < vector_size(roll_vector)) {
        print_book_info(vector_get(roll_vector, roll_idx));
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
    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    char *time_tip;
    if (local->tm_hour >= 22 || local->tm_hour < 6) time_tip = "夜深了";
    else if (local->tm_hour >= 6 && local->tm_hour < 9) time_tip = "早上好";
    else if (local->tm_hour >= 9 && local->tm_hour < 11) time_tip = "上午好";
    else if (local->tm_hour >= 11 && local->tm_hour < 14) time_tip = "中午好";
    else if (local->tm_hour >= 14 && local->tm_hour < 17) time_tip = "下午好";
    else if (local->tm_hour >= 17 && local->tm_hour < 22) time_tip = "晚上好";
    if (current_user->is_admin) {
        printf(VIEW_PAGE_SWITCH_WELCOME_FORMAT_ADMIN, time_tip, current_user->name);
    } else {
        printf(VIEW_PAGE_SWITCH_WELCOME_FORMAT_USER, time_tip, current_user->name);
    }
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
    printf(VIEW_PAGE_ADMIN_TITLE);
    printf(VIEW_SEPERATOR);
    printf(VIEW_PAGE_ADMIN_OPTIONS);
    printf(VIEW_SEPERATOR);
    printf("%s\n", COPYRIGHT);
}

static void print_page_book() {
    printf(VIEW_PAGE_BOOK_TITLE);
    printf(VIEW_SEPERATOR);
    printf(VIEW_PAGE_BOOK_OPTIONS);
    printf(VIEW_SEPERATOR);
    printf("%s\n", COPYRIGHT);
}

static void refresh_page() {
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
    if (current_user->is_admin == false) ch++;
    switch (ch) {
    case '1':
        switch_page(PAGE_ADMIN);
        break;
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
            break;
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

static void handle_page_admin(char ch) {
    switch (ch) {
    case '1': {
        char bookname[256];
        printf("请输入图书名：");
        scanf("%s", bookname);
        INFO info = book_search(bookname, true);
        BOOK *book = info.ptr;
        if (info.state == true) {
            printf("找到一本同名图书：\n");
            print_book_info(info.ptr);
            printf("继续输入将对本书信息进行修改！[Y/n]\n");
            char buf[256];
            scanf("%s", buf);
            if (strcmp(buf, "n") == 0 || strcmp(buf, "N") == 0) break;
            printf("提示：以下项目输入-1表示不修改\n");
        }
        char pubname[256];
        char author[256];
        int price;
        int total;
        printf("请输入出版社：");
        scanf("%s", pubname);
        printf("请输入作者：");
        scanf("%s", author);
        printf("请输入价格：");
        scanf("%d", &price);
        printf("请输入数量：");
        scanf("%d", &total);
        if (info.state == false) {
            info = book_add(bookname, pubname, author, price, total);
        } else {
            info = book_modify(book, bookname, pubname, author, price, total);
        }
        if (info.state == false) {
            printf("添加/修改失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_ADMIN);
            break;
        } else {
            printf("添加/修改成功！\n");
            print_book_info(info.ptr);
            switch_page_wait(PAGE_ADMIN);
        }
        break;
    }
    case '2': {
        int book_id = UNDEF;
        printf("请输入需要删除的图书ID：\n");
        scanf("%d", &book_id);
        if (book_id == UNDEF) {
            printf("请输入一个数字\n");
            switch_page_wait(PAGE_ADMIN);
            break;
        }
        INFO info = book_delete_id(book_id);
        if (info.state == false) {
            printf("删除失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_ADMIN);
            break;
        } else {
            printf("删除成功！\n");
            switch_page_wait(PAGE_ADMIN);
        }
        break;
    }
    case '3': {
        INFO info = user_list();
        if (info.state == false) {
            printf("查看失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_ADMIN);
            break;
        }
        roll_vector = info.ptr;
        printf("总计 %d 名注册用户：\n", vector_size(roll_vector));
        printf("按任意键滚动，按q退出查看\n");
        for (roll_idx = 0;
            roll_idx < DEFAULT_SHOW_ENTRIES &&
            roll_idx < vector_size(roll_vector);
            roll_idx++) {
            USER *user = vector_get(roll_vector, roll_idx);
            print_user_info(user);
        }
        roll_mode = true;
        roll_update_func = roll_users;
        break;
    }
    case '4': {
        char username[256];
        printf("请输入用户名：");
        scanf("%s", username);
        INFO info = user_search(username);
        if (info.state == false) {
            printf("修改失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_ADMIN);
            break;
        }
        USER *user = info.ptr;
        printf("提示：以下项目输入-1表示不修改\n");
        char password[256];
        char admin[256];
        bool is_admin = user->is_admin;
        printf("请输入密码：");
        scanf("%s", password);
        printf("是否为管理员[y/N]：");
        scanf("%s", admin);
        if (strcmp(admin, "y") == 0 || strcmp(admin, "Y") == 0) is_admin = true;
        info = user_modify(user, password, is_admin);
        if (info.state == false) {
            printf("修改失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_ADMIN);
            break;
        } else {
            printf("修改成功！\n");
            print_user_info(info.ptr);
            switch_page_wait(PAGE_ADMIN);
        }
        break;
    }
    case '5': {
        int user_id = UNDEF;
        printf("请输入需要删除的用户ID：\n");
        scanf("%d", &user_id);
        if (user_id == UNDEF) {
            printf("请输入一个数字\n");
            switch_page_wait(PAGE_ADMIN);
            break;
        }
        INFO info = user_delete_id(user_id);
        if (info.state == false) {
            printf("删除失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_ADMIN);
            break;
        }
        else {
            printf("删除成功！\n");
            switch_page_wait(PAGE_ADMIN);
        }
        break;
    }
    case '6':
        switch_page(PAGE_SWITCH);
        break;
    }
}

static void handle_page_book(char ch) {
    switch (ch) {
    case '1': {
        char bookname[256];
        printf("请输入图书名：");
        scanf("%s", bookname);
        INFO info = book_search(bookname, false);
        if (info.state == false) {
            printf("查询失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_BOOK);
            break;
        } else {
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
            break;
        }
        roll_vector = info.ptr;
        printf("馆藏 %d 本图书：\n", vector_size(roll_vector));
        printf("按任意键滚动，按q退出查看\n");
        for (roll_idx = 0;
            roll_idx < DEFAULT_SHOW_ENTRIES &&
            roll_idx < vector_size(roll_vector);
            roll_idx++) {
            BOOK *book = vector_get(roll_vector, roll_idx);
            print_book_info(book);
        }
        roll_mode = true;
        roll_update_func = roll_books;
        break;
    }
    case '3': {
        char bookname[256];
        printf("请输入要借阅图书名：");
        scanf("%s", bookname);
        INFO info = book_borrow_name(current_user, bookname);
        if (info.state == false) {
            printf("借阅失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_BOOK);
            break;
        } else {
            printf("借阅成功！\n");
            print_book_info(info.ptr);
            switch_page_wait(PAGE_BOOK);
        }
        break;
    }
    case '4': {
        INFO info = book_user_borrowed(current_user);
        if (info.state == false) {
            printf("借阅失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_BOOK);
            break;
        } else {
            vector *v = info.ptr;
            printf("您已借阅了 %d 本图书：\n", vector_size(v));
            for (int i = 0; i < vector_size(v); i++) {
                BOOK *book = vector_get(v, i);
                print_book_info(book);
            }
            free_vector(v);
        }
        int book_id = UNDEF;
        printf("请输入需要归还的图书ID：\n");
        scanf("%d", &book_id);
        if (book_id == UNDEF) {
            printf("请输入一个数字\n");
            switch_page_wait(PAGE_ADMIN);
            break;
        }
        info = book_return_id(current_user, book_id);
        if (info.state == false) {
            printf("归还失败！\n原因：%s\n", info.msg);
            switch_page_wait(PAGE_BOOK);
            break;
        } else {
            printf("归还成功！\n");
            switch_page_wait(PAGE_BOOK);
        }
        break;
    }
    case '5':
        switch_page(PAGE_SWITCH);
        break;
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
        case PAGE_ADMIN:
            handle_page_admin(ch);
            break;
        case PAGE_BOOK:
            handle_page_book(ch);
            break;
        }
    }
}