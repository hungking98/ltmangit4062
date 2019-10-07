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
#define thongbao4 "Error: File not found"
int exists(const char *fname)     //kiem tra file co ton tai k tren link goc
{
    FILE *file;
    if ((file = fopen(fname, "rb")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void tachlayten(char *duongdan, char *filename){
	char *token;
   
   /* lay token dau tien */
   token = strtok(duongdan, "/");
   
   /* duyet qua cac token con lai */
   while( token != NULL ) 
   {
      strcpy(filename,token);
    
      token = strtok(NULL, "/");
   }
}

long size_of_file(FILE *fin, char *filename)
{   fin=fopen(filename,"rb");
	fseek(fin, 0, SEEK_END);
	long size = ftell(fin);
	fclose(fin);
	
	return size;
}
int main(int argc, char ** argv){
	int client_sock;
	char buff[BUFF_SIZE],filename[BUFF_SIZE];
	struct sockaddr_in server_addr; /* server's address information */
	int  sin_size, bytes_sent, bytes_received;
	FILE *fin;  //con tro den file doc
	FILE *fpt;
	int numdoc; 
	long size_file;
	char thongbao[256];

	if(argc!=3){
		printf("[-]Sai so luong doi so nhap vao! Nhap lai duoi dang ./client IPaddress PortNumber\n");
		return 0;
	}

	//Step 1: Construct socket
	client_sock = socket(AF_INET,SOCK_STREAM,0);
	
	//Step 2: Specify server address
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	//Step 3: Request to connect server
	if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\n[-]Error!Can not connect to sever! Client exit imediately! ");
		return 0;
	}
	//Step 4: Communicate with server
	while(1){
		char link_file[256];
		char thongbao[256];
		size_file=0;
		printf("[+]Nhap duong dan file:\n");
		memset(buff,'\0',(strlen(buff)+1));
		fgets(buff, BUFF_SIZE, stdin);
		if(strlen(buff)<=1){return 0;}
		buff[strlen(buff)-1]='\0'; //bo di dau xuong dong
		strcpy(link_file,buff);    //luu buff vao link_file chua link den file can tai len server
		printf("%s\n",link_file);
		if(exists(link_file)==1){
		size_file = size_of_file(fin,buff); 
		tachlayten(buff,filename); //xau filename chua ten file, buff la duong dan file
		printf("[+]Dang gui file %s kich thuoc %ld bytes\n",filename, size_file );
       
        
        
		//gui ten file cho server
		
		bytes_sent = send(client_sock, filename, strlen(filename)+1, 0);
		if(bytes_sent <= 0){
			printf("\n[-]Connection closed! Khong gui ten file duoc\n");
	
			break;
		}
		
		//gui kich thuoc cho server
		char buff_read[size_file];
		bytes_received=recv(client_sock,thongbao,256,0);
		printf("[+]Tinh trang may chu:%s\n",thongbao );
		int h=write(client_sock,&size_file,256);
		if (h<0)
		{
			printf("[-]Khong the gui kich thuoc file %s\n",filename );
			break;
		}
		//gui file
		fin=fopen(link_file,"rb");
		fread(buff_read,size_file,1,fin);
		int k=write(client_sock,buff_read,size_file);
		printf("Da gui len server %d bytes\n",k );
		fclose(fin);

		//Nhan thong bao
		recv(client_sock,thongbao,256,0);
		printf("%s\n",thongbao );
	}
	else {
		printf("%s\n",thongbao4 );
	}

	}
	close(client_sock);

	return 0;
}