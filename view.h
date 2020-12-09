/*
 * @Author: your name
 * @Date: 2020-12-04 22:08:55
 * @LastEditTime: 2020-12-09 09:09:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \libadmin\view.h
 */
#ifndef _VIEW_H_
#define _VIEW_H_

typedef enum {
    PAGE_MAIN,
    PAGE_SWITCH,
    PAGE_ADMIN,
    PAGE_BOOK,
} PAGE;

void switch_page(PAGE page);
void message_loop();

#endif