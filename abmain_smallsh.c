/* author Amritpal Singh Benipal ID: 110032884
*/
#include "absmallsh.h"
#include "abproc_line.c"
#include "abruncommand.c"
#include "abuserin.c"

//variables defined to store the current directory 
//char *prompt = "Command>>> ";
char buffer[256];
char *prompt=buffer;
int userin(char *p);
void procline(void);

int main()
{
	getcwd(buffer, sizeof(buffer));// gets the current working directory
	while (userin(prompt) != EOF)//parse the cwd in userin program and checks for input
	{
		procline();//runs the procline to identify and run command
		getcwd(buffer, sizeof(buffer));  //gets the current working directory again after the command runs
	}
}
