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

int main(int argc, char *argv[]){
	int client_sock;
	char buff[BUFF_SIZE];
	struct sockaddr_in server_addr;
	int bytes_sent,bytes_received, sin_size;
	if(argc!=3){
		printf("Sai so luong doi so nhap vao! Nhap lai duoi dang ./client IPaddress PortNumber\n");
	}
	else{
		//Step 1: Construct a UDP socket
		if ((client_sock=socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){  /* calls socket() */
			perror("\nError: ");
			exit(0);
		}

		//Step 2: Define the address of the server
		bzero(&server_addr, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(atoi(argv[2]));  //argv[2] la so cong nhap vao tu client
		server_addr.sin_addr.s_addr = inet_addr(argv[1]); //argv [1] la dia chi IP
		//Step 3: Communicate with server
		while(1){
		printf("\nGui thong diep den may chu:");
		memset(buff,'\0',(strlen(buff)+1));
		fgets(buff, BUFF_SIZE, stdin);
		buff[strlen(buff)-1]='\0';
		
		
	
		sin_size = sizeof(struct sockaddr);
	
		bytes_sent = sendto(client_sock, buff, strlen(buff), 0, (struct sockaddr *) &server_addr, sin_size);
		if(bytes_sent <= 0){
			perror("Error: ");
			close(client_sock);
			return 0;
		}
		bytes_received = recvfrom(client_sock, buff, BUFF_SIZE - 1, 0, (struct sockaddr *) &server_addr, &sin_size);
			if(bytes_received < 0){
			perror("Error: ");
			close(client_sock);
		return 0;
		}
		buff[bytes_received] = '\0';
		printf("Thong diep tu may chu:\n");
		printf("%s\n",buff);
	}
		
		close(client_sock);
	}
	return 0;
}
