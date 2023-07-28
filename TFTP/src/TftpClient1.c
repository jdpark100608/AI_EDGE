/*
 * TftpClient.c
 *
 *  Created on: Jun 2, 2023
 *      Author: huins
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 20162
#define DATA_PACKET_SIZE 516

int main(void)
{
//	if(argc < 3)
//	{
//		printf("Usage : %s <server ip> <file_name> \n", argv[0]);
//		exit(1);
//	}

	int clnt_sock, n;
	struct sockaddr_in serv_addr;
	socklen_t addr_len;
	char buffer[DATA_PACKET_SIZE];
	char buffer_save[DATA_PACKET_SIZE];

	char ip[] = "192.168.0.10";
	char file_name[] = "test";

	printf("1 file name : %s \n", file_name);

	if((clnt_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		printf("create socket Failed\n");

	printf("2\n");


	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVER_PORT);
	serv_addr.sin_addr.s_addr = inet_addr(ip);


	printf("3\n");


	/* request to send file */
	buffer[0] = 0;
	buffer[1] = 1;
	strcpy(buffer + 2, file_name);

	printf("4, buffer = %s\n", buffer+2);


	addr_len = sizeof(serv_addr);



	/* send requesting to send file */
	if ( sendto(clnt_sock, buffer, strlen(file_name) +3, 0, (struct sockaddr*)&serv_addr, addr_len) < 0)
		printf("Failed sendto()\n");

	printf("5\n");


	printf(" request to send file \n");

	/* receive & save file */
	FILE *file = fopen(file_name, "wb");
	if(file == NULL)
		printf("Failed open file\n");

	printf("6\n");


	int block = 1;
	while(1)
	{
		/* receive DATA packet */
		n = recvfrom(clnt_sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&serv_addr, &addr_len);
		if(n < 0)
			printf("Failed recvfrom()\n");

		printf("7\n");

//		/* check error packet */
//		if(buffer[1] == 5)
//		{
//			printf("error : %s\n", buffer + 4);
//			fclose(file);
//			exit(1);
//		}

		printf("8\n");

		printf("buffer[1]:%d\n", buffer[1]);


		/* check Data packet */
		if(buffer[1] == 3)
		{
			/* Print the entire data packet */
			printf("Received data packet : \n");
			for(int i =0; i<n; i++)
			{
				printf("%c", buffer[i]);
			}
			printf("\n");


			/* send ACK packet */
			buffer[0] = 0;
			buffer[1] = 4;
			buffer[2] = (block >> 8) & 0xFF;
			buffer[3] = block & 0xFF;

			if (sendto(clnt_sock, buffer, 4, 0, (struct sockaddr*)&serv_addr, addr_len) < 0)
				printf("Failed ACK packet send\n");

			/* save Data */
			fwrite(buffer + 4, sizeof(char), n -4, file);

			FILE *file = fopen("test", "rb");
			printf("Saved data packet : \n");
			for(int i =0; i<n; i++)
			{
				printf("%c", buffer_save[i]);
			}
			printf("\n");

			if(n < DATA_PACKET_SIZE)
				break;

			printf("9\n");


			block ++;

			printf("10 block num : %d\n", block);
		}
	}

	printf(" complete sending file\n");
	fclose(file);
	close(clnt_sock);

	return 0;
}
