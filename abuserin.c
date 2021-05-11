/* Amritpal Singh Benipal ID:110032884
*/
#include "absmallsh.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

// command arg1 arg2 arg3
static char inpbuf[MAXBUF], tokbuf[2 * MAXBUF],
*ptr = inpbuf, *tok = tokbuf;

static int timed_out;

int inarg(char c);

/*
User defined Signal handler for SIGALRM
*/
void timeout(int sig){
	timed_out = 1;
	puts("hurry up.... press Enter\n");
}


int userin(char *p)
{
	signal(SIGALRM, timeout);
	int c, count;
	ptr = inpbuf;
	tok = tokbuf;
	printf("\n Enter command within 10 seconds:> ");
	count = 0;
	printf("\n%s $", p); //print the current directory
	while (1)
	{
		timed_out = 0;
		
		alarm(10); //raise the alarm if nothing entered within 10 seconds .... will send a signal SIGALRM

		fgets(inpbuf, sizeof(inpbuf), stdin); //gets the input 

		alarm(0);
		
		if (!timed_out) //if timed_out = 1 break
			break;
		else{
		   printf("\n Enter something within 10 seconds:> ");
		   printf("\n%s $", p);}
	}
	signal(SIGALRM, SIG_DFL); //default handler for SIGALRM
//	printf("inpbuf=%s\n", inpbuf);
}

int gettok(char **outptr)
{
	int type;
	
	*outptr = tok;
	
	while (*ptr == ' ' || *ptr == '\t')
		ptr++;
	
	*tok++ = *ptr;
	
	switch (*ptr++) {
	case '\n':
		type = EOL;
		break;
	/*
		Created the cases for pipe and redirect type
	*/
	case '|':
		type = PIPE;
		break;
	case '>':
		type = REDIRECT;
		break;
	case '&':
		type = AMPERSAND;
		break;
	case ';':
		type = SEMICOLON;
		break;
	default:
		type = ARG;
		
		while (inarg(*ptr))
			*tok++ = *ptr++;
	}
	*tok++ = '\0';
	return type;
}

//****************************************************************************************
static char special[] = { ' ', '\t', '&', ';', '\n', '\0', '|','>' };
int inarg(char c)
{
	char *wrk;
	for (wrk = special; *wrk; wrk++)
	{
		if (c == *wrk)
			return (0);
	}
	return (1);
}

