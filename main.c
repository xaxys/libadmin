/*
 * @Author: your name
 * @Date: 2020-12-04 21:57:56
 * @LastEditTime: 2020-12-08 15:11:12
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\main.c
 */
#include "view.h"
#include "dao.h"
#include <stdio.h>

int main() {
    db_init();
    switch_page(PAGE_MAIN);
    message_loop();
    db_close();
}