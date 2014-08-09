/*
 * =====================================================================================
 *
 *       Filename:  fuctions.c
 *
 *    Description:  definations of all functions
 *
 *        Version:  1.0
 *        Created:  2014年08月02日 15时13分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  pupengfei, 792570367@qq.com
 *        Company:  NONE
 *
 * =====================================================================================
 */

#include "functions.h"

void my_err(char *str, int line)
{
	fprintf(stderr, "line:%d ", line);
	perror(str);
	return;
}

void mainmenu()//在客户端打印菜单的函数
{
  printf("--------飞哥传书--------\n");
  printf("       1.注册账号\n");
  printf("       2.用户登录\n");
  printf("       3.退出系统\n");
  printf("请选择:");
}

void submenu(char username[NAME_L])//在客户端打印子菜单的函数
{
  printf("--------%s--------\n",username);
  printf("       1.我的资料\n");
  printf("       2.我的好友\n");
  printf("       3.在线好友\n");
  printf("       4.我加的群\n");
  printf("       5.添加好友\n");
  printf("       6.管理的群\n");
  printf("       7.好友私聊\n");
  printf("       8.开启群聊\n");
  printf("       9.修改资料\n");
  printf("       0.注销登录\n");
  printf("请选择:");
}


int input_string(char string[], int str_len)//自定义输入函数,代替gets
{
	int len = 0;
	char c;
	memset(string, 0, str_len);
	while (((c = getchar()) != '\n') && (len < str_len))
	{
		string[len++] = c;
	}
	return len;
}

void add_to_stud(STUD *head , USER user)//链表插入函数,向系统登记在线的用户信息,包括套接字和姓名,即上线
{
  	STUD  *s1,*s2;
	s1 = head;
	s2=(STUD *)malloc(sizeof(STUD));
	s2->user = user;
	s2->next = s1->next;
	s1->next = s2;
	return;
}

void delete_from_stud(STUD *head , USER user)//链表删除节点函数,向系统注销登陆,即下线
{
  STUD *p, *q;
  p = head;
  while(p != NULL)
  {
    q = p->next;
    if (strcmp(q->user.username,user.username) == 0)
    {
      p->next = q->next;
      free(q);
      return;
    }
    p = p->next;
  }
  return;
}

STUD *search_stud(STUD *head, char name_to_find[NAME_L])//搜索链表信息,返回节点指针
{
  STUD *p;
  p = head;
  while ((p = p->next) != NULL)
  {
    if (strcmp(p->user.username,name_to_find) == 0)
    {
      return p;
    }
  }
  return NULL;
}

int send_msg(MSG *msg, int fd, char cmd[24])
{
	int len = 0;//记录信息长度

	len = input_string((*msg).msg,MSG_MAX_L);

	if (strcmp(cmd,"single_chat") == 0)
	{
	  while(strcmp((*msg).msg,"exit") != 0)
	  {
	    if (len > 0)
	    {
	      if (send(fd, msg, sizeof(MSG), 0) < 0)
	      {
		my_err("send",__LINE__);
	      }
	    }
	  printf("%s:",msg->sender);
	  len = input_string((*msg).msg,MSG_MAX_L);
	  }
	}
	else
	{
		return 0;
	}
}

int receive_msg(MSG *msg, int fd, int msg_buf_len)
{
	int len = 0;//记录信息长度

	memset(msg, 0, sizeof(MSG));
	
	if ((len = recv(fd, msg, msg_buf_len, 0)) < 0)
	{
		printf("receive error\n");
		return -1;
	}
	return len;
}

int receive(char msg[],int fd, int msg_len)//接收字符数据函数
{
  int len = 0;
  
  memset(msg, 0, msg_len);
  
  if ((len = recv(fd, msg, msg_len, 0)) < 0)
	{
		printf("receive error\n");
		return -1;
	}
	return len;
}

void send_to_one(MSG *msg_r, int fd)//从消息里面解析出接受者并向其发送
{
  STUD *p;
  MSG msg_s;
  memset(&msg_s, 0, MSG_L);
  
  p = search_stud(&head,(* msg_r).receiver);
  if (p != NULL)
      {
	if (send(p->user.fd, msg_r, MSG_L, 0) == -1)
	{
	  my_err("send",__LINE__);
	}
      }
  else
  {
    strcpy(msg_s.command,"single_chat");
    strcpy(msg_s.msg,"无此联系人,请输入exit返回!");
    strcpy(msg_s.sender,"服务器");
    send(fd, &msg_s, MSG_L, 0);
  }
}

void *serve_chat(void *arg)
{
  USER *user = (USER *)arg;
  STUD *p;
  MSG msg_cmd;
  
  
  user->result = 'n';
  
  while(1)
  {
    memset(&msg_cmd, 0, sizeof(MSG));
    if (receive_msg(&msg_cmd, user->fd, sizeof(MSG)) < 0)
      break;
    if (strcmp(msg_cmd.command,"single_chat") == 0)
    {
      send_to_one(&msg_cmd, user->fd);
    }
    else if (strcmp(msg_cmd.command,"sys_authorise") == 0)
    {
      authorise_server(user->fd, user->username, &(user->result));
      if (user->result == 'y')
      {
	add_to_stud(&head,*user);
      }
      else
      {
	close(user->fd);
	(*user->order)--;
	pthread_exit(0);
      }
    }
    else if (strcmp(msg_cmd.command,"sys_login") == 0)
    {
      login_serve(user->fd, user->username, &(user->result));
      if (user->result == 'y')
      {
	add_to_stud(&head,*user);
      }
      else
      {
	close(user->fd);
	(*user->order)--;
	pthread_exit(0);
      }
    }
  }
}



/*void *serve_write(void *arg)
{
  printf("serev serve_write110\n");
  USER *user = (USER *)arg;
  FILE *fp;
  MSG msg;
  char filename[12],command[24];
  strcpy(command, "mv -f ");
  if (strcmp(user->username,"a") == 0)
  {
	  strcpy(filename,"bt");
	  strcat(command,"bt b");
  }
  else
  {
	  strcpy(filename,"at");
	  strcat(command,"at a");
  }
  while(1)
  {
    memset(&msg,0,sizeof(msg));
    receive_msg(msg.msg,user->fd,sizeof(msg.msg));
    msg.flag = 1;
     fp = fopen(filename,"a");
    fwrite(&msg,sizeof(msg),1,fp);
    fclose(fp);
	system(command);
  }
}
*/
void *client_recv(void *arg)
{
  USER *user = (USER *)arg;
  MSG msg_r;
  while(1)
  {
    receive_msg(&msg_r,user->fd,MSG_L);
    if (strcmp(msg_r.command,"single_chat") == 0)
    {
      printf("%s: %s\n",msg_r.sender,msg_r.msg);
    }
  }
}

void *client_send(void *arg)
{
  USER *user = (USER *)arg;
  MSG msg_s;
  int choice = 0;
   
  while(1)
  {
    submenu(user->username);
    scanf("%d",&choice);
    switch(choice)
    {
      case 7:
	printf("输入对方名称:");
	scanf("%s",msg_s.receiver);
	strcpy(msg_s.sender,user->username);
	strcpy(msg_s.command,"single_chat");
	send_msg(&msg_s, user->fd, msg_s.command); 
	break;
    }
  }
}

int is_name_used (int *flag, char *name)//查询用户名是否已被使用,被使用返回1,未被使用返回0
{
  FILE *fp;
  CLIENT client;
  
  fp = fopen("Clients","r");
  while(fread(&client, sizeof(client), 1, fp) != NULL)
  {
    if (strcmp(client.name,name) == 0)
    {
      *flag = 1;
      fclose(fp);
      return *flag;
    }
  }
  *flag = 0;
  fclose(fp);
  return *flag;
}

int is_client_exist(int *flag, char *name, char password[PSD_L])//查询用户是否存在,存在则复制用户密码至password并将flag置1,否则将flag置0,将password置空
{
  FILE *fp;
  CLIENT client;
  int i = 0;
  memset(password, 0, sizeof(password));
  
  fp = fopen("Clients","r");
  while(fread(&client, sizeof(client), 1, fp) != NULL)
  {
    if (strcmp(client.name,name) == 0)
    {
      *flag = 1;
      while (client.password[i++] != '\0')
      {
	password[i - 1] = client.password[i - 1] + 1;
      }
      password[i - 1] = '\0';
      fclose(fp);
      return *flag;
    }
  }
  *flag = 0;
  fclose(fp);
  return *flag;
}

void authorise_server(int fd, char username[NAME_L], char *result)//用户注册函数,服务端
{
  FILE *fp;
  int flag = 1;
  CLIENT client;
  
  *result = 'n';
  
  receive(client.name,fd,sizeof(client.name));
  IS_EXIT(client.name,fd);
  is_name_used(&flag, client.name);
while(is_name_used(&flag, client.name))
{
      send(fd, "fail",strlen("fail"), 0);
       receive(client.name,fd,sizeof(client.name));
       IS_EXIT(client.name,fd);
}

send(fd, "success",strlen("success"), 0);

while(receive(client.password, fd, sizeof(client.password)) <= 0)
{
  IS_EXIT(client.password,fd);
  send(fd, "fail",strlen("fail"), 0);
}

send(fd, "success",strlen("success"), 0);

while(receive(client.addr, fd, sizeof(client.addr)) <= 0)
{
  IS_EXIT(client.addr,fd);
  send(fd, "fail",strlen("fail"), 0);
}

send(fd, "success",strlen("success"), 0);

while(receive(client.sex, fd, sizeof(client.sex)) <= 0)
{
  send(fd, "fail",strlen("fail"), 0);
}

send(fd, "success",strlen("success"), 0);

while(receive(client.signature, fd, sizeof(client.signature)) <= 0)
{
  send(fd, "fail",strlen("fail"), 0);
}

client.type = 'C';

if ((fp = fopen("Clients","a")) == NULL)
{
  printf("打开文件出错,注册失败!\n");
}
if (fwrite(&client, sizeof(client), 1, fp) == 0)
{
  printf("写文件出错,注册失败!\n");
}
else
{
  printf("用户: %s 注册成功!\n",client.name);
  *result = 'y';
  send(fd, "success",strlen("success"), 0);
  fclose(fp);
}
send(fd, "success",strlen("success"), 0);
strcpy(username,client.name);
*result = 'y';
return;
}

void authorise_client(int fd, USER *user)//用户注册函数,客户端
{
  CLIENT client;
  char status[8];
  char password_temp[PSD_L];
  
  user->result = 'n';
  
  printf("请输入您的姓名或昵称(1~16)>>:\n");
  while(input_string(client.name, sizeof(client.name)) <= 0)
  {
    IS_EXIT(client.name,fd);
    printf("姓名不能为空,系统要求重新输入!\n");
    printf("请输入您的姓名或昵称(1~16)>>:\n");
  }
  send(fd, client.name,strlen(client.name), 0);
  receive(status, fd, sizeof(status));
  IS_EXIT(status,fd);
  while(strcmp(status,"fail") == 0)
  {
    printf("对不起,系统已存在用户: %s ,请重新输入>>:\n",client.name);
    input_string(client.name, sizeof(client.name));
    IS_EXIT(client.name,fd);
    send(fd, client.name,strlen(client.name), 0);
    receive(status, fd, sizeof(status));
    IS_EXIT(status,fd);
  }
  
  printf("用户名合法!\n请输入您的密码(1~16)>>:\n");
  input_string(client.password, sizeof(client.password));
  IS_EXIT(client.password,fd);
  printf("请确认您的密码(1~16)>>:\n");
  input_string(password_temp, sizeof(password_temp));
  IS_EXIT(password_temp,fd);
  while (strcmp(password_temp,client.password) != 0)
  {
    printf("密码不一致,系统要求重新输入!\n");
    printf("请输入您的密码(1~16)>>:\n");
    input_string(client.password, sizeof(client.password));
    IS_EXIT(client.password,fd);
    printf("请确认您的密码(1~16)>>:\n");
    input_string(password_temp, sizeof(password_temp));
    IS_EXIT(password_temp,fd);
  }
  send(fd, client.password,strlen(client.password), 0);
  receive(status, fd, sizeof(status));
  IS_EXIT(status,fd);
  if (strcmp(status,"fail") == 0)
    memset(password_temp, 0, sizeof(password_temp));
  while(strcmp(status,"fail") == 0)
  {
    printf("输入失败,请重新输入>>:\n");
    while (strcmp(password_temp,client.password) != 0)
    {
      printf("请输入您的密码(1~16)>>:\n");
      input_string(client.password, sizeof(client.password));
      printf("请确认您的密码(1~16)>>:\n");
      input_string(password_temp, sizeof(password_temp));
      IS_EXIT(password_temp,fd);
    }
    send(fd, client.password,strlen(client.password), 0);
    receive(status, fd, sizeof(status));
    IS_EXIT(status,fd);
  }
  
  printf("请输入地址(1~12)>>:\n");
   input_string(client.addr, sizeof(client.addr));
   IS_EXIT(client.addr,fd);
   send(fd, client.addr,strlen(client.addr), 0);
   receive(status, fd, sizeof(status));
    IS_EXIT(status,fd);
  while(strcmp(status,"fail") == 0)
  {
    printf("输入地址失败,请重新输入>>:\n");
    input_string(client.addr, sizeof(client.addr));
    IS_EXIT(client.addr,fd);
   send(fd, client.addr,strlen(client.addr), 0);
   receive(status, fd, sizeof(status));
   IS_EXIT(status,fd);
  }
  
  printf("地址和法!\n请输入性别(男/女)>>:\n");
  input_string(client.sex, sizeof(client.sex));
  IS_EXIT(client.sex,fd);
  while((strcmp(client.sex,"男") != 0) &&(strcmp(client.sex,"女") != 0))
  {
    printf("输入非法,系统要求重新输入!\n");
    printf("请输入性别(男/女)>>:\n");
    input_string(client.sex, sizeof(client.sex));
    IS_EXIT(client.sex,fd);
  }
  send(fd, client.sex, strlen(client.sex), 0);
  receive(status, fd, sizeof(status));
  IS_EXIT(status,fd);
  while(strcmp(status,"fail") == 0)
  {
    printf("输入失败,请重新输入>>:\n");
    input_string(client.sex, sizeof(client.sex));
    IS_EXIT(client.sex,fd);
    send(fd, client.sex, strlen(client.sex), 0);
    receive(status, fd, sizeof(status));
    IS_EXIT(status,fd);
  }
  
  printf("输入成功,请输入个性签名(1~50)>>:\n");
  while(input_string(client.signature, sizeof(client.signature)) == 0);
  send(fd, client.signature, strlen(client.signature), 0);
  receive(status, fd, sizeof(status));
  while(strcmp(status,"fail") == 0)
  {
    printf("输入失败,请重新输入>>:\n");
    input_string(client.signature, sizeof(client.signature));
    send(fd, client.signature, strlen(client.signature), 0);
    receive(status, fd, sizeof(status));
  }
   receive(status, fd, sizeof(status));
   if (strcmp(status,"success") == 0)
   {
     strcpy(user->username,client.name);
     user->result = 'y';
     return;
   }
}

void login_serve(int fd, char username[NAME_L], char *result)//登陆函数,用于服务端
{
  int flag = 0;
  char password[PSD_L];
  char name[NAME_L];
  char status[12];
  *result = 'n';
  
  while(flag == 0)
  {
    receive(name, fd, sizeof(name));
    if (is_client_exist(&flag, name, password))
    {
      strcpy(username,name);
      send(fd, "password",strlen("password"), 0);
      receive(status, fd, sizeof(status));
      if (strcmp(status,"Y") == 0)
      {
	send(fd, password, strlen(password), 0);
	break;
      }
    }
    else
      send(fd, "userillegal", strlen("userillegal"), 0 );
  }
  receive(status, fd, sizeof(status));
  if (strcmp(status,"continue") == 0)
  {
    *result = 'n';
    return;
  }
  else if (strcmp(status,"logged") == 0)
  {
    *result = 'y';
    return;
  }
  *result = 'n';
  return;
}

void login_client(int fd, char username[NAME_L], char *result)//登陆函数,用于客户端
{
  memset(username, 0, sizeof(username));
  char password[PSD_L], password_temp[PSD_L];
  char name[NAME_L];
  char status[12];
  int i = 5, j = 0;
  
  printf("用户名:");
  while(input_string(name, sizeof(name)) <= 0)
  printf("用户名不能为空,请重新输入!\n");
  send(fd, name, sizeof(name), 0);
  receive(status, fd, sizeof(status));
  while(strcmp(status,"password") != 0)
  {
    if (strcmp(status,"userillegal"))
      printf("用户不存在,请重新输入(exit 退出登录!)\n");
    while(input_string(name, sizeof(name)) <= 0)
    {
      if (strcmp(name,"exit") ==0)
      {
	send(fd, "continue", strlen("continue"), 0);
	return;
      }
      printf("用户名不能为空,请重新输入!\n");
    }
    send(fd, name, sizeof(name), 0);
    receive(status, fd, sizeof(status));
  }
  send(fd, "Y", strlen("Y"), 0);
  receive(password, fd, sizeof(password));
  while (i-- > 0 )
  {
    j = 0;
    printf("请输入密码(剩余 %d 机会):\n",i);
    input_string(password_temp, sizeof(password_temp));
    while(password_temp[j++] != '\0')
      password_temp[j - 1] += 1;
    if (strcmp(password,password_temp) == 0)
    {
      printf("登陆成功!\n");
      *result = 'y';
      break;
    }
  }
  if (i < 0)
  {
    memset(username, 0, sizeof(username));
    *result = 'n';
    send(fd, "continue", strlen("continue"), 0);
    return;
  }
  else
  {
    strcpy(username, name);
    send(fd, "logged", strlen("logged"), 0);
  }
  return;
}

/*void create_serve(char flag, USER user)//创建群或添加联系人,用flag,标记功能,用于服务端
{
  
}

void add_friend_client(char flag, USER user)//添加好友,也可向群里面添加,用flag标记功能,用于客户端
{
  char friendname[NAME_L];
  
  if (flag == 'g')
  {
    printf("要添加的好友>>:");
   while(input_string(friendname, sizeof(friendname)) <= 0)
    {
      if (strcmp(friendname,"exit") ==0)
      {
	send(fd, "continue", strlen("continue"), 0);
	return;
      }
      printf("用户名不能为空,请重新输入!\n");
    }
    send(fd, friendname, sizeof(friendname), 0);
  }
  
}*/