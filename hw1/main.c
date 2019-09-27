#include<stdio.h>  
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
#include"dslkd.h"
void Menu();									
void scanlist(FILE *fin);						
void SignIn();									
void Register();								
void Search();									
void SignOut();									
void checkSignIn();								
void checkRegister();							
void checkSearch(root,username);				
void updatefile();								
void chaneStatus(node *p,char username[]);		
char UserNameCurrent[25];						
int timesFalse=0;								
void scanlist(FILE *fin){
	elementtype tmp;
	while(!feof(fin))
    {
      fscanf(fin,"%s %s %d\n",tmp.username,tmp.password,&tmp.status);     
      insert_first(tmp);												
    }
}
void Menu(){
	int choice;
	do{
		printf("USER MANAGEMENT PROGRAM\n");
		printf("------------------------------------\n");
		printf("1.Register\n");
		printf("2.Sign In\n");
		printf("3.Search\n");
		printf("4.Sign Out\n");
		printf("5.Quit\n");
		do{
			printf("\nChon chuc nang: ");
			scanf("%d",&choice);
			if(choice<1||choice>5){
				printf("Lua chon khong kha thi!(chon 1-->6)\n");
			}
		}while(choice<1||choice>5);
		switch(choice){
			case 1:
				Register();
				updatefile(root);
				displaynode(root);
				break;		
			case 2:
				SignIn();
				updatefile(root);
				break;
			case 3:
				Search();
				break;
			case 4:
				SignOut();
				break;
			case 5:
			    delete_all_list();
				break;
		}
	}while(choice!=5);
}
void updatefile(node *p){
	FILE * fp;
   	fp = fopen ("account.txt", "w+");
	while(p!=NULL){
		elementtype tmp=p->element;
		fprintf(fp, "%s %s %d\n",tmp.username,tmp.password,tmp.status );    
		p=p->next;
	}
				
	fclose(fp);														
}
void Register(){
	int check;
	char username[25];
	char password[25];
	printf("\nNhap tai khoan:");__fpurge(stdin);
	gets(username);
	printf("Nhap mat khau:");__fpurge(stdin);
	gets(password);
	checkRegister(root,username,password);
}
void checkRegister(node *p,char username[],char password[]){
	int checkUsername = 1;
	while(p!=NULL){
    	elementtype tmp=p->element;
    	if(strcmp(username,tmp.username)==0){
    		checkUsername = 0;
    		printf("\nAccount existed \n\n");
    	}
    	p=p->next;
    }
    if(checkUsername==1){
    	printf("\nSuccessful registration\n\n");
    	elementtype tmp;
    	strcpy(tmp.username,username);
    	strcpy(tmp.password,password);
    	tmp.status = 1;
    	insert_first(tmp);
    }
}
void SignIn(){
	char username[25];
	char password[25];
	printf("\nNhap tai khoan:");__fpurge(stdin);
	gets(username);
	printf("Nhap mat khau:");__fpurge(stdin);
	gets(password);
	checkSignIn(root,username,password);
}
void checkSignIn(node *p,char username[],char password[]){
	int checkUsername = 0;
	int checkPassword = 0;
	int status;
	while(p!=NULL){
    	elementtype tmp=p->element;
    	if(strcmp(username,tmp.username)==0){
    		checkUsername = 1;
    		if(strcmp(password,tmp.password)==0){
    			checkPassword = 1;
    			status = tmp.status;
    		}
    	}
  		p=p->next;
  	}
  	if(checkUsername == 0){
  		printf("\nCannot find account\n\n");
  	}
  	else{
  		if(checkPassword == 0){				//Kiểm tra số lần nhập sai 
  			timesFalse++;    
  			if(timesFalse==3){
  				chaneStatus(root,username);
  				timesFalse=0;
  			}
  			printf("\nPassword is incorrect\n\n");
  		}
  		else{
  			if(status == 0){
  				printf("\nAccount is blocked\n\n");
  			}
  			else{
  				printf("\nHello %s\n\n",username );
  				strcpy(UserNameCurrent,username);
  			}
  		}
  	}
}
void chaneStatus(node *p,char username[]){
	while(p!=NULL){
    	if(strcmp(p->element.username,username)==0){
    		p->element.status = 0;				
    		printf("oke\n");
    	}
  		p=p->next;
  	}
}
void Search(){
	char username[25];
	printf("\nNhap tai khoan muon tim kiem:" );
	__fpurge(stdin);
	gets(username);
	checkSearch(root,username);
}
void checkSearch(node *p,char username[]){
	int check = 0;
	while(p!=NULL){
    	elementtype tmp=p->element;
    	if(strcmp(tmp.username,username)==0){
    		check = 1;
    		if(tmp.status==0){
    			printf("\nAccount is blocked\n\n");
    		}
    		else{
    			printf("\nAccount is active \n\n");
    		}
    	}
  		p=p->next;
  	}
  	if(check==0){
  		printf("\nCannot find account\n\n");
  	}
}
void SignOut(){
	char username[25];
	if(strlen(UserNameCurrent)==0){
		printf("\nAccount is not sign in\n\n");
	}
	else{
		printf("\nNhap ten tai khoan da dang nhap:");
		__fpurge(stdin);
		gets(username);
		if(strcmp(username,UserNameCurrent)==0){
			printf("\nGoodbye %s\n\n", UserNameCurrent);
			for(int i=strlen(UserNameCurrent)-1;i>=0;i--){
				UserNameCurrent[i]='\0';
			}
		}
		else{
			printf("\nCannot find account \n\n");
		}
	}
}
int main()
{	
	FILE * fp;
	fp = fopen ("account.txt", "r");
	rewind(fp);
   	scanlist(fp);
   	fclose(fp);
   	Menu();
   
   
   return 0;
}