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
#define BACKLOG 5
#define thongbao1 "Successful transfering"
#define thongbao2 "Error: File is existent on server"
#define thongbao3 "Error: File tranfering is interupted"

int exists(const char *fname)     //kiem tra file co ton tai k tren kho chua
{
    FILE *file;
    if ((file = fopen(fname, "rb")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}
int main(int argc, char** argv){
	int listen_sock,new_sock;
	char recv_data[BUFF_SIZE];
	int bytes_sent, bytes_received;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	int sin_size;
	int sohieucong; // PortNumber nhap tu ban phim
 
    FILE *fin;
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
		printf("[+]Server dang chay tren cong so %d\n",sohieucong );
	}

	//Step 3: Listen request from client
	if(listen(listen_sock, BACKLOG) == -1){  /* calls listen() */
		perror("\nError: ");
		return 0;
	}
	sin_size = sizeof(struct sockaddr_in);
	
	//Step 4: Communicate with client
	while(1){
		
		new_sock = accept(listen_sock,( struct sockaddr *)&client, &sin_size);

		if (new_sock<0)
		{
			exit(1);
		}
		printf("[+]Connection accepted from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		
		if((childpid = fork()) == 0){
		close(listen_sock);
		while(1){	//memset(recv_data,'\0',(strlen(recv_data)+1));
					long size_file;
					char link_file[256];
					char filename[256];
					bytes_received = recv(new_sock, recv_data, BUFF_SIZE - 1, 0);
					if (bytes_received <= 0){
						printf("\nConnection closed");
						break;
						}
					strcpy(filename,recv_data);
					printf("Nhan file %s tu client\n",filename);
					
					bytes_sent=send(new_sock,"OK nhan ten file",sizeof("OK nhan ten file"),0);
					int k=read(new_sock,&size_file,256);
					if (k<0)
					{
						printf("Khong nhan duoc kich thuoc file tai len\n");
						return 0;
					}
					strcpy(link_file,"hung/");   //cai nay la thu muc minh tao ra trc de luu tren server thui
					strcat(link_file,filename);
					printf("Link den noi luu %s\n",link_file );
					printf("Kich thuoc file %ld\n",size_file );
					//kiem tra file da ton tai chua:))
					if (exists(link_file)==0)  //neu khong tim thay file trong kho chua server, OK tien hanh ghi
					{
						/* code */
					
						char buff_read[size_file];
						fin=fopen(link_file,"wb");
						int Num_read=read(new_sock,buff_read,size_file);
						if(Num_read==size_file){
							
							send(new_sock,thongbao1,sizeof(thongbao1),0);  // thong bao truyen tai thanh cong
						}else {
							
							send(new_sock,thongbao3,sizeof(thongbao3),0);
							exit(0); // thong bao loi truyen tai
						}

						printf("%d\n",Num_read );
						fwrite(buff_read,size_file,1,fin);
						fclose(fin);
					}else{
						send(new_sock,thongbao2,sizeof(thongbao2),0);
						exit(0);
					}
				
			}
		}
	} 
	close(new_sock);
	return 0;
}