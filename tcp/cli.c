/*************************************************************************
    > File Name: cli.c
    > Created Time: Thu 15 Nov 2018 08:18:54 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/un.h>



int main()
{
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(sockfd != -1);

	struct sockaddr_in ser,cli;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(6500);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	int err = connect(sockfd, (struct sockaddr*)&ser, sizeof(ser));
	if(err == -1)
	{
		printf("connect error!\n");
	}

	while(1)
	{
		printf("please input:");
		fflush(stdout);

		char buff[1500] = {0};
		fgets(buff, 1500, stdin);
		buff[strlen(buff)-1] = 0;
		if(strncmp(buff,"end",3)==0)
		{
			break;
		}
		
		err = send(sockfd, buff, strlen(buff), 0);
		assert(err != -1);

		char recvbuff[128] = {0};
		err = recv(sockfd, recvbuff, 127, 0);
		assert(err = -1);
		printf("%s\n",recvbuff);

	}
	close(sockfd);
	return 0;
}
