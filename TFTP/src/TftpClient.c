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

#define SERVER_PORT 69
#define DATA_PACKET_SIZE 516

/* Define TFTP PACKET */
typedef struct
{
	unsigned short opcode;
	union
	{
		unsigned short block_num;
		unsigned short error_code;
		char data[DATA_PACKET_SIZE -4];
	};
} TftpPacket;

/* Define Error Code */
#define ERROR_NOT_DEFINED 0
#define ERROR_FILE_NOT_FOUND 1
#define ERROR_ACCESS_VIOLATION 2

int main()
{
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVER_PORT);
	serv_addr.sin_addr.s_addr = inet_addr("TFTP_SERVER_IP_ADDRESS");

	int clnt_socket = socket(AF_INET, SOCK_DGRAM, 0);
	in(clnt_socket < 0)
	{
		perror("socket() Failed");
		exit(1);
	}

	/* write request packet */
	TftpPacket write_request_packet;
	write_request_packet.opcode = htons(2);
	strcpy(write_request_packet.data, "File name");
	/* wanted Transport File name */

	if( sendto(clnt_sock, &write_request_packet, sizeof(write_request_packet), 0,
			   (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("write packet send() Failed");
		exit(1);
	}

	TftpPacket data_packet;
	data_packet.opcode = htons(3);
	unsigned short block_num = 1;

	while(1)
	{
		size_t bytes_read = fread(data_packet.data, 1, sizeof(data_packet.data), file);

		data_packet.block_num = htons(block_num);
		if( sendto(clnt_sock, &data_packet, bytes_read + 4, 0,
				   (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		{
			perror("Transport data packet Failed()");
			exit(1);
		}

		if(bytes_read < sizeof(data_packet.data))
		{
			break;
		}

		/* ACK packet Received */
		TftpPacket ack_packet;
		if( recvfrom(clnt_sock, &ack_packet, sizeof(ack_packet), 0, NULL, NULL) < 0)
		{
			perror("Receive ACK packet Failed");
			exit(1);
		}

		/* Check ACK packet */
		unsigned short received_block_num = ntohs(ack_packet.block_num);
		if(received_block_num != block_num)
		{
			printf("ACK packet error : different with expected block number\n");
			exit(1);
		}

		block_num++;
	}

	printf("complete send file\n");

	close(clnt_socket);

	fclose(file);

	return 0;

}




