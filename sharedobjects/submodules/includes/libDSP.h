/******************************************************/
/*****   libDSP  Header file  	********************/
/*****   Akhela                 **********************/
/******************************************************/

#ifndef LIBDSP_H
#define LIBDSP_H


#include <stdbool.h>
#include "def_variables_DSP.h"

/***************************************************/
/*  APIs                                           */
/*  library  interfaces                            */
/***************************************************/
/*
 *  bool initDSP(void)
 *  @brief: open device i2c to connect the DSP,
 *		execute initialization procedure of DSPs
 *  @return: 1 if OK, 0 otherwise
*/
bool initDSP(void);

/*
 *  bool dspEnd(void)
 *  @brief: close device i2c to disconnect the DSP
 *  @return: 1 if OK, 0 otherwise
*/
bool dspEnd();


/*
 *  bool setVolume(int p_volume )
 *  @brief: set volume between 0 and 30 unit.
 *  @return: 1 if OK, 0 otherwise
*/
bool setVolume(int p_volume);

/*
 *  bool getVolume(int *p_volume )
 *  @brief: get volume between 0 and 30 unit.
 *  @return: 1 if OK, 0 otherwise
*/
bool getVolume(int *p_volume);

/*
 *  bool increase_Volume()
 *  @brief: increase_Volume.
 *  @return: 1 if OK, 0 otherwise
*/

bool increase_Volume();
/*
 *  bool decrease_Volume)
 *  @brief: decrease_Volume.
 *  @return: 1 if OK, 0 otherwise
*/
bool decrease_Volume();

/*
 *  bool doMute(void)
 *  @brief: mute audio sources.
 *  @return: 1 if OK, 0 otherwise
*/
bool doMute(void);

/*
 *  bool doMute(void)
 *  @brief: demute audio sources.
 *  @return: 1 if OK, 0 otherwise
*/
bool doDeMute(void);

/*
 *  bool SetBalance(void)
 *  @brief: SetBalance audio value between  [-11/+11].
 *  @return: 1 if OK, 0 otherwise
*/
bool SetBalance( sint8 p_balance );

/*
 *  bool SetFader(void)
 *  @brief: SetFader audio value between  [-11/+11].
 *  @return: 1 if OK, 0 otherwise
*/
bool SetFader( sint8 p_fader );

/*
 *  bool SetLoudness(void)
 *  @brief: enable or disable audio Loudness.
 *  @return: 1 if OK, 0 otherwise
*/
bool SetLoudness( uint8 p_loudness );

/*
 *  bool setSource(AUDIO_SOURCES_TYPES p_source )
 *  @brief: select and connect a new audio source 
 *  @return: 1 if OK, 0 otherwise
*/
bool setSource(AUDIO_SOURCES_TYPES p_source );

/*
 *  bool getSource(int *p_source )
 *  @brief: get current audible audio source
 *  @return: 1 if OK, 0 otherwise
*/
bool getSource(AUDIO_SOURCES_TYPES *p_source);

/*
 *  bool playSin(int p_volume )
 *  @brief: play a sin with setted volume amplitude.
 *  @return: 1 if OK, 0 otherwise
*/
bool playSin(int p_volume,int p_frequency, bool p_enable);

/*
 *  bool change_band(char p_new_band)
 *  @brief: change band (write on Tuner device).
 *  @return: 1 if OK, 0 otherwise
*/
bool change_band(char p_new_band);

/*
 *  bool get_band(char p_new_band)
 *  @brief: get current band (read on Tuner device).
 *  @return: 1 if OK, 0 otherwise
*/
void get_band(char p_band, char p_subband);

/*
 *  bool set_frequency(int p_frequency, char p_band, char p_subAM_band)
 *  @brief: set a frequency in to related band and subband (write on Tuner device).
 *  @return: 1 if OK, 0 otherwise
*/
bool set_frequency(int p_frequency, char p_band, char p_subAM_band);


/*
 *  bool set_frequency(int p_frequency, char p_band, char p_subAM_band)
 *  @brief: get current frequency (read on Tuner device).
 *  @return: int current frequency
*/
int get_frequency();

#endif

