#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>


#include "utils.h"


void PrintMessage ( int p_color, 
                    const char *p_format, ... )
{
	va_list  l_args;
	char *l_head;
	//char *l_name;
	char *l_tail=TAIL_MESSAGE;
	char  l_new_format[MSG_BUFF_LEN];

	memset ( l_new_format, 0, MSG_BUFF_LEN );
	fflush ( stderr );

	switch ( p_color )
	{
		case RED_PRINT:
		{
			l_head="\e[1;31m";
	    }	
		break;
		case GREEN_PRINT:
		{
			l_head="\e[3;32m";
	    }	
		break;
		case YELLOW_PRINT:
		{
			l_head="\e[3;33m";
	    }	
		break;
	}
/*
	if ( (p_color == 1 && initializeFile->getPrintR() == 1 ) || 
	     (p_color == 2 && initializeFile->getPrintB() == 1 ) ||
	     (p_color == 3 && initializeFile->getPrintG() == 1 ) )*/
	{
		strcpy ( l_new_format, l_head );
		strcat ( l_new_format, p_format );
		strcat ( l_new_format, l_tail );
	
		va_start ( l_args, p_format );
		vprintf ( l_new_format, l_args );
		va_end ( l_args );
	
		fflush ( stderr );
	}
// 	else
		return;
}
