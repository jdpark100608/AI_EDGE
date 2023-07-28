#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 100
#define port 20162

void error_handling(char* message);
static void TestFunc(unsigned char message[]);

int main(int argc, char* argv[])
{
	int serv_sock;
	unsigned char message[BUF_SIZE];
	unsigned char* mesg[5];

	int str_len;
	socklen_t clnt_adr_sz;
	struct sockaddr_in serv_adr, clnt_adr;

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);

	if(serv_sock == -1)
	{
		error_handling("UDP socket creation error");
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(port);

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
	{
		error_handling("bind creation error");
	}

	while(1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		printf("received message from client : %d \n", str_len );

		TestFunc(&message[0]);

		sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);

	}

	close(serv_sock);

	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

static void TestFunc(unsigned char message[])
{
	static int Cnt = 0;
	printf("[%s] (%d) \nreceived message : %d %d %d\n", __func__, Cnt, message[0], message[1], message[2]);
	printf("received message : %c %c %c %c %c %c %c %c %c %c \n", message[0], message[1], message[2],message[3],message[4],message[5],message[6],message[7],message[8],message[9]);
	printf("received message, s : %s \n\n", message);
	Cnt++;
}












