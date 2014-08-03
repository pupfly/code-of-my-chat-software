/*
 * =====================================================================================
 *
 *       Filename:  functions.h
 *
 *    Description:  decelarations of all functions and globle variables 
 *
 *        Version:  1.0
 *        Created:  2014年08月02日 15时09分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  pupengfei, 792570367@qq.com
 *        Company:  NONE
 *
 * =====================================================================================
 */

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 6666        //端口
#define LIST_MAX 500     //等待队列长度
#define MSG_MAX_L 100   //信息最大长度

void my_err(char *,int line);//自定义错误函数

int input_msg(char *, int);//自定义输入字符串函数

int send_msg(char *, int, int);//发送数据函数

int receive_msg(char *,int, int);//接收数据函数
