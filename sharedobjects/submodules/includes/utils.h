/******************************************************/
/*****   libDirana  Header file  **********************/
/*****   Akhela                  **********************/
/******************************************************/

#ifndef UTILS_H
#define UTILS_H
#define TAIL_MESSAGE         "\e[00m\n"
#define MSG_BUFF_LEN 1024
#define RED_PRINT    0 
#define GREEN_PRINT  1
#define YELLOW_PRINT 2




void PrintMessage ( int p_color, 
                    const char *p_format, ... );

#endif //#ifndef UTILS_H
