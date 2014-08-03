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

int input_msg(char msg[], int msg_len)
{
	int len = 0;
	char c;
	memset(msg, 0, msg_len);
	while (((c = getchar()) != '\n') && (len < MSG_MAX_L))
	{
		msg[len++] = c;
	}
	return len;
}

int send_msg(char msg[], int fd,int msg_len)
{
	int len = 0;//记录信息长度

	len = input_msg(msg, msg_len);

	if (strlen(msg) > 0)
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

