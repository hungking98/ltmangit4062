#include <stdio.h> 
#include <stdlib.h>
#include <ctype.h>         
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio_ext.h> // __fpurge(stdin);


#define SERV_PORT 5550
#define SERV_IP "127.0.0.1"
#define BUFF_SIZE 256
#define saimk "[-]Sai mat khau. Nhap lai mat khau!"
int main(int argc, char **argv){
	int client_sock;
	char buff[BUFF_SIZE],taikhoan[BUFF_SIZE],matkhau[BUFF_SIZE];
	struct sockaddr_in server_addr; /* server's address information */
	int  sin_size, bytes_sent, bytes_received;
	
	if(argc!=3){
		printf("Nhap sai so luong tham so truyen vao, nhap lai! Cu phap ./client IPnum PortNum\n");
		exit(1);
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
	sin_size = sizeof(struct sockaddr_in);
	
	//Step 4: Communicate with server // cho nay bat dau dvao menu dang nhap dang xuat
	int n; 
	do{
		printf("----------------------------\n");
		printf("Chuong trinh quan li tai khoan nguoi dung\n");
		printf("1.Dang nhap\n");
		printf("2.Dang xuat\n");
		printf("3.Exit\n");
		do{
			printf("\nChon chuc nang: ");
			scanf("%d",&n);
			if(n<1||n>3){
				printf("Lua chon khong kha thi!(chon 1-->3)\n");
			}
		}while(n<1||n>3);
		
		write(client_sock,&n,sizeof(n));

		switch(n){
		case 1:
		{	char thongbao[256];
			memset(thongbao,'\0',sizeof(thongbao));
			printf("[+]Nhap tai khoan:\n"); __fpurge(stdin); gets(taikhoan);
			write(client_sock,taikhoan,256);
			printf("[+]Nhap mat khau:\n");__fpurge(stdin); gets(matkhau);
			write(client_sock,matkhau,256);
			read(client_sock,thongbao,256);    //doc thong bao lan dau tien
			if(strcmp(thongbao,saimk)==0){
				while(1){
					
					if(strcmp(thongbao,saimk)!=0){
						break;
					}
					memset(matkhau, '\0', sizeof(matkhau));
					printf("[-]Nhap sai mat khau. Nhap lai mat khau:\n"); __fpurge(stdin); gets(matkhau);
					write(client_sock,matkhau,256);
					memset(thongbao,'\0',sizeof(thongbao));
					read(client_sock,thongbao,256);
					
				}
			}
			printf("%s\n",thongbao );

			break;
		}
		case 2:
		{	//kiem tra tai khoan co dang dang nhap khong
			//neu dang dang nhap thi out, neu chua thi thong bao chua dang nhap
			char thongbao[256];
			char user_out[256];
			char curent_user[256];
			memset(thongbao,'\0',sizeof(thongbao));
			
			read(client_sock,thongbao,256);
			if(strcmp(thongbao,"[+]Tien hanh dang xuat...")==0){
				write(client_sock,"OK",256);
				printf("%s\n",thongbao );
				read(client_sock,curent_user,256);
				do{
				memset(user_out,'\0',sizeof(user_out));
				printf("Nhap ten tai khoan dang dang nhap:\n" ); __fpurge(stdin);  gets(user_out);
				if(strcmp(user_out,curent_user)==0){
					printf("[+]Dang xuat thanh cong. Bye %s\n",curent_user);
					write(client_sock,"Dang xuat thanh cong",256);
					break;
				}else{
					printf("Nhap sai ten tai khoan dang su sung! Dang xuat that bai\n");
				}
				}while(1);
			}
			else printf("%s\n",thongbao );
			break;
		}
		case 3:
		{
			break;
		}

		}
	}while(n!=3);

	close(client_sock);
	return 0;
}