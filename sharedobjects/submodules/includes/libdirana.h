/******************************************************/
/*****   libDirana  Header file  **********************/
/*****   Akhela                  **********************/
/******************************************************/

#ifndef LIBDIRANA_H
#define LIBDIRANA_H

#include <unistd.h>

#include "dirana_utils.h"
#include "libDSP.h"

#include "def_variables_DSP.h"

typedef enum {
   DIRANA_ERROR,
   DIRANA_OK,
   DIRANA_WAIT
}  DIRANA_STATUS;


union USUC
{
  unsigned short us;                                            
  unsigned char uc[2];
};


typedef struct
{
  /* IF Filter center frequency */   /* 1 bytes */
  int FM_IF_Filter_Center_Freq_Main_tuner;
  int AM_IF_Filter_Center_Freq_Main_tuner;
  int FM_IF_Filter_Center_Freq_Sub_tuner;
  int AM_IF_Filter_Center_Freq_Sub_tuner;
  /* Offset alignment */   /* 3 bytes */
  int FM_Offset_Main_tuner_Europe_H;
  int FM_Offset_Main_tuner_Europe_L;
  int FM_Offset_Main_tuner_Japan_H;
  int FM_Offset_Main_tuner_Japan_L;
  int MW_Offset_tuner_Europe_H;
  int MW_Offset_tuner_Europe_L;
  int KW_Offset_tuner_H;
  int KW_Offset_tuner_L;
  int MW_Offset_tuner_USA_H;
  int MW_Offset_tuner_USA_L;
  /* Level alignment */   /* 2 bytes */
  int FM_Level_Main_tuner;
  int FM_Level_Sub_tuner;
  int FM_Level_Main_tuner_Japan;
  int FM_Level_Sub_tuner_Japan;
  int MW_Level_Main_tuner;
  int MW_Level_Sub_tuner;
  int KW_Level_Main_tuner;
  int KW_Level_Sub_tuner;
  /* Stereo decoder */   /* 2 bytes */
  int FM_Stereo_decoder;
  /* free RESERVED */   /* 5 bytes */
  /* CheckSum */   /* 1 byte */
}  DIRANA_ALIGNMENT_TYPE;

#define VOLUMEINDBTABLE  121
#define MAXVOLSTEP 30 + 1

/********************************************/

/* library APIs  declarations */
bool initDirana(void);
bool setSourceDirana(AUDIO_SOURCES_TYPES p_source );
bool getSourceDirana(AUDIO_SOURCES_TYPES *p_source);
bool setVolumeDirana(int p_volume);
bool getVolumeDirana(int *p_volume);

bool SetBalanceDirana( sint8 p_balance );
bool SetFaderDirana( sint8 p_fader );
bool SetLoudnessDirana( uint8 p_loudness );

/***play sin****/
bool playSinDirana(int p_volume,int p_frequency, bool p_enable);

bool doMuteDirana(int val);
bool change_bandDirana(TYPE_BAND p_new_band);
void get_bandDirana(TYPE_BAND p_band, TYPE_SUB_BAND p_subband);
bool set_frequencyDirana(int p_frequency, TYPE_BAND p_band, TYPE_SUB_BAND p_subAM_band);
int get_frequencyDirana();

bool set_LeafDiceCfgDirana(int p_freq, TYPE_BAND p_band, TYPE_SUB_BAND p_subAM_band);
bool get_Freq_Band_fromTunerDirana(int *p_freq, TYPE_BAND *p_band, TYPE_SUB_BAND *p_subband);

//STATUS setSourceAUXDirana();
//STATUS SetAuxOnPrimaryStereoDirana();
bool SetTunerOnPrimaryStereoDirana();
bool SetI2SOnPrimaryStereoDirana();
//STATUS SetCdcOnPrimaryStereoDirana();
//STATUS SetPhoneOnPrimaryStereoDirana();
///******************************///

#endif //#ifndef LIBDIRANA_H
