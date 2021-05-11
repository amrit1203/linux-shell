/* author Amritpal Singh Benipal ID: 110032884
*/
#include <fcntl.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include<sys/types.h>
#include<sys/wait.h>

/* Included all the programs in header*/
#include "absmallsh.h"
#include "abproc_line.c"
#include "abruncommand.c"
#include "abuserin.c"

//variables defined to get the current directory to run the shell again after calculator quits
char buffer[256];
char *prompt=buffer;
int userin(char *p);
void procline(void);


int main()
{
     	char c[50]= "This program makes simple arithmetic operation \n ";
    	write(1, c, sizeof(c));
     
        while(1)
	{
	char b[50]="Enter an arithmetic operation e.g 34 + 132 \n>";
	write(1, b, sizeof(b));
	char d[200];
	
	read(0, d, 200); //reading the statement 
	
	int pid, status;
	

	pid=fork();
	wait(&status);

	if(pid==0)
	{	
		void childfunction(char *);
		childfunction(d); //child calling the function
	}

	if(WEXITSTATUS(status) == 50){	
		printf("wrong statement\n");
	}
	else if(WEXITSTATUS(status) == 100){	
		printf("zero division\n");
	}
	else if(WEXITSTATUS(status) == 200){	
		printf("wrong operator\n");}
	
	else{
		printf("\n");
	}
	}
}
 
void childfunction(char* line)
{

	int op1, op2, s;
	char op;
	char c[10];
	sscanf(line, "%s", &c); // scans the input line if it has "quit"
	if(!strcmp(c,"quit"))
	{
	    getcwd(buffer, sizeof(buffer)); // gets the current working directory
	    while (userin(prompt) != EOF) //parse the cwd in userin program and checks for input
	    {
		      procline(); //runs the procline to identify and run command
		      getcwd(buffer, sizeof(buffer)); //gets the current working directory again after the command runs
	    }
		//kill(0, SIGINT);
	}

	int r=0;
	r= sscanf(line, "%d %c %d %c", &op1, &op, &op2);
	
	if(r!=3) // wrong statement 
	{
		exit(50);
	}
	else if(op=='/' && op2==0) //division by zero message
	{
		exit(100); 
	}
	else                     //operation performed
	{
		if(op=='+')
	        {s= op1+op2;}
   		else if(op=='-')
		{s= op1-op2;}
		else if(op=='*')
		{s= op1*op2;}
		else if(op=='/')
		{s= op1/op2;}
		else exit(200);//operator not matched
	}
	char buffer[50];
	sprintf(buffer,"%d %c %d = %d\n" , op1, op, op2, s); 
	write(1, buffer, strlen(buffer));// Printing the result
	exit(0);// exit normally
}  
