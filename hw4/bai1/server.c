/*UDP Echo Server*/
#include <stdio.h>          /* These are the usual header files */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define PORT 5550  /* Port that will be opened */ 
#define BUFF_SIZE 1024
#define BACKLOG 10

int checkmess(char* mess) {
	int i;
	for(i = 0; i < strlen(mess); i++) {
		if((isalpha(mess[i]) == 0) && (isdigit(mess[i]) == 0)) {  //check xem xau nhap vao co ki tu la khong
			return 0;
		}
	}
	return 1;
}

void laychucai_from_mess(char* mess, char* str) {
	int i;
	int j = 0;
	for(i = 0; i < strlen(mess); i++) {         //lay xau tu chuoi mess
		if(isalpha(mess[i]) != 0) {
			str[j++] = mess[i];
		}
	}
}

void laychuso_from_mess(char* mess, char* str) {   // lay so tu chuoi
	int i;
	int j = 0;
	for(i = 0; i < strlen(mess); i++) {
		if(isdigit(mess[i]) != 0) {
			str[j++] = mess[i];
		}
	}
}

int main(int argc, char **argv){
	int listen_sock,new_sock;
	char recv_data[BUFF_SIZE];
	int bytes_sent, bytes_received;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	int sin_size;
	int sohieucong; // PortNumber nhap tu ban phim
	pid_t childpid; 

	if (argc!=2){
		printf("Sai so luong tham so truyen vao! Nhap lai theo vi du:./server 3300\n");
		return 0;
	}
	sohieucong=atoi(argv[1]);
	//tao socket lang nghe ket noi;
	//Step 1: Construct a TCP socket to listen connection request
	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}

	//Step 2: Bind address to socket
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;         
	server.sin_port = htons(sohieucong);   /* Remember htons() from "Conversions" section? =) */
	server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */ 
	if(bind(listen_sock, (struct sockaddr*)&server, sizeof(server))==-1){ /* calls bind() */
		perror("\nError: ");
		return 0;
	}else{
		printf("Server dang chay tren cong so %d\n",sohieucong );
	}

	//Step 3: Listen request from client
	if(listen(listen_sock, BACKLOG) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}
	sin_size = sizeof(struct sockaddr_in);
	
	//Step 4: Communicate with client
	while(1){

		//printf("------------------------------------------------\n");
		new_sock = accept(listen_sock,( struct sockaddr *)&client, &sin_size);
		if (new_sock<0)
		{
			exit(1);
		}
		printf("[+]Connection accepted from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		memset(recv_data,'\0',(strlen(recv_data)+1)); //reset xau nhan moi lan lap
		if((childpid = fork()) == 0){
		close(listen_sock);
		//receives message from client
		while(1){
		bytes_received = recvfrom(new_sock, recv_data, BUFF_SIZE - 1, 0, (struct sockaddr *) &client, &sin_size);
		if (bytes_received <= 0){
				printf("\nConnection closed\n");
				exit(1);
			}else{
				recv_data[bytes_received] = '\0';
				printf("\n[+]Xau nhan tu client: %s\n", recv_data);
				if (checkmess(recv_data)!=1)
				{
					bytes_sent = sendto(new_sock, "Xau chua ki tu sai", sizeof("Xau chua ki tu sai"), 0, (struct sockaddr *) &client, sin_size );
					if (bytes_sent < 0)
						perror("\nError: ");
				}
				else{
					char number[BUFF_SIZE] = "";    //thuchien tach lay so,chu cai rieng biet roi gui lai ve client
 					char character[BUFF_SIZE] ="";
					char xauguilai_from_server[3000] = "";
					laychucai_from_mess(recv_data,character);
					laychuso_from_mess(recv_data,number);
					strcat(xauguilai_from_server, character); 
					strcat(xauguilai_from_server, "\n");
					strcat(xauguilai_from_server, number);
					strcat(xauguilai_from_server, "\0");
					
					bytes_sent = sendto(new_sock, xauguilai_from_server, strlen(xauguilai_from_server), 0, (struct sockaddr *) &client, sin_size ); /* send to the client welcome message */
					
					if (bytes_sent < 0)
						perror("\nError: ");

				}

			}
		}


		}
	} 
	close(new_sock);   
	
	return 0;
}