/***************************************************/
/*****   libDSP  			  	********************/
/*****   Akhela               **********************/
/***************************************************/

#include "libDSP.h"
#include "libdirana.h"
#include "dirana_utils.h"
#include "utils.h"


extern int g_current_volume;

/*
 *  bool initDSP(void)
 *  @brief: open device i2c to connect the DSP,
 *		execute initialization procedure of DSPs
 *  @return: 1 if OK, 0 otherwise
*/
bool initDSP(void)
{
	  bool l_res = OK;
	  l_res = initDirana();

	  return (OK == l_res);
}

/*
 *  bool dspEnd(void)
 *  @brief: close device i2c to disconnect the DSP
 *  @return: 1 if OK, 0 otherwise
*/
bool dspEnd()
{
	bool l_res = OK;
	l_res = close_device();

	return (OK == l_res);
}

/*
 *  bool setVolume(int p_volume )
 *  @brief: set volume between 0 and 30 unit.
 *  @return: 1 if OK, 0 otherwise
*/
bool setVolume(int p_volume)
{
	bool l_res = OK;
	l_res = setVolumeDirana(p_volume);
	return (OK == l_res);
}

/*
 *  bool getVolume(int *p_volume )
 *  @brief: get volume between 0 and 30 unit.
 *  @return: 1 if OK, 0 otherwise
*/
bool getVolume(int *p_volume)
{
	bool l_res = OK;
	l_res = getVolumeDirana(p_volume);
	return (OK == l_res);
}

/** ************************************************* **/
/**   API    : increase_Volume                               */
/**   RETURN : OK or ERROR                             */
/** ************************************************* **/
bool increase_Volume()
{
   bool l_res = OK;
   PrintMessage(RED_PRINT,"Volume  = %d",g_current_volume);
   g_current_volume++;
   if (g_current_volume>30){
   			   g_current_volume=30;
   			}
   l_res = setVolume(g_current_volume);
	PrintMessage(RED_PRINT,"Volume  = %d",g_current_volume);

   return (OK == l_res);

}

/** ************************************************* **/
/**   API    : decrease_Volume                         */
/**   RETURN : OK or ERROR                             */
/** ************************************************* **/
bool decrease_Volume()
{
	bool l_res = OK;
	g_current_volume--;
	PrintMessage(RED_PRINT,"Volume  = %d",g_current_volume);
	if (g_current_volume<0){
		   g_current_volume=0;
		}
	l_res = setVolume(g_current_volume);
	PrintMessage(RED_PRINT,"Volume  = %d",g_current_volume);
	 return (OK == l_res);
}

/*
 *  bool doMute(void)
 *  @brief: mute audio sources.
 *  @return: 1 if OK, 0 otherwise
*/
bool doMute(void)
{
	bool l_res = OK;
	l_res = doMuteDirana(1);
	return (OK == l_res);
}

/*
 *  bool doMute(void)
 *  @brief: demute audio sources.
 *  @return: 1 if OK, 0 otherwise
*/
bool doDeMute(void)
{
	bool l_res = OK;
	l_res = doMuteDirana(0);
	return (OK == l_res);
}

/*
 *  bool SetBalance(void)
 *  @brief: SetBalance audio value between  [-11/+11].
 *  @return: 1 if OK, 0 otherwise
*/
bool SetBalance( sint8 p_balance )
{
	bool l_res = OK;
	l_res = SetBalanceDirana(p_balance );
	return (OK == l_res);
}

/*
 *  bool SetFader(void)
 *  @brief: SetFader audio value between  [-11/+11].
 *  @return: 1 if OK, 0 otherwise
*/
bool SetFader( sint8 p_fader )
{
	bool l_res = OK;
		l_res = SetFaderDirana(p_fader);
		return (OK == l_res);
}

/*
 *  bool SetLoudness(void)
 *  @brief: enable or disable audio Loudness.
 *  @return: 1 if OK, 0 otherwise
*/
bool SetLoudness( uint8 p_loudness )
{
	bool l_res = OK;
		l_res =  SetLoudnessDirana(  p_loudness );
		return (OK == l_res);
}


/*
 *  bool setSource(AUDIO_SOURCES_TYPES p_source )
 *  @brief: select and connect a new audio source
 *  @return: 1 if OK, 0 otherwise
*/
bool setSource(AUDIO_SOURCES_TYPES p_source )
{
	bool l_res = OK;
	doMute();
	l_res = setSourceDirana(p_source);
	doDeMute();
	setVolume(17);
	return (OK == l_res);
}

/*
 *  bool getSource(int *p_source )
 *  @brief: get current audible audio source
 *  @return: 1 if OK, 0 otherwise
*/
bool getSource(AUDIO_SOURCES_TYPES *p_source)
{
	bool l_res = OK;
	l_res = getSourceDirana(p_source);
	return (OK == l_res);
}


/*
 *  bool playSin(int p_volume )
 *  @brief: play a sin with setted volume amplitude.
 *  @return: 1 if OK, 0 otherwise
*/
bool playSin(int p_volume,int p_frequency, bool p_enable)
{
	bool l_res = OK;
	l_res = playSinDirana(p_volume,p_frequency, p_enable);
	return (OK == l_res);
}


/*
 *  bool change_band(char p_new_band)
 *  @brief: change band (write on Tuner device).
 *  @return: 1 if OK, 0 otherwise
*/
bool change_band(char p_new_band)
{
	bool l_res = OK;
	l_res = change_bandDirana(p_new_band);
	return (OK == l_res);
}

/*
 *  bool get_band(char p_new_band)
 *  @brief: get current band (read on Tuner device).
 *  @return: 1 if OK, 0 otherwise
*/
void get_band(char p_band, char p_subband)
{
	get_bandDirana(p_band, p_subband);
}

/*
 *  bool set_frequency(int p_frequency, char p_band, char p_subAM_band)
 *  @brief: set a frequency in to related band and subband (write on Tuner device).
 *  @return: 1 if OK, 0 otherwise
*/
bool set_frequency(int p_frequency, char p_band, char p_subAM_band)
{
	bool l_res = OK;
	l_res =  set_frequencyDirana( p_frequency, p_band, p_subAM_band);
	return (OK == l_res);
}


/*
 *  bool set_frequency(int p_frequency, char p_band, char p_subAM_band)
 *  @brief: get current frequency (read on Tuner device).
 *  @return: int current frequency
*/
int get_frequency()
{
	return get_frequencyDirana();
}

