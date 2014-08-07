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
//	while(1)
	{
//		r_len = 0;
//		r_len = receive_msg(r_msg, client_fd,sizeof(r_msg));
//		memset(r_msg,0,sizeof(r_msg));
//		r_len = recv(client_fd,r_msg,sizeof(r_msg),0);
//		if (r_len > 0)
//		  printf("Server: %s\n",r_msg);
//		else
//		  printf("Server:NULL\n");
		printf("%d\n",user.fd);
	//	printf("Key in your name>>:");
		authorise_client(user.fd);
		//scanf("%s",user.username);
		//send(user.fd, user.username, sizeof(user.username), 0);
		pthread_t pid1, pid2;
		pthread_create(&pid1, NULL, client_recv,(void *)&user);
		pthread_create(&pid2, NULL, client_send,(void *)&user);
		
		pthread_join(pid1,(void *)&status1);
		pthread_join(pid2,(void *)&status2);
//		memset(s_msg,0,sizeof(s_msg));
//		gets(s_msg);
//		send(client_fd,s_msg,strlen(s_msg),0);
		//if (strcmp(s_msg,"client_exit") == 0)
		  //break;
		
	}
	//close(client_fd);
	return 0;
}

