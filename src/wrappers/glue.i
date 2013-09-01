%module dsp

%{
#include "libDSP.h"
%}

%include "enumtypeunsafe.swg"
%javaconst(1);

enum DR_AUDIO_SOURCES{
   SRC_NO_SOURCE,
   SRC_TUNER,
   SRC_CD,
   SRC_MP3,
   SRC_CDC,
   SRC_AUX,
   SRC_PHONE,//12s4
   SRC_TTS,
   SRC_TA_PTY,
   SRC_TTS_ON_AUX,
   SRC_AUX_CONVERGENCE,
   SRC_MAX_AUDIO_SOURCES
};

/*
enum  DR_DSP_STATUS{
   ON,
   OFF,
   DR_ERROR
};
*/


enum AMP_STATUS{

   AMP_ON,
   AMP_OFF
};



enum STATUS{

   ERROR,
   OK
};


enum AMPLI_CHANNEL{

   AMP_NO_CHANNEL,
   AMP_FRONT_CHANNEL,
   AMP_REAR_CHANNEL,
   AMP_END_CHANNEL

};


enum AMPLI_GAINS{
   NO_GAIN,
   LOW_GAIN12,
   HIGH_GAIN26,
   END_GAIN

};



enum TYPE_BAND
{
	FM_BAND,
	AM_BAND
};

enum TYPE_SUB_BAND
{
	AM_MW_BAND,
	AM_KW_BAND
};

%inline %{
int g_current_volume;
%}

#define VOLUMEINDBTABLE  121
#define MAXVOLSTEP 30 + 1


extern bool initDSP(void);

/*
 *  bool dspEnd(void)
 *  @brief: close device i2c to disconnect the DSP
 *  @return: 1 if OK, 0 otherwise
*/
extern bool dspEnd();


/*
 *  bool setVolume(int p_volume )
 *  @brief: set volume between 0 and 30 unit.
 *  @return: 1 if OK, 0 otherwise
*/
extern bool setVolume(int p_volume);

/*
 *  bool getVolume(int *p_volume )
 *  @brief: get volume between 0 and 30 unit.
 *  @return: 1 if OK, 0 otherwise
*/
extern bool getVolume(int *p_volume);

/*
 *  bool increase_Volume()
 *  @brief: increase_Volume.
 *  @return: 1 if OK, 0 otherwise
*/

extern bool increase_Volume();
/*
 *  bool decrease_Volume)
 *  @brief: decrease_Volume.
 *  @return: 1 if OK, 0 otherwise
*/
extern bool decrease_Volume();

/*
 *  bool doMute(void)
 *  @brief: mute audio sources.
 *  @return: 1 if OK, 0 otherwise
*/
extern bool doMute(void);

/*
 *  bool doMute(void)
 *  @brief: demute audio sources.
 *  @return: 1 if OK, 0 otherwise
*/
extern bool doDeMute(void);

/*
 *  bool SetBalance(void)
 *  @brief: SetBalance audio value between  [-11/+11].
 *  @return: 1 if OK, 0 otherwise
*/
extern bool SetBalance( int p_balance );

/*
 *  bool SetFader(void)
 *  @brief: SetFader audio value between  [-11/+11].
 *  @return: 1 if OK, 0 otherwise
*/
extern bool SetFader( int p_fader );

/*
 *  bool SetLoudness(void)
 *  @brief: enable or disable audio Loudness.
 *  @return: 1 if OK, 0 otherwise
*/
extern bool SetLoudness( int p_loudness );

/*
 *  bool setSource(int p_source )
 *  @brief: select and connect a new audio source 
 *  @return: 1 if OK, 0 otherwise
*/
extern bool setSource(int p_source );

/*
 *  bool getSource(int *p_source )
 *  @brief: get current audible audio source
 *  @return: 1 if OK, 0 otherwise
*/
extern bool getSource(int *p_source);

/*
 *  bool playSin(int p_volume )
 *  @brief: play a sin with setted volume amplitude.
 *  @return: 1 if OK, 0 otherwise
*/
extern bool playSin(int p_volume,int p_frequency, bool p_enable);

/*
 *  bool change_band(int p_new_band)
 *  @brief: change band (write on Tuner device).
 *  @return: 1 if OK, 0 otherwise
*/
extern bool change_band(int p_new_band);

/*
 *  bool get_band(int p_new_band)
 *  @brief: get current band (read on Tuner device).
 *  @return: 1 if OK, 0 otherwise
*/
extern void get_band(int p_band, int p_subband);

/*
 *  bool set_frequency(int p_frequency, int p_band, int p_subAM_band)
 *  @brief: set a frequency in to related band and subband (write on Tuner device).
 *  @return: 1 if OK, 0 otherwise
*/
extern bool set_frequency(int p_frequency, int p_band, int p_subAM_band);


/*
 *  bool set_frequency(int p_frequency, int p_band, int p_subAM_band)
 *  @brief: get current frequency (read on Tuner device).
 *  @return: int current frequency
*/
extern int get_frequency();

