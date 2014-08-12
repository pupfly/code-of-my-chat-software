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
#include <signal.h>

#define PORT 6666        //端口
#define LIST_MAX 500     //等待队列长度,系统可服务的最大客户数
#define MSG_MAX_L 100   //每条消息信息最大长度
#define SERVER_IP "127.0.0.1" //服务端IP
#define NAME_L 16      //用户名长度
#define STUD struct stud     //重命名链表结构体
#define ADDR_L 12      //用户地址长度,用于注册
#define SIG_L 50         //个性签名长度
#define PSD_L 16   //密码长度
#define IS_EXIT(a,b)   if (strcmp(a,"exit") == 0) {send(b, "exit", sizeof("exit") + 1, 0);return;}//判断是否要退出
#define MSG_L sizeof(MSG) //消息结构体大小

typedef struct//信息结构体
{
  char command[24];//消息类型,single_chat私聊,group_chat为群聊,sys_authorize为注册命令,sys_login为登陆命令
  int flag;//flag为1表示已发送
  char sender[NAME_L];//发送者
  char receiver[NAME_L];//接受者
  char msg[MSG_MAX_L];//信息
}MSG;

typedef struct
{
  char type;
  char name[NAME_L];
}LINKMAN;

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
  char id[NAME_L];
  char password[PSD_L];//密码
  char addr[ADDR_L];//地址
  char sex[4];//性别
  char signature[SIG_L];//个性签名
}CLIENT;

struct stud
{
  USER user;
  struct stud *next;
};

STUD head;

void my_err(char *,int line);//自定义错误函数

int input_string(char string[], int str_len);//自定义输入函数,代替gets

void mainmenu();//在客户端打印菜单的函数

void submenu(char username[NAME_L]);//在客户端打印子菜单的函数

void add_to_stud(STUD *head , USER user);//链表插入函数,向系统登记在线的用户信息,包括套接字和姓名

void delete_from_stud(STUD *head , USER user);//链表删除节点函数,向系统注销登陆,即下线

STUD *search_stud(STUD *head, char name_to_find[NAME_L]);//搜索链表信息,返回节点指针

int send_msg(MSG *msg, int fd, char cmd[24]);//发送数据函数,缓存区,发送者,接受者,描述符,缓存大小

int receive_msg(MSG *,int, int);//接收结构体数据函数

int receive(char *,int, int);//接收字符数据函数

void send_to_one(MSG*, int);//从消息里面解析出接受者并向其发送

void *serve_chat(void *arg);//扫描文件并发送信息

void *serve_save_msg(void *arg);//收到当前用户信息,对方不在线,将消息写进未发送链表,注意参数

void *client_recv(void *arg);//客户端接收

void *client_send(void *arg);//客户端发送

void authorise_server(int fd, char username[NAME_L], char *result);//用户注册函数,服务端

void authorise_client(int fd, USER *user);//用户注册函数,客户端

int is_name_used (int *flag,char *name);//查询用户名是否已被使用,被使用返回1,未被使用返回0

int is_client_exist(int *flag, char *person, char *password);//查询用户是否存在,存在则复制用户密码至password并将flag置1,否则将flag置0,将password置空

void login_serve(int fd, char username[NAME_L], char *result);//登陆函数,用于服务端

void login_client(int fd, char username[NAME_L], char *result);//登陆函数,用于客户端

void create_serve(MSG *msg, USER user);//创建群或添加联系人,用flag,标记功能,用于服务端

void add_friend_client(MSG *msg, int fd);//添加好友,也可向群里面添加,用flag标记功能,用于客户端

void show_me(MSG *msg, USER user);//显示用户信息

void show_myfriend(MSG *msg, USER user);//显示所有好友包括群

void show_friend_online(MSG *msg, USER user);//显示在线好友

void show_mygroup(MSG *msg, USER user);//显示用户的群

void manage_group(MSG *msg, USER user);//群主用此函数管理群

void manage_group_client(MSG* msg, USER user);//群管理的客户端函数

