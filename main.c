/*
 * @Author: your name
 * @Date: 2020-12-04 21:57:56
 * @LastEditTime: 2020-12-13 11:14:08
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\main.c
 */
#include "view.h"
#include "dao.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    system("chcp 65001");
    db_init();
    switch_page(PAGE_MAIN);
    message_loop();
    db_close();
}