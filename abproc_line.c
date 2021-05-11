/*
    Amritpal Singh Benipal ID:110032884
*/
#include "absmallsh.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int gettok(char **outptr);
int runcommand(char **cline, int where);
int c1=0; //count number of arguments on left side of pipe
int c2=0; //count number of arguments on right side of pipe
int rd_args=0; //Number of arguments to pass in redirect

/*
   Join function to complete the pipe task
   Here I m passing whole arguments set
   Inside functions dividing the arguments into two char set on each side of pipe
*/
int join(char **args) {

	char **com1= malloc(256* sizeof args[0]);
	char **com2= malloc(256* sizeof args[0]);

	int i=0, j=0;
	
	while(i<c1)
	{
		com1[i] = args[i];
		i++;
	}

	while(j<c2)
	{
		com2[j] = args[i + j];
		j++;
	}

	com1[i] = NULL;
	com2[j] = NULL;

	int p[2], status;

	switch (fork()) {
		case -1: 
			perror("1st fork call in join");
			exit(3);
		case 0: 
			break;
		default: 
			wait(&status);
			return(status);
	}
	
	//child process's code
	if (pipe(p) == -1){
		perror("pipecall in join ");
		exit(4);
	}

	switch (fork()) {
		case -1:
			perror("2nd fork call failed in join");
			exit(5);
		case 0:  //the writing process (grandchild procss)
			dup2(p[1], 1);  //redirect stdout to the pipe, writing end.
				
			close(p[0]);
			close(p[1]);

			execvp(com1[0], com1);

			perror("1st execvp call failed in join");
			exit(6);
		default: //the "parent" process's code
			dup2(p[0], 0); //redirect stdin to the pipe, reading end

			close(p[0]);
			close(p[1]);

			execvp(com2[0], com2);

			perror("2nd execvp call failed in join");
			exit(7);
		}
}

/*
	redirect function to run the commands and store the result in .txt file
*/
void redirect(char **in)
{
	int fd=open(in[rd_args], O_WRONLY | O_TRUNC | O_CREAT, 0777); //open a file
	if(!fork()) 
	{
	   close(1);//close stdout
	   dup(fd); //to store the output in file
	   if(rd_args==1)//for a single argument command such as ls command
	   {
	      execlp(in[0], in[0], (char*) NULL);
	   }
	   else
	   {
	      for(int i=0; i< rd_args;i++)	
	      {
	         execlp(in[i], in[i],in[i+1], (char*) NULL); // executing the commands
	      }
	   }
	}
	else
	{
	    close(fd); //close file descriptor
	    wait(NULL);
	}
}

void procline(void) 
{
	char *arg[MAXARG + 1]; 
	int toktype; 
	int narg; 
	int type; 
	int pipeflag=0; //Pipe flag
        int pcount=1; //flag for counting arguments on each side of pipe 
	int rd_flag=0; //redirect flag
	

	narg = 0;
	for (;;) 
	{
	   switch (toktype = gettok(&arg[narg])) {
		case ARG:
			if (narg < MAXARG) {
				narg++;
				if(pcount==1)
				{
				    c1++; //incrementing till pipe not match
				}
				else
				{
				    c2++; //incrementing after pipe match
				}
			}
			break;
		case EOL:
		case SEMICOLON:
		case AMPERSAND:
			 if(pipeflag == PIPE) //This will execute the join function to run pipe 
			 {
				int ret = join(arg);
				pipeflag = 0; //setting flag and count to 0
				c1=0;
				c2=0;
			 }
			 else if(rd_flag==1) //This will execute the redirect function to run ">"
			 {
				printf("\n stored in file...... \n");
				redirect(arg);
				rd_flag=0; //setting flag and arguments to 0
				rd_args=0;
			 }
			else{if (toktype == AMPERSAND)
				type = BACKGROUND;
			else
				type = FOREGROUND;
			if (narg != 0)
			{
				arg[narg] = NULL;
				runcommand(arg, type);
			}}
			if (toktype == EOL)
				return;
			narg = 0;
			break;
		case PIPE:
			 pipeflag = PIPE; //pipe flag set
			 pcount=0; //setting pcount=0 to increment c2 case ARG
			 break;
		case REDIRECT:	
			      rd_flag=1; //redirect flag set
			      rd_args= narg;
			      break;
	  }
	}
}

