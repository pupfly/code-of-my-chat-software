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
	exit(1);
}

int input_msg(char msg[], char sender[], char receiver[], int msg_len)
{
	int len = 0;
	char c;
	memset(msg, 0, msg_len);
	strcpy(msg,"s ");
	strcat(msg,sender);
	strcat(msg," ");
	strcat(msg,receiver);
	len = strlen(msg);
	msg[len++] = ' ';
	while (((c = getchar()) != '\n') && (len < MSG_MAX_L))
	{
		msg[len++] = c;
	}
	return len;
}

void add_to_stud(STUD *head , USER user)//链表插入函数,保存登录的用户信息,套接字和姓名
{
  	STUD  *s1,*s2;
	s1 = head;
	s2=(STUD *)malloc(sizeof(STUD));
	s2->user = user;
	s2->next = s1->next;
	s1->next = s2;
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

int send_msg(char msg[],char sender[], char receiver[],  int fd,int msg_len)
{
	int len = 0;//记录信息长度

	len = input_msg(msg,sender, receiver, msg_len);

	if (strlen(msg) > (strlen(sender) + strlen(receiver) + 4))
	{
		if (send(fd, msg, strlen(msg), 0) < 0)
			my_err("send",__LINE__);
		return len;
	}
	else
	{
		return 0;
	}
}

int receive_msg(char msg[], int fd,int msg_buf_len)
{
	int len = 0;//记录信息长度

	memset(msg, 0, sizeof(msg));
	
	if ((len = recv(fd, msg, msg_buf_len, 0)) < 0)
	{
		printf("receive error\n");
		return -1;
	}
	msg[len] = '\0';
	return len;
}

void *serve_chat(void *arg)
{
  USER *user = (USER *)arg;
  //printf("user:%s,id:%d",user->username,user->fd);
  char msg_r[MSG_MAX_L];
  char msg_to_send[MSG_MAX_L];
  char receiver[NAME_L];
  int i = 0, j = 0,t = 0, k = 0;
  STUD *p;
  while(1)
  {
    memset(msg_to_send, 0, MSG_MAX_L);
    memset(receiver, 0, NAME_L);
    receive_msg(msg_r, user->fd, MSG_MAX_L);
    if (msg_r[0] == 's')
    {
      while(msg_r[i++] != '\0')
      {
	if (msg_r[i - 1] == ' ')
	{
	  j++;
	  if (j == 2)
	  {
	    t = i;
	    while(msg_r[t++] != ' ')
	    {
	      receiver[k++] = msg_r[t - 1];
	    }
	    receiver[k] = '\0';
	  }
	  if (j == 3)
	    break;
	}
      }
      j = 0;
      while(msg_r[i++] != '\0')
      {
	msg_to_send[j++] = msg_r[i - 1];
      }
      msg_to_send[j] = '\0';
      p = search_stud(&head, receiver);
      if (p != NULL)
      {
	if (send(p->user.fd, msg_to_send, strlen(msg_to_send), 0) == -1)
	{
	  my_err("send",__LINE__);
	}
      }
      else
      {
	send(user->fd, "无此联系人!",strlen("无此联系人!"), 0);
      }
    }
    i = 0, j = 0, k = 0, t = 0;
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
  char msg_r[MSG_MAX_L];
  while(1)
  {
    receive_msg(msg_r,user->fd,sizeof(msg_r));
    printf("%s\n",msg_r);
  }
}

void *client_send(void *arg)
{
  USER *user = (USER *)arg;
  char msg_s[MSG_MAX_L];
  char receiver[NAME_L];
  
  printf("key in receiver's name under this tip ! \n");
  scanf("%s",receiver);
  
  while(1)
  {
    send_msg(msg_s, user->username, receiver, user->fd, sizeof(msg_s));
  }
}
