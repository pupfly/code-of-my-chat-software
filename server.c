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
	int server_fd, client_addr_len;
	int r_len;//接收数据的长度
	int optval = 1;
	int clients = 0;
	int t;
	char s_msg[MSG_MAX_L];
	char r_msg[MSG_MAX_L];
	pthread_t pid[LIST_MAX];
	struct sockaddr_in server_addr, client_addr;
	USER user[LIST_MAX];
	
	client_addr_len = sizeof(client_addr);

	head.next = NULL;
	
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

	while(clients < LIST_MAX)
	{
	  if ((user[clients].fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0)
		{
			my_err("accept", __LINE__);
		}
	  printf("Detected New Client,IP:%s\n",inet_ntoa(client_addr.sin_addr));
	  t = user[clients].fd;
	  printf("clients: %d,id:--%d\n",clients,user[clients].fd);
	 /* if (pid == 0)
	  {
	    USER user, other;
	    MSG msg;
	    FILE *fp;
	    char filename[12];
	    user.fd = client_fd;
	    pthread_t tid1, tid2;*/
	//	while(1)
	//	{
			//send_msg(s_msg, client_fd, sizeof(s_msg));
	 /*   printf("%d\n",client_fd);*/
			r_len = receive_msg(user[clients].username, user[clients].fd, sizeof(user[clients].username));
			user[clients].fd = t;
			printf("%s,--%d\n",user[clients].username, user[clients].fd);
			add_to_stud(&head,user[clients]);
			pthread_create(&pid[clients],NULL,serve_chat,(void *)&user[clients]);
			clients++;
		//	pthread_create(&tid2,NULL,serve_write,(void *)&other);
	//	}
	  }
	 /* else
	  {
		  close(client_fd);
	  }
	}*/
	return 0;
}

