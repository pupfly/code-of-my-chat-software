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
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <sys/select.h>

#define PORT 6666        //端口
#define LIST_MAX 500     //等待队列长度
#define MSG_MAX_L 100   //信息最大长度
#define SERVER_IP "192.168.1.105"
#define NAME_L 16
#define STUD struct stud

typedef struct//信息结构体,flag为1表示未发送
{
  int flag;
  char msg[MSG_MAX_L];
}MSG;

typedef struct//用户结构体
{
  char username[NAME_L];
  int fd;
}USER;

struct stud
{
  USER user;
  struct stud *next;
};

STUD head;

void my_err(char *,int line);//自定义错误函数

int input_msg(char *, char *, char *, int);//自定义输入字符串函数

void add_to_stud(STUD *head , USER user);//链表插入函数,保存登录的用户信息,套接字和姓名

STUD *search_stud(STUD *head, char name_to_find[NAME_L]);//搜索链表信息,返回节点指针

int send_msg(char *, char *,  char *, int, int);//发送数据函数,缓存区,发送者,接受者,描述符,缓存大小

int receive_msg(char *,int, int);//接收数据函数

void *serve_chat(void *arg);//扫描文件并发送信息

void *serve_write(void *arg);//收到当前用户信息,写进对方文件,注意参数

void *client_recv(void *arg);//客户端接收

void *client_send(void *arg);//客户端发送
