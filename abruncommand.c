/* Amritpal Singh Benipal ID:110032884
*/
#include "absmallsh.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int runcommand(char **cline, int where)
{
	pid_t pid;
	int status;
	switch (pid = fork()) {
		case -1:
			perror("smallsh");
			return (-1);
		case 0: if(!strcmp(cline[0],"cd")) //if user entered the cd command 
			{
				int flag=1;
				if(cline[1]==NULL) //if user entered only cd and secong arguments is null then home directory set as current directory
				{
				     char b[256];
				     getcwd(b, sizeof(b));
				     int i=0;//count tokens
				     char path[100]="/home/";
				     char *tok = strtok(b,"/");
				     while (tok!= NULL) //extracting the tokens from cwd and putting the username in path "/home/"
 				     {
					   i++;
					   if(i==2)
					   {
						strcat(path, tok);
					   }
				           tok = strtok(NULL, "/");
 				     }
				     flag=chdir(path);
				}
				else
				{
				     flag=chdir(cline[1]); // if path is given in secong argument
				}
			    if(flag==0) //if change of path is successfull
				{
		  		         printf("\nSuccessfull\n");
				}
				else
				{	
				         printf("\nError changing directory\n");
				}
			}
			else if(!strcmp(cline[0],"calculator")) // if user entered argument as calculator
			{
				execl("./calculator", "calculator", NULL);// running the calculator program
			}
			else if(!strcmp(cline[0],"quit")||!strcmp(cline[0],"exit")) //if user entered quit or exit
			{
				kill(0, SIGINT); //quits the shell
			}
			else 
			{
				execvp(cline[0], cline); //runs the command
				perror(*cline);
			        exit(1);
			}
	}
	
	if (where == BACKGROUND)
	{
		printf("[process id %d]\n", pid);
		return (0);
	}
	
	if (waitpid(pid, &status, 0) == -1)
		return (-1);
	else
		return (status);
}