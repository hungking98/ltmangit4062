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
#include <netdb.h>

#define PORT 5550  /* Port that will be opened */ 
#define BUFF_SIZE 1024
int main(int argc,char **argv){
	int server_sock; /* file descriptors */
	char buff[BUFF_SIZE];
	int bytes_sent, bytes_received;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	int sin_size;
	int sohieucong;    //port-number -->cong chung ket noi cho client - server
	int i;
	struct hostent *he;
	struct in_addr **addr_list;
	struct in_addr ipv4addr;
	if (argc!=2){
		printf("Sai so luong tham so truyen vao! Nhap lai theo vi du:./server 3300\n");
	}else{
		sohieucong=atoi(argv[1]);
		if ((server_sock=socket(AF_INET, SOCK_DGRAM, 0)) == -1 ){  /* calls socket() */
			perror("\nError: ");
			exit(0);
		}
		//Step 2: Bind address to socket
		server.sin_family = AF_INET;         
		server.sin_port = htons(sohieucong);   
		server.sin_addr.s_addr = INADDR_ANY;  /* INADDR_ANY puts your IP address automatically */   
		bzero(&(server.sin_zero),8); /* zero the rest of the structure */

  
		if(bind(server_sock,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){ /* calls bind() */
			perror("\nError: ");
			exit(0);
		}
		printf("Sever dang chay tren cong so %d\n",sohieucong );
		//Step 3: Communicate with clients
		while(1){
				memset(buff,'\0',(strlen(buff)+1));
				sin_size=sizeof(struct sockaddr_in);
    		    
				bytes_received = recvfrom(server_sock, buff, BUFF_SIZE-1, 0, (struct sockaddr *) &client, &sin_size);
				
		
				if (bytes_received < 0)
					perror("\nError: ");
				else{
					
					printf("Thong tin client-[%s]-[%d]: %s\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), buff);
				}
				char xauguilai_from_server[3000]="";
				if(inet_addr(buff)==-1){
					he=gethostbyname(buff);
					if(he!=NULL){
						addr_list = (struct in_addr **)he->h_addr_list;
						strcat(xauguilai_from_server,"Official IP:");
						strcat(xauguilai_from_server,inet_ntoa(*addr_list[0]));
						strcat(xauguilai_from_server,"\n");
						strcat(xauguilai_from_server,"Alias IP:");
						strcat(xauguilai_from_server,"\n");
						for(i = 1; addr_list[i] != NULL; i++) {
							strcat(xauguilai_from_server,inet_ntoa(*addr_list[i]));//chuyen cau truc in_addr sang cau truc a.b.c.d
							strcat(xauguilai_from_server,"\n");
        				   
    				}

					}else{ 
						strcat(xauguilai_from_server,"Not found information"); 
						strcat(xauguilai_from_server,"\n");
					}

				}
				else{
					inet_pton(AF_INET, buff, &ipv4addr);
					he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
					if(he!=NULL){
						strcat(xauguilai_from_server,"Official name:");
						strcat(xauguilai_from_server,he->h_name);
						strcat(xauguilai_from_server,"\n");
						strcat(xauguilai_from_server,"Alias name:");
						strcat(xauguilai_from_server,"\n");
						while(*he->h_aliases){
							strcat(xauguilai_from_server,*he->h_aliases);
							strcat(xauguilai_from_server,"\n");
							he->h_aliases++;
				
							}


					}else{
						strcat(xauguilai_from_server,"IP Address is invalid"); 
						
					}

				}


				bytes_sent = sendto(server_sock, xauguilai_from_server, 3000, 0, (struct sockaddr *) &client, sin_size ); /* send to the client welcome message */
					if (bytes_sent < 0)
						perror("\nError: ");
		}

	}
	return 0;
}