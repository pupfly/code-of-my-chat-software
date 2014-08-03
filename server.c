/*
 * =====================================================================================
 *
 *       Filename:  server.c
 *
 *    Description:  code of server
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
	int server_fd,client_fd,client_addr_len;
	int r_len;//接收数据的长度
	int optval = 1;
	char s_msg[MSG_MAX_L];
	char r_msg[MSG_MAX_L];
	pid_t pid;
	struct sockaddr_in server_addr, client_addr;
	client_addr_len = sizeof(client_addr);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		my_err("socket", __LINE__);
	}

	if ((setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int))) < 0)
	{
		my_err("setsockopt", __LINE__);
	}

	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) < 0)
	{
		my_err("bind", __LINE__);
	}
	
	if (listen(server_fd,LIST_MAX) < 0)
	{
		my_err("listen", __LINE__);
	}

	while(1)
	{
	  if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0)
		{
			my_err("accept", __LINE__);
		}
	  printf("Detected New Client,IP:%s\n",inet_ntoa(client_addr.sin_addr));
	  pid = fork();
	  if (pid == 0)
	  {
		while(1)
		{
			printf("Key in your word>>:");
			send_msg(s_msg, client_fd, sizeof(s_msg));
//			memset(s_msg,0,sizeof(s_msg));
//			gets(s_msg);
//			send(client_fd,s_msg,strlen(s_msg),0);
//			memset(r_msg,0,sizeof(r_msg));
			r_len = receive_msg(r_msg, client_fd, sizeof(r_msg));
			if (strcmp(r_msg,"server_exit") == 0)
			  break;
			if (r_len > 0)
			  printf("Client: %s\n",r_msg);
			else
			  printf("Client:\n");
		}
	  }
	  else
	  {
		  close(client_fd);
	  }
	}
	return 0;
}

