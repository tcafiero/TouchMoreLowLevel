#ifndef DEF_VARIABLES_DSP_H
#define DEF_VARIABLES_DSP_H


typedef enum {
   SRC_NO_SOURCE,
   SRC_TUNER,
   SRC_CD,
   SRC_MP3,
   SRC_CDC,
   SRC_AUX,
   SRC_PHONE,
   SRC_MAX_AUDIO_SOURCES

}  AUDIO_SOURCES_TYPES;

typedef enum
{
	FM_BAND,
	AM_BAND
}TYPE_BAND;

typedef enum
{
	AM_MW_BAND,
	AM_KW_BAND
}TYPE_SUB_BAND;

typedef unsigned char uint8 ;
typedef unsigned short uint16 ;
typedef unsigned int uint32 ;
typedef unsigned long long uint64 ;
typedef signed char sint8 ;
typedef signed short sint16 ;
typedef signed int sint32 ;
typedef signed long long sint64 ;

typedef enum {

   ERROR,
   OK
}  STATUS;

#define DELAY_TIME 10000
#define LOUDNESS_ON 1
#define LOUDNESS_OFF 0

#endif //DEF_VARIABLES_DSP_H
