/*UDP Echo Client*/
#include <stdio.h> 
#include <stdlib.h>
#include <ctype.h>         
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SERV_PORT 5550
#define SERV_IP "127.0.0.1"
#define BUFF_SIZE 1024

int main(int argc, char ** argv){
	int client_sock;
	char buff[BUFF_SIZE];
	struct sockaddr_in server_addr; /* server's address information */
	int  sin_size, bytes_sent, bytes_received;

	if(argc!=3){
		printf("Sai so luong doi so nhap vao! Nhap lai duoi dang ./client IPaddress PortNumber\n");
		return 0;
	}

	//Step 1: Construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);
	
	//Step 2: Specify server address
	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));

	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	//Step 3: Request to connect server
	int ret=connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");
	

	//Step 4: Communicate with server
	while(1){
		sin_size = sizeof(struct sockaddr_in);
		//send message
		printf("Nhap xau can xu li:");
		memset(buff,'\0',(strlen(buff)+1));
		fgets(buff, BUFF_SIZE, stdin);
		if (strlen(buff) == 0) break;
		buff[strlen(buff)-1]='\0'; //bo di dau xuong dong
		

		bytes_sent = sendto(client_sock, buff, strlen(buff), 0, (struct sockaddr *) &server_addr, sin_size);
		//printf("%d\n", bytes_sent );
		if(bytes_sent <= 0){
			printf("\nConnection closed!\n");
			break;
		}
		memset(buff,'\0',(strlen(buff)+1));
		bytes_received = recvfrom(client_sock, buff, BUFF_SIZE - 1, 0, (struct sockaddr *) &server_addr, &sin_size);

		if(bytes_received < 0){
				perror("Error: ");
				close(client_sock);
				return 0;	
				}
		buff[bytes_received] = '\0';
		printf("Thong diep tu may chu:\n");
		printf("%s\n",buff);
		printf("----------------------------------------\n");
		}
		
	return 0;
}