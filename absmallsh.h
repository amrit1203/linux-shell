/* Amritpal Singh Benipal ID:110032884
*/
//*****************************************************************
#include <sys/wait.h>
#include <stdio.h>

//#include <sys/wait.h>

#define EOL 1 
#define ARG 2 
#define AMPERSAND 3 
#define SEMICOLON 4 
#define REDIRECT 5 //REDIRECT DEFINED
#define PIPE 7     //PIPE DEFINED
#define MAXARG 512 
#define MAXBUF 512 

#define FOREGROUND 0 
#define BACKGROUND 1