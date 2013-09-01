/******************************************************/
/*****   libDirana  Source file  **********************/
/*****   Akhela                  **********************/
/******************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "libdirana.h"
#include "utils.h"
#include "dirana_utils.h"


AUDIO_SOURCES_TYPES g_src_active = SRC_NO_SOURCE;

int g_current_volume = 0;
int g_current_band = FM_BAND;
int g_current_frequency = 87500;
unsigned char data_T1A[TUNER_REG_SIZE_WRITE] =
	{
		0x30,0x87,0xe0,0xE2,0x07,0x00,0x04
	};

/*unsigned char data_T1B[TUNER_REG_SIZE] =
	{
		0x30,0x87,0xe0,0xE2,0x06,0x00,0x04
	};
*/
/* Buffer with default values of alignment */
volatile DIRANA_ALIGNMENT_TYPE g_tuner_DiranaAlignment =
{
  /* IF Filter center frequency */   /* 1 bytes */
  0x000006,   /* FM_IF_Filter_Center_Freq_Main_tuner; */
  0x000006,   /* AM_IF_Filter_Center_Freq_Main_tuner; */
  0x000005,   /* FM_IF_Filter_Center_Freq_Sub_tuner; */
  0x000006,   /* AM_IF_Filter_Center_Freq_Sub_tuner; */
  /* Offset alignment */   /* 3 bytes */
  0xE283DB,   /* FM_Offset_Main_tuner_Europe_H; */
  0xE26875,   /* FM_Offset_Main_tuner_Europe_L; */
  0xE281DD,   /* FM_Offset_Main_tuner_Japan_H; */
  0xE26A73,   /* FM_Offset_Main_tuner_Japan_L; */
  0xFA7001,   /* MW_Offset_tuner_Europe_H; */
  0xFA6FB5,   /* MW_Offset_tuner_Europe_L; */
  0xFA4294,   /* KW_Offset_tuner_H; */
  0xFA40E0,   /* KW_Offset_tuner_L; */
  0xFA56CC,   /* MW_Offset_tuner_USA_H; */
  0xFA5680,   /* MW_Offset_tuner_USA_L; */
  /* Level alignment */   /* 2 bytes */
  0x000FC1,   /* FM_Level_Main_tuner; */
  0x000FC1,   /* FM_Level_Sub_tuner; */
  0x000FC8,   /* FM_Level_Main_tuner_Japan; */
  0x000FC4,   /* FM_Level_Sub_tuner_Japan; */
  0x000FB7,   /* MW_Level_Main_tuner; */
  0x000FB8,   /* MW_Level_Sub_tuner; */
  0x000FC4,   /* KW_Level_Main_tuner; */
  0x000FC5,   /* KW_Level_Sub_tuner; */
  /* Stereo decoder */   /* 2 bytes */
  0x00054D,   /* FM_Stereo_decoder; */
  /* free RESERVED */   /* 5 bytes */
  /* CheckSum */   /* 1 byte */
};


/*********************************Loudness*********************************/
// Loudness treble filter
#define LOUDNESS_T_BOOST_MAX (0x3A3) // 9dB
#define LOUDNESS_T_COEFF 5

const unsigned short Loudness_treble_filter[LOUDNESS_T_COEFF]={
0x17F, 0x000, 0x2BF, 0xD40, 0x000 // a1, a2, b0, b1, b2
};

// Loudness bass filter
#define LOUDNESS_B_BOOST_MAX (0x614) // 17dB
#define LOUDNESS_B_COEFF 8

const unsigned short Loudness_bass_filter[LOUDNESS_B_COEFF]={
0x3EE, 0x522, 0x000, 0x000, 0x115, 0x05D, 0x000, 0x000 // a1bH, a1bL, a2bH, a2bL, b0bH, b0bL, b2bH, b2bL
};

// Loudness Boost curve
const uint16 loudness_boost_table[VOLUME_NB_STEP+1]={
  0x0000, /*volume=-120 dB*/
  0x001D,
  0x0039,
  0x0056,
  0x0072,
  0x008E,
  0x00A9,
  0x00C5,
  0x00E0,
  0x00FB,
  0x0115,
  0x012F,
  0x0149,
  0x0163,
  0x017D,
  0x0196,
  0x01AF,
  0x01C8,
  0x01E1,
  0x01F9,
  0x0211,
  0x022A,
  0x0241,
  0x0259,
  0x0270,
  0x0288,
  0x029F,
  0x02B6,
  0x02CC,
  0x02E3,
  0x02F9,
  0x030F,
  0x0325,
  0x033B,
  0x0351,
  0x0367,
  0x037C,
  0x0392,
  0x03A7,
  0x03BC,
  0x03D1,
  0x03E6,
  0x03FB,
  0x0410,
  0x0424,
  0x0439,
  0x044D,
  0x0462,
  0x0476,
  0x048B,
  0x049F,
  0x04B3,
  0x04C8,
  0x04DC,
  0x04F0,
  0x0505,
  0x0519,
  0x052E,
  0x0542,
  0x0557,
  0x056C,
  0x0581,
  0x0596,
  0x05AB,
  0x05C1,
  0x05D6,
  0x05ED,
  0x0603,
  0x061A,
  0x0632,
  0x064A,
  0x0663,
  0x067D,
  0x0699,
  0x06B5,
  0x06D4,
  0x06F5,
  0x071A,
  0x0745,
  0x077D,
  0x0800 /*volume=0 dB*/
};

const unsigned char UserVolumeArrayStMode[MAXVOLSTEP]={
   0,18,22,27,31,35,38,40,42,44,
   46,48,50,52,54,56,58,60,62,63,
   64,65,66,67,68,70,72,74,76,78,80};


const unsigned short VolumeArrayForStdMode[VOLUMEINDBTABLE][2] =
  // Table computed with MaxVoldb 0, MaxLoud 19 for STANDARD MODE
{
  {0x0000, 0x0008}, // -120 dB, 0
  {0x0000, 0x0008}, // -119 dB, 1
  {0x0000, 0x0008}, // -118 dB, 2
  {0x0000, 0x0008}, // -117 dB, 3
  {0x0000, 0x0008}, // -116 dB, 4
  {0x0000, 0x0008}, // -115 dB, 5
  {0x0FFF, 0x0008}, // -114 dB, 6
  {0x0FFF, 0x0008}, // -113 dB, 7
  {0x0FFF, 0x0008}, // -112 dB, 8
  {0x0FFF, 0x0008}, // -111 dB, 9
  {0x0FFF, 0x0008}, // -110 dB, 10
  {0x0FFF, 0x0008}, // -109 dB, 11
  {0x0FFE, 0x0008}, // -108 dB, 12
  {0x0FFE, 0x0008}, // -107 dB, 13
  {0x0FFE, 0x0008}, // -106 dB, 14
  {0x0FFE, 0x0008}, // -105 dB, 15
  {0x0FFD, 0x0008}, // -104 dB, 16
  {0x0FFD, 0x0008}, // -103 dB, 17
  {0x0FFC, 0x0008}, // -102 dB, 18
  {0x0FFC, 0x0008}, // -101 dB, 19
  {0x0FFB, 0x0008}, // -100 dB, 20
  {0x0FFB, 0x0008}, // -99 dB, 21
  {0x0FFA, 0x0008}, // -98 dB, 22
  {0x0FF9, 0x0008}, // -97 dB, 23
  {0x0FF8, 0x0008}, // -96 dB, 24
  {0x0FF7, 0x0008}, // -95 dB, 25
  {0x0FF6, 0x0008}, // -94 dB, 26
  {0x0FF5, 0x0008}, // -93 dB, 27
  {0x0FF3, 0x0008}, // -92 dB, 28
  {0x0FF2, 0x0008}, // -91 dB, 29
  {0x0FF0, 0x0008}, // -90 dB, 30
  {0x0FEE, 0x0008}, // -89 dB, 31
  {0x0FEC, 0x0008}, // -88 dB, 32
  {0x0FE9, 0x0008}, // -87 dB, 33
  {0x0FE6, 0x0008}, // -86 dB, 34
  {0x0FE3, 0x0008}, // -85 dB, 35
  {0x0FDF, 0x0008}, // -84 dB, 36
  {0x0FDB, 0x0008}, // -83 dB, 37
  {0x0FD7, 0x0008}, // -82 dB, 38
  {0x0FD2, 0x0008}, // -81 dB, 39
  {0x0FCC, 0x0008}, // -80 dB, 40
  {0x0FC6, 0x0008}, // -79 dB, 41
  {0x0FBE, 0x0008}, // -78 dB, 42
  {0x0FB6, 0x0008}, // -77 dB, 43
  {0x0FAD, 0x0008}, // -76 dB, 44
  {0x0FA3, 0x0008}, // -75 dB, 45
  {0x0F98, 0x0008}, // -74 dB, 46
  {0x0F8B, 0x0008}, // -73 dB, 47
  {0x0F7D, 0x0008}, // -72 dB, 48
  {0x0F6D, 0x0008}, // -71 dB, 49
  {0x0F5B, 0x0008}, // -70 dB, 50
  {0x0F46, 0x0008}, // -69 dB, 51
  {0x0F30, 0x0008}, // -68 dB, 52
  {0x0F1B, 0x0008}, // -67 dB, 53
  {0x0F1B, 0x0009}, // -66 dB, 54
  {0x0F1B, 0x000A}, // -65 dB, 55
  {0x0F1B, 0x000B}, // -64 dB, 56
  {0x0F1B, 0x000C}, // -63 dB, 57
  {0x0F1B, 0x000E}, // -62 dB, 58
  {0x0F1B, 0x0010}, // -61 dB, 59
  {0x0F1B, 0x0012}, // -60 dB, 60
  {0x0F1B, 0x0014}, // -59 dB, 61
  {0x0F1B, 0x0016}, // -58 dB, 62
  {0x0F1B, 0x0019}, // -57 dB, 63
  {0x0F1B, 0x001C}, // -56 dB, 64
  {0x0F1B, 0x0020}, // -55 dB, 65
  {0x0F1B, 0x0024}, // -54 dB, 66
  {0x0F1B, 0x0028}, // -53 dB, 67
  {0x0F1B, 0x002D}, // -52 dB, 68
  {0x0F1B, 0x0033}, // -51 dB, 69
  {0x0F1B, 0x0039}, // -50 dB, 70
  {0x0F1B, 0x0040}, // -49 dB, 71
  {0x0F1B, 0x0048}, // -48 dB, 72
  {0x0F1B, 0x0051}, // -47 dB, 73
  {0x0F1B, 0x005B}, // -46 dB, 74
  {0x0F1B, 0x0066}, // -45 dB, 75
  {0x0F1B, 0x0073}, // -44 dB, 76
  {0x0F1B, 0x0081}, // -43 dB, 77
  {0x0F1B, 0x0090}, // -42 dB, 78
  {0x0F1B, 0x00A2}, // -41 dB, 79
  {0x0F1B, 0x00B6}, // -40 dB, 80
  {0x0F1B, 0x00CC}, // -39 dB, 81
  {0x0F1B, 0x00E5}, // -38 dB, 82
  {0x0F1B, 0x0101}, // -37 dB, 83
  {0x0F1B, 0x0121}, // -36 dB, 84
  {0x0F1B, 0x0144}, // -35 dB, 85
  {0x0F1B, 0x016C}, // -34 dB, 86
  {0x0F1B, 0x0198}, // -33 dB, 87
  {0x0F1B, 0x01CA}, // -32 dB, 88
  {0x0F1B, 0x0202}, // -31 dB, 89
  {0x0F1B, 0x0241}, // -30 dB, 90
  {0x0F1B, 0x0287}, // -29 dB, 91
  {0x0F1B, 0x02D6}, // -28 dB, 92
  {0x0F1B, 0x032F}, // -27 dB, 93
  {0x0F1B, 0x0392}, // -26 dB, 94
  {0x0F1B, 0x0402}, // -25 dB, 95
  {0x0F1B, 0x047F}, // -24 dB, 96
  {0x0F1B, 0x050C}, // -23 dB, 97
  {0x0F1B, 0x05A9}, // -22 dB, 98
  {0x0F1B, 0x065A}, // -21 dB, 99
  {0x0F1B, 0x0721}, // -20 dB, 100
  {0x0F1B, 0x0800}, // -19 dB, 101
  {0x0EFF, 0x0800}, // -18 dB, 102
  {0x0EDF, 0x0800}, // -17 dB, 103
  {0x0EBC, 0x0800}, // -16 dB, 104
  {0x0E94, 0x0800}, // -15 dB, 105
  {0x0E68, 0x0800}, // -14 dB, 106
  {0x0E36, 0x0800}, // -13 dB, 107
  {0x0DFE, 0x0800}, // -12 dB, 108
  {0x0DBF, 0x0800}, // -11 dB, 109
  {0x0D79, 0x0800}, // -10 dB, 110
  {0x0D2A, 0x0800}, //  -9 dB, 111
  {0x0CD1, 0x0800}, //  -8 dB, 112
  {0x0C6E, 0x0800}, //  -7 dB, 113
  {0x0BFE, 0x0800}, //  -6 dB, 114
  {0x0B81, 0x0800}, //  -5 dB, 115
  {0x0AF4, 0x0800}, //  -4 dB, 116
  {0x0A57, 0x0800}, //  -3 dB, 117
  {0x09A6, 0x0800}, //  -2 dB, 118
  {0x08DF, 0x0800}, //  -1 dB, 119
  {0x0800, 0x0800}, //   0 dB, 120
};

/*************************************Fader************************************/
const uint16 fader_table[BALANCE_MAX*2+1][2] =
    {
        { 0x000, 0x800 },  //left reduction: inf dB
        { 0xF7E, 0x800 },  //left reduction: 24 dB
        { 0xF33, 0x800 },  //left reduction: 20 dB
        { 0xEBB, 0x800 },  //left reduction: 16 dB
        { 0xE35, 0x800 },  //left reduction: 13 dB
        { 0xD78, 0x800 },  //left reduction: 10 dB
        { 0xC6D, 0x800 },  //left reduction: 7 dB
        { 0xB80, 0x800 },  //left reduction: 5 dB
        { 0xA56, 0x800 },  //left reduction: 3 dB
        { 0x9A5, 0x800 },  //left reduction: 2 dB
        { 0x8DE, 0x800 },  //left reduction: 1 dB
        { 0x800, 0x800 },  //no reduction ( 0 dB )
        { 0x800, 0x8DE },  //right reduction: 1 dB
        { 0x800, 0x9A5 },  //right reduction: 2 dB
        { 0x800, 0xA56 },  //right reduction: 3 dB
        { 0x800, 0xB80 },  //right reduction: 5 dB
        { 0x800, 0xC6D },  //right reduction: 7 dB
        { 0x800, 0xD78 },  //right reduction: 10 dB
        { 0x800, 0xE35 },  //right reduction: 13 dB
        { 0x800, 0xEBB },  //right reduction: 16 dB
        { 0x800, 0xF33 },  //right reduction: 20 dB
        { 0x800, 0xF7E },  //right reduction: 24 dB
        { 0x800, 0x000 }  //right reduction: inf dB
    };

const uint16 balance_table[BALANCE_MAX*2+1][2] =
    {
        { 0x000, 0x800 },  //left reduction: inf dB
        { 0xF7E, 0x800 },  //left reduction: 24 dB
        { 0xF33, 0x800 },  //left reduction: 20 dB
        { 0xEBB, 0x800 },  //left reduction: 16 dB
        { 0xE35, 0x800 },  //left reduction: 13 dB
        { 0xD78, 0x800 },  //left reduction: 10 dB
        { 0xC6D, 0x800 },  //left reduction: 7 dB
        { 0xB80, 0x800 },  //left reduction: 5 dB
        { 0xA56, 0x800 },  //left reduction: 3 dB
        { 0x9A5, 0x800 },  //left reduction: 2 dB
        { 0x8DE, 0x800 },  //left reduction: 1 dB
        { 0x800, 0x800 },  //no reduction ( 0 dB )
        { 0x800, 0x8DE },  //right reduction: 1 dB
        { 0x800, 0x9A5 },  //right reduction: 2 dB
        { 0x800, 0xA56 },  //right reduction: 3 dB
        { 0x800, 0xB80 },  //right reduction: 5 dB
        { 0x800, 0xC6D },  //right reduction: 7 dB
        { 0x800, 0xD78 },  //right reduction: 10 dB
        { 0x800, 0xE35 },  //right reduction: 13 dB
        { 0x800, 0xEBB },  //right reduction: 16 dB
        { 0x800, 0xF33 },  //right reduction: 20 dB
        { 0x800, 0xF7E },  //right reduction: 24 dB
        { 0x800, 0x000 }  //right reduction: inf dB
    };

/***************************************************/
/*  APIs                                           */
/*  library  interfaces                            */
/***************************************************/
/** ************************************************  **/
/**   API    : initDirana                              */
/**   RETURN : OK or ERROR                             */
/** ************************************************* **/
bool initDirana(void)
{
   bool l_res = OK;

   l_res = dspStart(2);

   if(OK==l_res) initTuner();

   setVolumeDirana(20);

   return (l_res);
}


/** ************************************************  **/
/**   API    : doMute                              */
/**   RETURN : OK or ERROR                             */
/** ************************************************* **/
bool doMuteDirana(int val)
{
   bool l_res = OK;

	switch(val)
	{
	 case 0:
	 {
		if(l_res == OK)
		{
		   /** disable MUTE on primary channel*/
		   l_res = dspWrite(ADSP_Y_Mute_P,Y_REG_SIZE,0x000800);
		}

		if(l_res == OK)
		{
		   /** disable MUTE on secondary channel*/
		   l_res = dspWrite(ADSP_Y_Mute_S,Y_REG_SIZE,0x000800);
		}
	 }
	 break;
	 case 1:
	 {
		if(l_res == OK)
		{
		   /** muting primary channel*/
		   l_res = dspWrite(ADSP_Y_Mute_P,Y_REG_SIZE,0x000000);
		}

		if(l_res == OK)
		{
		   /** muting secondary channel*/
		   l_res = dspWrite(ADSP_Y_Mute_S,Y_REG_SIZE,0x000000);
		}
	 }
	 break;
	 default:
	 {
		l_res = ERROR;
	 }
	 break;
	}

return(l_res);
}

/** ************************************************  **/
/**   API    : setSource                               */
/**   RETURN : OK or ERROR                             */
/** ************************************************* **/
bool setSourceDirana(AUDIO_SOURCES_TYPES p_source )
{
   bool l_res = OK;
   //disable sin
   dspWrite(ADSP_X_EasyP_Index,X_REG_SIZE,ADSP_EASYP_NoiseGen_SinGen_CheckSum_Disable);

   switch(p_source)
   {
      case SRC_TUNER:
      {
         l_res = SetTunerOnPrimaryStereoDirana();
      }
      break;
      case SRC_CD:
      {
         l_res = SetI2SOnPrimaryStereoDirana(2);
      }
      break;
      case SRC_MP3:
      {
         l_res = SetI2SOnPrimaryStereoDirana(2);
      }
      break;
      case SRC_CDC:
      {
//         l_res = SetCdcOnPrimaryStereoDirana();
      }
      break;
      case SRC_AUX:
      {
//         l_res = SetAuxOnPrimaryStereoDirana();
      }
      break;
      case SRC_PHONE:
      {
//         l_res = SetPhoneOnPrimaryStereoDirana();
      }
      break;
	  default:
      {
         //return ERROR;
         l_res = ERROR;
         break;
	  }
   }

   if(l_res == OK)
   {
      g_src_active = p_source;
   }

   return (l_res);

}

/** ************************************************  **/
/**   API    : getSource                               */
/**   RETURN : OK or ERROR                             */
/** ************************************************* **/
bool getSourceDirana(AUDIO_SOURCES_TYPES  *p_source )
{

   *p_source = g_src_active;
   
   return (OK);

}


/** ************************************************* **/
/**   API    : setVolume                               */
/**   RETURN : OK or ERROR                             */
/** ************************************************* **/
bool setVolumeDirana(int p_volume)
{
   bool l_res = OK;

   union USUC l_uservol;

   l_uservol.us  =  VolumeArrayForStdMode[UserVolumeArrayStMode[p_volume]][0];

   if(l_res == OK)
   {
      l_res = dspWrite(ADSP_Y_Vol_Main1P,Y_REG_SIZE,l_uservol.us);
   }

   l_uservol.us  =  VolumeArrayForStdMode[UserVolumeArrayStMode[p_volume]][1];

   if(l_res == OK)
   {
      l_res = dspWrite(ADSP_Y_Vol_Main2P,Y_REG_SIZE,l_uservol.us);

   }

   l_uservol.us  =  VolumeArrayForStdMode[UserVolumeArrayStMode[p_volume]][0];

    if ( l_res == OK )
    {
    	l_res = dspWrite( ADSP_Y_Vol_Main1S, Y_REG_SIZE, l_uservol.us );
    }

    l_uservol.us  =  VolumeArrayForStdMode[UserVolumeArrayStMode[p_volume]][1];

    if ( l_res == OK )
    {
    	l_res = dspWrite( ADSP_Y_Vol_Main2S, Y_REG_SIZE, l_uservol.us );
    	 g_current_volume = p_volume;
    }


   return (l_res);

}

/** ************************************************* **/
/**   API    : getVolume                              **/
/**   RETURN : OK or ERROR                            **/
/** ************************************************* **/
bool getVolumeDirana(int *p_volume)
{
      bool l_res = OK;
      int value1 = 0,value2 = 0,i = 0;

      if(l_res == OK)
      {
         l_res = dspRead(ADSP_Y_Vol_Main1P,Y_REG_SIZE,&value1);
      }

      if(l_res == OK)
      {
         l_res = dspRead(ADSP_Y_Vol_Main2P,Y_REG_SIZE,&value2);
      }

      if(l_res == OK)
      {
         for(i=0;i<MAXVOLSTEP;i++)
         {  
            if((VolumeArrayForStdMode[UserVolumeArrayStMode[i]][0] == value1)&&(VolumeArrayForStdMode[UserVolumeArrayStMode[i]][1] == value2 ))
            {
               *p_volume = i;
               return (OK);
            }
         }

      }

      return (l_res);
}


/***************************************************************************/
/*   NAME: SetLoudness                                                     */
/*                                                                         */
/*   GOAL:  ON7OFF Static Loudness                                         */
/*                                                                         */
/* RESULT: true (Success) false (Failed)                                   */
/*                                                                         */
/***************************************************************************/
bool SetLoudnessDirana( uint8 p_loudness )
{
	bool l_result = OK;
	int p_volume;
	p_loudness = LOUDNESS_ON;

	getVolume(&p_volume);

	if(p_loudness == LOUDNESS_ON)
	{
		l_result = dspWrite( ADSP_Y_Loudf_StatLev, Y_REG_SIZE, loudness_boost_table[p_volume]);
		if( OK == l_result )
		l_result = dspWrite( ADSP_Y_Loudf_StatLevT, Y_REG_SIZE, loudness_boost_table[p_volume]);

	}
	else
	{
		l_result = dspWrite( ADSP_Y_Loudf_StatLev, Y_REG_SIZE, 0x7FF);
		l_result = dspWrite( ADSP_Y_Loudf_StatLevT, Y_REG_SIZE, 0x7FF);

	}

	return l_result;
}

/***************************************************************************/
/*   NAME: SetBalance                                                      */
/*                                                                         */
/*   GOAL: This function allows the user to move the sound                 */
/*         to the left channels or to the right channels                   */
/*                                                                         */
/* RESULT: true (Success) false (Failed)                                     */
/*                                                                         */
/***************************************************************************/


bool SetBalanceDirana( sint8 p_balance )   //!< the balance step (enum t_audio_balance_step)
{
	bool l_res = OK;
	uint16 l_value_left;
	uint16 l_value_right;
	uint8 l_index = 0;

	if(BALANCE_MIN > p_balance || BALANCE_MAX < p_balance)
	{
		l_res = ERROR;
	}
	else if(0 <= p_balance)
	{
		l_index = (uint8)(p_balance + BALANCE_MAX);
	}
	else
	{
		l_index = (uint8)(p_balance - (BALANCE_MIN));
	}

	l_value_left  = balance_table[l_index][0];
	l_value_right = balance_table[l_index][1];

	if( OK == l_res ) l_res = dspWrite( ADSP_Y_Vol_BalPL, Y_REG_SIZE, l_value_left );
	if( OK == l_res ) l_res = dspWrite( ADSP_Y_Vol_BalPR, Y_REG_SIZE, l_value_right );

	return l_res;
}

/***************************************************************************/
/*   NAME: SetFader                                                        */
/*                                                                         */
/*   GOAL: This function allows the user to move the sound                 */
/*          to the front channels or to the rear channels                  */
/*                                                                         */
/* RESULT: true (Success) false (Failed)                                     */
/*                                                                         */
/***************************************************************************/
bool SetFaderDirana( sint8 p_fader )   //!< the fader step (enum t_audio_fader_step)
{
	bool l_res = OK;
	uint16 l_value_front;
	uint16 l_value_rear;
	uint8 l_index = 0;

	if( FADER_MIN > p_fader || FADER_MAX < p_fader )
	{
		l_res = ERROR;
	}
	else if( 0 <= p_fader )
	{
		l_index = ( uint8 )(p_fader + FADER_MAX);
	}
	else
	{
		l_index = ( uint8 )(p_fader - (FADER_MIN));
	}

	l_value_front = fader_table[l_index][0];
	l_value_rear  = fader_table[l_index][1];
	if( OK == l_res ) l_res = dspWrite( /*ADSP_Y_Mute_GC2*/ADSP_Y_Vol_FadF, Y_REG_SIZE, l_value_front );
	if( OK == l_res ) l_res = dspWrite( /*ADSP_Y_Mute_GC1*/ADSP_Y_Vol_FadR, Y_REG_SIZE, l_value_rear );
	return l_res;
}


/** *********************************************************/
/**   API    : playSin                                	 	*/
/* @param p_enable Represent the start/stop operation   	*/
/* @param p_volume The volume steps (0-79)					*/
/* @param p_frequency The frequency in Hz (range 20-20000) 	*/
/**   RETURN : OK or ERROR                           	  	*/
/** *********************************************************/
bool playSinDirana(int p_volume, int p_frequency, bool p_enable)
{
   bool l_res = OK;
   unsigned short l_low = 0;
   unsigned short l_hi = 0;

   if( p_enable == true )
   {

 	if(OK == l_res)
     {
       l_res = dspWrite( ADSP_X_SChanSelPntr, X_REG_SIZE, ADSP_X_SinGen_OutL_REL );
     }

     if( OK == l_res )
       l_res = dspWrite( ADSP_X_EasyP_Index, X_REG_SIZE, ADSP_EASYP_SinGen_Enable );

     // Channel volume to -12.0 Db
     if( OK == l_res )
       l_res = dspWrite( ADSP_Y_SinGen_GL, Y_REG_SIZE, 0x000800 );

     if( OK == l_res )
       l_res = dspWrite( ADSP_Y_SinGen_GR, Y_REG_SIZE, 0x000800 );

     DIRANA_get_waveform_registers(p_frequency, &l_hi, &l_low); //da aggiungere se vogliamo modificare la forma d'onda in base alla frequenza
//     l_low = 0x0001FD;//forma d'onda fissa
//     l_hi = 0x0007EB;

     // Set the waveform
     if( OK == l_res )
       l_res = dspWrite( ADSP_Y_SinGen_aHbak, Y_REG_SIZE, l_hi);

     if( OK == l_res )
       l_res = dspWrite( ADSP_Y_SinGen_aLbak, Y_REG_SIZE,  l_low);

     if( OK == l_res )
        {
//          g_pre_sin_volume = g_current_volume;
          l_res = setVolume( p_volume );
        }

     if( OK == l_res )
       l_res = dspWrite( ADSP_Y_SinGen_Init, Y_REG_SIZE, 0x001 );

     //Detach PDC from Path
      if( OK == l_res )
        l_res = dspWrite( ADSP_X_SrcScal6_F_InPntr, X_REG_SIZE, 0 );
      //Detach GONG from Path
      if( OK == l_res )
       l_res = dspWrite( ADSP_X_PhonBackInPntr, X_REG_SIZE, 0);

   }
   else
   {
     l_res = dspWrite( ADSP_X_EasyP_Index,
                           X_REG_SIZE,
                           ADSP_EASYP_NoiseGen_SinGen_CheckSum_Disable );
     //Reattach PDC
     if( OK == l_res )
       l_res = dspWrite( ADSP_X_SrcScal6_F_InPntr, X_REG_SIZE, ADSP_X_SRC2InL_REL );
     //Reattach Gong
     if( OK == l_res )
      l_res = dspWrite( ADSP_X_PhonBackInPntr, X_REG_SIZE, ADSP_X_SRC2InR_REL );

     if( OK == l_res )
     {
 		l_res = setSource(  SRC_MP3  );
 	}

    l_res = setVolume(g_current_volume);
   }

 return l_res;
}


/** ************************************************  **/
/**   API    : SetAuxOnPrimaryStereo                                 */
/**   RETURN : OK or ERROR                             */
/** ************************************************* **/
bool SetAuxOnPrimaryStereoDirana()
{
   bool l_res = OK;
   PrintMessage(RED_PRINT,"SetAuxOnPrimaryStereo: setting oux on primary stereo...");

   if(l_res == OK)
   {
      l_res = dspWrite(ADSP_Y_Mute_P,Y_REG_SIZE,0x000000);  /** muting primary channel*/
   }
   if(l_res == OK)
   {
   /** AUX  come input del primary  */
      l_res = dspWrite(ADSP_X_EasyP_Index,X_REG_SIZE,ADSP_EASYP_SrcSw_AIN0onA);
   }
   if(l_res == OK)
   {
      l_res = dspWrite(ADSP_Y_Vol_SrcScalP,Y_REG_SIZE,0x12e); /** setting scaler**/
   }
   if(l_res == OK)
   {
      l_res = dspWrite(ADSP_Y_Mute_P,Y_REG_SIZE,0x000800);    /** disable MUTE on primary channel*/
   }
   setVolume(19);

   return (l_res);
}

/** ************************************************  **/
/**   API    : SetCdcOnPrimaryStereo                                 */
/**   RETURN : OK or ERROR                             */
/** ************************************************* **/
//STATUS SetCdcOnPrimaryStereoDirana()
//{ //TEST
//
//   STATUS l_res = OK;
//
//   PrintMessage(RED_PRINT,"SetCDCOnPrimaryStereo: setting CDC on primary stereo...");
//
//   if(l_res == OK)
//   {
//      /** muting primary channel*/
//      dspWrite(ADSP_Y_Mute_P,Y_REG_SIZE,0x000000);
//   }
//
//   if(l_res == OK)
//   {
//      /** CDC  come input del primary  */
//      dspWrite(ADSP_X_EasyP_Index,X_REG_SIZE,ADSP_EASYP_SrcSw_AIN2onA);
//   }
//
//   if(l_res == OK)
//   {
//      /** setting scaler**/
//      dspWrite(ADSP_Y_Vol_SrcScalP,Y_REG_SIZE,0x12e);
//   }
//
//   if(l_res == OK)
//   {
//      /** disable MUTE on primary channel*/
//      dspWrite(ADSP_Y_Mute_P,Y_REG_SIZE,0x000800);
//   }
//
//   setVolume(1);
//
//   return (l_res);
//}
//
//
///** ************************************************  **/
///**   API    : SetPhoneOnPrimaryStereo                                 */
///**   RETURN : OK or ERROR                             */
///** ************************************************* **/
//STATUS SetPhoneOnPrimaryStereoDirana()
//{
//
//   STATUS l_res = OK;
//
//   PrintMessage(RED_PRINT,"SetPhoneOnPrimaryStereo: setting Phone on primary stereo...");
//
//   if(l_res == OK)
//   {
//      /** muting primary channel*/
//      l_res = dspWrite(ADSP_Y_Mute_P,Y_REG_SIZE,0x000000);
//   }
//
//   if(l_res == OK)
//   {
//      /** Phone  come input del primary  */
//      l_res = dspWrite(ADSP_X_EasyP_Index,X_REG_SIZE,ADSP_EASYP_SrcSw_AIN3ViaAD1onNav);
//   }
//   if(l_res == OK)
//   {
//      /** setting scaler**/
//      l_res = dspWrite(ADSP_Y_Vol_SrcScalP,Y_REG_SIZE,0x12e);
//   }
//   if(l_res == OK)
//   {
//      /** disable MUTE on primary channel*/
//      l_res = dspWrite(ADSP_Y_Mute_P,Y_REG_SIZE,0x000800);
//   }
//
//   setVolume(1);
//
//   return (l_res);
//}


/** ************************************************  **/
/**   API    : SetI2SOnPrimaryStereo                                 */
/**   RETURN : OK or ERROR                             */
/** ************************************************* **/
bool SetI2SOnPrimaryStereoDirana(int p_bus)
{
   bool l_res = OK;

   PrintMessage(RED_PRINT,"SetI2SOnPrimaryStereo: setting oux on primary stereo...");

//   int l_value = 0;
   switch(p_bus)
   {
      case 1:
      {
         /** I2S1 come input del primary  */
//         l_value = ADSP_EASYP_SrcSw_I2S1onA;
      }
      break;
      case 2:
      {
         /** I2S2 come input del primary  */
	   	  l_res = dspWrite( ADSP_X_SChanSelPntr, X_REG_SIZE, ADSP_X_HIIS2InL_REL);   //SL;
      }
      break;
      case 3:
      {
         /** I2S3 come input del primary  */
//         l_value = ADSP_EASYP_SrcSw_I2S3onA;
      }
      break;
      case 4:
      {
         /** I2S4 come input del primary  */
//         l_value = ADSP_EASYP_SrcSw_I2S4onA;
      }
      break;

      default:
      {
         //return ERROR;
		 //l_flag_check = false;
		 l_res = ERROR;
		 break;
      }
   }

#if 0

   if(l_res == OK)
   {
     l_res = dspWrite(ADSP_X_EasyP_Index,X_REG_SIZE,l_value);
   }

#endif

   if(l_res == OK ) l_res = dspWrite( ADSP_X_SProcSelPntr, X_REG_SIZE, ADSP_X_DCfilt_OutSL_REL );

	if(l_res == OK )
		    {
  l_res = dspWrite(ADSP_Y_Vol_SrcScalP,Y_REG_SIZE,0x0400);
		    }

  l_res = dspWrite(ADSP_X_LEF_InPntr,Y_REG_SIZE,ADSP_X_Loudf_OutL_REL);


   return (l_res);
}


/** ************************************************  **/
/**   API    : SetTunerOnPrimaryStereo                                 */
/**   RETURN : OK or ERROR                             */
/** ************************************************* **/
bool SetTunerOnPrimaryStereoDirana()
{ 

   bool l_res = OK;

   unsigned char tuner_init[] = {
   		        0x30,0x87,0xA8,0x00,0x00,0x00,0x00
   		    };
//	unsigned char tuner_read[TUNER_REG_SIZE_READ] ={ 0x00 };

   PrintMessage(RED_PRINT,"SetTunerOnPrimaryStereo: setting Tuner on primary stereo...");

	int l_rval =0;
	  if( OK == l_res )
			l_res = dspRead( ADSP_DIGSRC_SEL, X_REG_SIZE, (int*)&l_rval );

		  l_rval &= 0xFFFFFE;
		  l_rval |= 0x000006;

		  if( OK == l_res )
			l_res = dspWrite( ADSP_DIGSRC_SEL, X_REG_SIZE, l_rval );
		  if( OK == l_res )
			l_res = dspWrite( ADSP_X_SChanSelPntr, X_REG_SIZE, ADSP_X_SRC1InL_REL );
		  if( OK == l_res )
			l_res = dspWrite( ADSP_X_SProcSelPntr, X_REG_SIZE, ADSP_X_DCfilt_OutSL_REL );
          if(l_res == OK )
   		    {
      			l_res = dspWrite(ADSP_Y_Vol_SrcScalP,Y_REG_SIZE,0x0400);
   		    }


   if(l_res== OK)
   {
	   l_res =dspTunerWrite( TUNER_T1A_ADDRESS, TUNER_REG_SIZE_WRITE, &tuner_init[0] );
	   usleep(DELAY_TIME);
//	   if(l_res== OK)
//	    {
//		   l_res = dspTunerRead(TUNER_T1A_ADDRESS, TUNER_REG_SIZE_READ, &tuner_read[0]);
//	    }
   }

   return(l_res);
}

bool change_bandDirana(TYPE_BAND p_new_band)
{
	 PrintMessage(RED_PRINT,"change_band the band: %d", p_new_band);
	bool l_ret = OK;
	 if ( FM_BAND == p_new_band)
	 {
		 PrintMessage(RED_PRINT,"FM BAND");
		 if(FM_BAND !=g_current_band)
	 	{
			 g_current_band = FM_BAND;
			 /* rad_mode : FM MainTuner */
			 l_ret = dspWrite( TDSP1_X_FW_RadMod, X_REG_SIZE, 0x000052 );
			 /* FM mode - level detector FM function */
	 		if( OK == l_ret)	 l_ret =  dspWrite(TDSP1E_Y_FW_FDLE_1_RanSlp, Y_REG_SIZE, 0x00E8 );
	 		 /* FM Stereo decoder to maximize channel separation */
	 		if( OK == l_ret) l_ret = dspWrite( TDSP1E_Y_FMLR_1_GanCor, Y_REG_SIZE, ( g_tuner_DiranaAlignment.FM_Stereo_decoder & 0x0FFF ) );
	 		/* FM mode - level detector FM function */
	 		if( OK == l_ret) l_ret = dspWrite( TDSP1E_Y_FW_FDLE_1_RanOfs, Y_REG_SIZE, ( g_tuner_DiranaAlignment.FM_Level_Main_tuner & 0x0FFF ) );
			  /* FM frequency offset detector alignment for Main tuner */
	 		if( OK == l_ret) l_ret = dspWrite( PDC2_FMIX_0_REG, X_REG_SIZE, ( g_tuner_DiranaAlignment.FM_Offset_Main_tuner_Europe_H & 0xFFFFFF ) );
	 		if( OK == l_ret) l_ret = dspWrite( PDC2_FMIX_1_REG, X_REG_SIZE, ( g_tuner_DiranaAlignment.FM_Offset_Main_tuner_Europe_L & 0xFFFFFF ) );
	}
	 }
  else
 	{
		 PrintMessage(RED_PRINT,"AM BAND");
		if(AM_BAND !=g_current_band)
	 	{
	 		//g_current_band = AM_BAND;
	 		/* rad_mode : AM MainTuner */
			l_ret = dspWrite( TDSP1_X_FW_RadMod, X_REG_SIZE, 0x000051 );
	 		//default MW da aggiungere controllo sottobanda
	 		if(1)
	 		{
	 		/* AM mode - level detector MW function */
	 			if( OK == l_ret) l_ret = dspWrite(TDSP1E_Y_FW_ADLE_1_RanOfs, Y_REG_SIZE, ( g_tuner_DiranaAlignment.MW_Level_Main_tuner & 0x0FFF ) );
			/* MW frequency offset detector alignment for Main tuner */
	 			if( OK == l_ret) l_ret = dspWrite( PDC2_FMIX_0_REG, X_REG_SIZE, ( g_tuner_DiranaAlignment.MW_Offset_tuner_Europe_H & 0xFFFFFF ) );
	 			if( OK == l_ret) l_ret = dspWrite( PDC2_FMIX_1_REG, X_REG_SIZE, ( g_tuner_DiranaAlignment.MW_Offset_tuner_Europe_L & 0xFFFFFF ) );
	 		}
	 		else
	 		{//kw
			 /* AM mode - level detector KW function */
	 			l_ret = dspWrite( TDSP1E_Y_FW_ADLE_1_RanOfs, Y_REG_SIZE, ( g_tuner_DiranaAlignment.KW_Level_Main_tuner & 0xFFFF ) );

		  	/* KW frequency offset detector alignment for Main tuner */
			 if( OK == l_ret) l_ret = dspWrite( PDC2_FMIX_0_REG, X_REG_SIZE, ( g_tuner_DiranaAlignment.KW_Offset_tuner_H & 0xFFFFFF ) );
			 if( OK == l_ret) l_ret = dspWrite( PDC2_FMIX_1_REG, X_REG_SIZE, ( g_tuner_DiranaAlignment.KW_Offset_tuner_L & 0xFFFFFF ) );
	 		}
	 	}
 	}

return l_ret;
}



bool set_frequencyDirana(int p_frequency, TYPE_BAND p_band, TYPE_SUB_BAND p_subAM_band)
{
	PrintMessage(RED_PRINT,"set_frequency: %d", p_frequency );
	int l_res;
//	unsigned char tuner_read[TUNER_REG_SIZE_READ] ={ 0x00 };
	set_LeafDiceCfgDirana(p_frequency,p_band,  p_subAM_band);
	l_res = dspTunerWrite( TUNER_T1A_ADDRESS, TUNER_REG_SIZE_WRITE, &data_T1A[0] );
//	if(l_res == OK)
//	{
//		l_res = dspTunerRead(TUNER_T1A_ADDRESS, TUNER_REG_SIZE_READ, &tuner_read[0]);
//	}
	return l_res;
}



int get_frequencyDirana()
{
	int l_freq_res = 0;
	TYPE_BAND l_band = 0;
	TYPE_SUB_BAND l_subband = 0;
	get_Freq_Band_fromTunerDirana(&l_freq_res, &l_band,  &l_subband);
	PrintMessage(GREEN_PRINT,"get_frequency -- l_freq_res = %d \t l_current_band = %d \t l_current_sub_band = %d",l_freq_res,l_band, l_subband );
	return l_freq_res;
}

void get_bandDirana(TYPE_BAND p_band, TYPE_SUB_BAND p_subband)
{
	int l_freq_res = 0;

	get_Freq_Band_fromTunerDirana(&l_freq_res, &p_band, &p_subband);
	PrintMessage(GREEN_PRINT,"get_band -- l_freq_res = %d \t l_current_band = %d  \t l_current_sub_band = %d ",l_freq_res,p_band, p_subband );

}

bool get_Freq_Band_fromTunerDirana(int *p_freq, TYPE_BAND *p_band, TYPE_SUB_BAND *p_subband)//only main tuner
{
	  bool l_res = OK;
	  unsigned char tuner_read[TUNER_REG_SIZE_READ] ={ 0x00 };
	  l_res = dspTunerRead(TUNER_T1A_ADDRESS, TUNER_REG_SIZE_READ, &tuner_read[0]);
	  int l_freq;
	  char l_band;
	  l_freq = (int) (((tuner_read[4] & 0x1F) << 8) | tuner_read[5]);


	  l_band = (TYPE_BAND) ((tuner_read[4]) & 0xE0);
	  if ( 0x80 == l_band)
	  {
		  *p_band =  FM_BAND;
		  *p_freq= l_freq *50;
	  }
	  else
	  {
		  *p_band =  AM_BAND;
		  if ( 0x20 == l_band)
		  {
		  *p_subband = AM_MW_BAND;
		  *p_freq= l_freq;
		  }
		  else if ( 0x60 == l_band)
		  {
			*p_subband = AM_KW_BAND;
			*p_freq= l_freq *5;
		  }

	  }

	  PrintMessage(GREEN_PRINT,"get_Freq_Band_fromTunerDirana --  p_freq = %d \t  p_band = %d \t p_subband = %d ", *p_freq, *p_band, *p_subband);
	  return l_res;
}

bool set_LeafDiceCfgDirana(int p_freq, TYPE_BAND p_band, TYPE_SUB_BAND p_subAM_band)//only main tuner
{
	PrintMessage(RED_PRINT,"InitLeafDiceCfg");
    bool l_res = OK;
//    char l_Freqbuff[2] = { 0x00, 0x00 };
    char l_band_reg;
    int l_f = 0;

    switch (p_band)
    {
        case AM_BAND://MW default
        	if(AM_MW_BAND == p_subAM_band)//MW default
        	{
           	 l_band_reg = 0x20;
           	 l_f = (p_freq);
        	}
        	else if(AM_KW_BAND == p_subAM_band)
        	{
        	 l_band_reg = 0x60;
        	 l_f = (p_freq)/5;
        	}
            break;
        case FM_BAND:
            l_band_reg = 0x80;
            l_f = (p_freq)/(50);
            break;
       default:
            l_res = ERROR;
            break;
    }

    if ( l_res == OK )
    {

            data_T1A[1] = ( ( (TYPE_BAND)((l_f & 0x00001f00) >> 8) ) | l_band_reg );
            data_T1A[2] = (TYPE_BAND)( l_f & 0x000000ff );
            data_T1A[5] = 0x00;
            if ( p_band == FM_BAND )
            {
                data_T1A[3] = 0xA0;
                data_T1A[4] = (TYPE_BAND)((g_tuner_DiranaAlignment.FM_IF_Filter_Center_Freq_Main_tuner & 0x0000000F)|(0xC0));
                data_T1A[6] = 0x04;
            }
            else
            {
                data_T1A[3] = 0x20;
                data_T1A[4] = (TYPE_BAND)(g_tuner_DiranaAlignment.AM_IF_Filter_Center_Freq_Main_tuner & 0x0000000F);
                data_T1A[6] = 0x00;
            }
   }
    return(l_res);
}
