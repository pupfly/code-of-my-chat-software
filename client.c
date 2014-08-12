/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  code of client
 *
 *        Version:  1.0
 *        Created:  2014年08月02日 12时25分04秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  pupengfei, 792570367@qq.com
 *        Company:  NONE
 *
 * =====================================================================================
 */

#include "functions.h"

void print_tip(int signo)
{
  printf("警告:不支持强制退出!\n");
  return;
}

int main(int argc, char *argv[])
{
	sigset_t newmask,oldmask;
	signal(SIGINT, print_tip);
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGINT);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	int server_addr_len, *status1, *status2;
	int r_len;
	char s_msg[MSG_MAX_L];
	char r_msg[MSG_MAX_L];
	struct sockaddr_in server_addr;
	USER user;
	MSG msg;
	pthread_t pid1, pid2;
	
	user.result = 'n';
	server_addr_len = sizeof(server_addr);
	
	if ((user.fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		my_err("socket", __LINE__);
	}
	char addr[] = "6666";
	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(addr));
	if (inet_aton(SERVER_IP,&server_addr.sin_addr) == 0)
	{
		my_err("inet_aton",__LINE__);
	}
	if ((connect(user.fd, (struct sockaddr *)&server_addr, server_addr_len)) < 0)
	{
		my_err("connect", __LINE__);
	}
	else
	  printf("Msg From Server:Welcom !\n");
	printf("你的套接字描述符: %d\n",user.fd);
	while(user.result != 'y')
	{
	  mainmenu();
	  input_string(s_msg,sizeof(s_msg));
	  switch(s_msg[0])
	  {
	    case '1':
	      strcpy(msg.command,"sys_authorise");
	      send(user.fd, &msg, MSG_L, 0);
	      authorise_client(user.fd,&user);
	      if (user.result == 'y')
		break;
	      else
	      {
	      send(user.fd,"exit", 4, 0);
	      *user.order--;
	      close(user.fd);
	      printf("即将退出,欢迎下次使用!\n");
	      sleep(1);
	      return;
	      }
	      break;
	    case '2':
	      strcpy(msg.command,"sys_login");
	      send(user.fd, &msg, MSG_L, 0);
	      login_client(user.fd, user.username,&(user.result));
	      if (user.result == 'y')
		break;
	      else
	      {
		strcpy(msg.command,"client_shutdown");
	      send(user.fd, &msg, MSG_L, 0);
	      *user.order--;
	      close(user.fd);
	      printf("即将退出,欢迎下次使用!\n");
	      sleep(1);
		return;
	      }
	      break;
	    case '3':
	     strcpy(msg.command,"client_shutdown");
	      send(user.fd, &msg, MSG_L, 0);
	      *user.order--;
	      close(user.fd);
	      printf("即将退出,欢迎下次使用!\n");
	      sleep(1);
	      return;
	  }
	}
	
	pthread_create(&pid1, NULL, client_recv,(void *)&user);
	pthread_create(&pid2, NULL, client_send,(void *)&user);
	
	pthread_join(pid1,(void *)&status1);
	pthread_join(pid2,(void *)&status2);
	return 0;
}

