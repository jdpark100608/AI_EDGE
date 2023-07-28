/*
 * TCP Server Example
 * Availiable input Text
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 100
#define port 20162

void error_handling(char *message);
static void TestFunc(unsigned char message[]);

int main(void) {
	int serv_sock, clnt_sock;

	struct sockaddr_in serv_adr, clnt_adr;

//	char sendmsg[] = "serv to clnt";
	char sendmsg[1024];

	char recvmsg[1024];

	char ip[] = "192.168.0.10";

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
	{
		error_handling("socket() error");
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(port);

	int bindret = bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr) );
	if(bindret == -1)
	{
		error_handling("bind() error");
	}

	int listenret = listen(serv_sock, 10);
	if(listenret == -1)
	{
		error_handling("listen() error");
	}

	int clnt_adr_size = sizeof(clnt_adr);
	int acceptret = accept(serv_sock, (struct sockaddr*) NULL, NULL);
	if(acceptret == -1)
	{
		error_handling("accept() error");
	}

	/* Input Text */
	while(1)
	{
//		fputs("insert message(q to quit) : ", stdout);
//		fgets(sendmsg, sizeof(sendmsg), stdin);
//
//		if(!strcmp(sendmsg, "q\n" || !strcmp(sendmsg,"Q\n")))
//		{
//			break;
//		}

		int readstrlen = read(acceptret, &recvmsg[0], sizeof(recvmsg)-1);
		printf("read = %d, recvmsg = %s\n", readstrlen, recvmsg);

//		int writestrlen = write(acceptret, sendmsg, sizeof(sendmsg) );
//		printf("write = %d, sendmsg = %s\n", writestrlen, sendmsg);
	}
//		sleep(5);

	close(serv_sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
