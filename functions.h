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
#define LIST_MAX 500     //等待队列长度,系统可服务的最大客户数
#define MSG_MAX_L 100   //每条消息信息最大长度
#define SERVER_IP "222.24.51.63" //服务端IP
#define NAME_L 16      //用户名长度
#define STUD struct stud     //重命名链表结构体
#define ADDR_L 12      //用户地址长度,用于注册
#define SIG_L 50         //个性签名长度
#define PSD_L 16   //密码长度
#define IS_EXIT(a)   if (strcmp(a,"exit") == 0) return//判断是否要退出

typedef struct//信息结构体,flag为1表示未发送
{
  int flag;
  char msg[MSG_MAX_L];
}MSG;

typedef struct//用于保存在线客户的套接字的用户结构体
{
  char username[NAME_L];
  int fd;
  int *order;//登陆顺序,相当于主函数记录登陆的客户端数目的clients,如果注册或登陆失败要利用它修改clients的值
  char result;//存储是否能正确上线.注册等状态y/n
}USER;

typedef struct
{
  char type;//普通用户或管理员
  char name[NAME_L];//姓名
  char password[PSD_L];//密码
  char addr[ADDR_L];//地址
  char sex[4];//性别
  char signature[SIG_L];
}CLIENT;

struct stud
{
  USER user;
  struct stud *next;
};

STUD head;

void my_err(char *,int line);//自定义错误函数

int input_msg(char *, char *, char *, int);//自定义输入信息函数,信息会被处理为含发送和接受者的格式

int input_string(char string[], int str_len);//自定义输入函数,代替gets

void menu();//在客户端打印菜单的函数

void add_to_stud(STUD *head , USER user);//链表插入函数,向系统登记在线的用户信息,包括套接字和姓名

void delete_from_stud(STUD *head , USER user);//链表删除节点函数,向系统注销登陆,即下线

STUD *search_stud(STUD *head, char name_to_find[NAME_L]);//搜索链表信息,返回节点指针

int send_msg(char *, char *,  char *, int, int);//发送数据函数,缓存区,发送者,接受者,描述符,缓存大小

int receive_msg(char *,int, int);//接收数据函数

void send_to_one(char *, char *, char *, int fd);//从消息里面解析出接受者并向其发送

void *serve_chat(void *arg);//扫描文件并发送信息

void *serve_write(void *arg);//收到当前用户信息,写进对方文件,注意参数

void *client_recv(void *arg);//客户端接收

void *client_send(void *arg);//客户端发送

void authorise_server(int fd, char username[NAME_L], char *result);//用户注册函数,服务端

void authorise_client(int fd, USER *user);//用户注册函数,客户端

int is_name_used (int *flag,char *name);//查询用户名是否已被使用,被使用返回1,未被使用返回0

int is_client_exist(int *flag, char *person, char *password);//查询用户是否存在,存在则复制用户密码至password并将flag置1,否则将flag置0,将password置空

void login_serve(int fd, char username[NAME_L], char *result);//登陆函数,用于服务端

void login_client(int fd, char username[NAME_L], char *result);//登陆函数,用于客户端
