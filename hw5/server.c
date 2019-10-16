#include <stdio.h>          /* These are the usual header files */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio_ext.h> // __fpurge(stdin);
#define PORT 5550  /* Port that will be opened */ 
#define BUFF_SIZE 256
#define BACKLOG 10
typedef struct user
{
	char user_id[20];
	char pass[20];
	int status;
}user;

int check_userID(user list[], char curent_id[], int soluong){
   for(int i=0;i<soluong;i++)
   {
   	if(strcmp(list[i].user_id,curent_id)==0) return 1;
   }
   return 0;
}
int check_Pass(user list[], char curent_pass[],int soluong){
	for(int i=0;i<soluong;i++){
		if(strcmp(list[i].pass,curent_pass)==0) return 1;
	}
	return 0;

}

int check_Status(user list[], char curent_id[], int soluong){
	for (int i = 0; i < soluong; i++)
	{
		if(strcmp(list[i].user_id,curent_id)==0) return list[i].status;
	}
}

void update_Status(user list[], char curent_id[],int soluong, FILE *fin){
	for (int i = 0; i < soluong; i++)
	{
			if(strcmp(list[i].user_id,curent_id)==0) {
				list[i].status=0;
				break;
			}
	}
	fin = fopen("acount.txt","w+");
	for (int i = 0; i < soluong; i++)
	{
		fprintf(fin, "%s %s %d\n",list[i].user_id,list[i].pass,list[i].status );
	}
	fclose(fin);

}
int main(int argc, char ** argv){
	int listen_sock,new_sock;
	char recv_data[BUFF_SIZE];
	int bytes_sent, bytes_received;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	int sin_size;
	int sohieucong; // PortNumber nhap tu ban phim
	pid_t childpid;
	int n; //kiem tra xem client yeu cau Sign hay Sign 
	FILE *fin;
	if(argc!=2){
		printf("Sai so luong doi so truyen vao, nhap lai theo dang ./server PortNum\n");
		exit(1);
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
		char taikhoan[256],matkhau[256];
		user list[10];
		new_sock = accept(listen_sock,( struct sockaddr *)&client, &sin_size);
		if (new_sock<0)
		{
			exit(1);
		}
		printf("[+]Connection accepted from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		
		
		if((childpid = fork()) == 0){    // tao ra 1 tien trinh con
		close(listen_sock);
		read(new_sock,&n,sizeof(n));
		printf("%d\n",n );
		do{
		if(n==1){
			read(new_sock,taikhoan,256);
			printf("[+]Tai khoan nguoi dung nhap: %s\n",taikhoan );
			read(new_sock,matkhau,256);
			printf("[+]Mat khau nguoi dung nhap: %s\n",matkhau );
			// doc du lieu tu file txt vao mang cau truc user
			fin=fopen("acount.txt","r");
			int i=0;
			while(!feof(fin)){
				fscanf(fin,"%s %s %d\n",list[i].user_id, list[i].pass, &list[i].status);
				i++;
			}
			fclose(fin);
			int soluong=i;
		
			/*for(i=0;i<soluong;i++){
				printf("%s %s %d\n",list[i].user_id, list[i].pass, list[i].status);
			}*/
			// kiem tra tai khoan, mat khau nhap vao
			if(check_userID(list,taikhoan,soluong)==1)  //co tim thay tai khoan
			{	int check_status =1;
				//truong hop dung user nhung sai mat khau, thong bao ve client+ tang bien check.
				if(check_Status(list,taikhoan,soluong)==0){
					write(new_sock,"[-]Tai khoan da bi khoa",strlen("[-]Tai khoan da bi khoa"));
					exit(1);
				}
				while(1){
						if(check_Pass(list,matkhau,soluong)==0){
						write(new_sock,"[-]Sai mat khau. Nhap lai mat khau!",strlen("[-]Sai mat khau. Nhap lai mat khau!"));
						check_status+=1;
						read(new_sock,matkhau,256);
						if(check_status==3){
							update_Status(list,taikhoan,soluong,fin);
							printf("[-]May chu khoa tai khoan %s\n",taikhoan );
							break;
						}
						}else{
						//truong hop dung user dung pass, gui thong bao hello user
					
						char pass_login[256];
						strcat(pass_login,"[+]Dang nhap thanh cong!\n[+]Hello");
						strcat(pass_login," ");
						strcat(pass_login,taikhoan);

						write(new_sock,pass_login,256);
						break;
					}
				}
				
			}
			else{ write(new_sock,"Tai khoan khong ton tai",strlen("Tai khoan khong ton tai"));} //gui thong bao la tk khong ton tai ve client
		}else{ // chuc nang Sign Out
			break;
		}
	}while(n!=3);
		}
	}
	close(new_sock);

	return 0;
}
