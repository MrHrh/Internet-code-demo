/*************************************************************************
    > File Name: ser.c
    > Created Time: Thu 15 Nov 2018 07:49:54 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<signal.h>
void func(int c)
{
	while(1)
	{
		char buff[128] = {0};
		int err = recv(c, buff, 127, 0);
		if(err <= 0)
		{
			printf("%dclient disconnect!\n",c);
			break;
		}
		printf("%d: %s\n",c,buff);
		char sendbuff[128] = {0};
		printf("please input:");
		fflush(stdout);
		fgets(sendbuff, 127, stdin);
		err = send(c, sendbuff, strlen(sendbuff)-1, 0);
		if(err == -1)
		{
			printf("error\n");
		}
	}
	close(c);

}

void Zombie(int sign)
{
	wait(NULL);
}

int main()
{
	signal(SIGCHLD, Zombie);
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(sockfd != -1);
	
	struct sockaddr_in ser,cli;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(6500);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");

	int err = bind(sockfd, (struct sockaddr*)&ser, sizeof(ser));
	assert(err != -1);

	err = listen(sockfd, 5);
	assert(err != -1);
	
	while(1)
	{
		int len = sizeof(cli);
		int c = accept(sockfd, (struct sockaddr*)&cli, &len);
		if(c == -1)
		{
			printf("accept error\n");
			continue;
		}
		int n = fork();
		if(n == 0)
		{
			func(c);
			break;
		}
		else
		{
			close(c);
		}
	}
	close(sockfd);
	return 0;
}
