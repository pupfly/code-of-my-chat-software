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

int main(int argc, char *argv[])
{
	int server_addr_len, *status1, *status2;
	int r_len;
	char s_msg[MSG_MAX_L];
	char r_msg[MSG_MAX_L];
	struct sockaddr_in server_addr;
	USER user;
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
	  menu();
	  input_string(s_msg,sizeof(s_msg));
	  switch(s_msg[0])
	  {
	    case '1':
	      send(user.fd, s_msg, strlen(s_msg), 0);
	      authorise_client(user.fd,&user);
	      if (user.result == 'y')
		break;
	      else
	      {
		close(user.fd);
		return;
	      }
	      break;
	    case '2':
	      send(user.fd, s_msg, strlen(s_msg), 0);
	      login_client(user.fd, user.username,&(user.result));
	      if (user.result == 'y')
		break;
	      else
	      {
		close(user.fd);
		return;
	      }
	      break;
	  }
	}
	
	pthread_create(&pid1, NULL, client_recv,(void *)&user);
	pthread_create(&pid2, NULL, client_send,(void *)&user);
	
	pthread_join(pid1,(void *)&status1);
	pthread_join(pid2,(void *)&status2);
	return 0;
}

