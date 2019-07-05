/*************************************************************************
    > File Name: cli.c
    > Created Time: Sat 17 Nov 2018 08:35:50 PM CST
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
	int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	assert(sockfd != -1);
	
	struct sockaddr_in ser,cli;
	ser.sin_family = AF_INET;
	ser.sin_port = htons(6500);
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");

	while(1)
	{
		char buff[128] = {0};
		printf("please input:");
		fflush(stdout);
		fgets(buff, 127, stdin);
		buff[strlen(buff)-1] = 0;
		if(strcmp(buff,"end") == 0)
		{
			break;
		}
		int err = sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&ser, sizeof(ser));
		assert(err != -1);

		char recvbuff[128] = {0};
		int len = sizeof(cli);
		err = recvfrom(sockfd, recvbuff, 127, 0, (struct sockaddr*)&cli, &len);
		assert(err != -1);
		printf("%s\n", recvbuff);

	}
	close(sockfd);
	return 0;
}
