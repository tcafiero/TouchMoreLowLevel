/*
 * libdirana.c
 *
 *  Created on: 16/lug/2013
 *      Author: user
 */

#include "libDSP.h"
#include <stdio.h>

/* library APIs  declarations */
bool initDSP(void)
{
	printf("Function Activated: %s\n",__func__);
	return true;
}

bool dspEnd()
{
	printf("Function Activated: %s\n",__func__);
	return true;
}

bool increase_Volume()
{
	printf("Function Activated: %s\n",__func__);
	return true;
}

bool decrease_Volume()
{
	printf("Function Activated: %s\n",__func__);
	return true;
}

bool SetBalance( sint8 p_balance )
{
	printf("Function Activated: %s\n",__func__);
	return true;
}

bool SetFader( sint8 p_fader )
{
	printf("Function Activated: %s\n",__func__);
	return true;
}

bool SetLoudness( uint8 p_loudness )
{
	printf("Function Activated: %s\n",__func__);
	return true;
}

bool setSource(AUDIO_SOURCES_TYPES p_source )
{
	printf("Function Activated: %s\n",__func__);
	return true;
}


bool getSource(AUDIO_SOURCES_TYPES *p_source)
{
	printf("Function Activated: %s\n",__func__);
	return true;
}


bool setVolume(int p_volume)
{
	printf("Function Activated: %s\n",__func__);
	return true;
}


bool getVolume(int *p_volume)
{
	printf("Function Activated: %s\n",__func__);
	return true;
}


//STATUS getStatus(DR_DSP_STATUS *p_status);
bool setChannelGain(int p_channel, int p_gain)
{
	printf("Function Activated: %s\n",__func__);
	return true;
}

bool getChannelGain(int p_channel, int *p_gain)
{
	printf("Function Activated: %s\n",__func__);
	return true;
}

bool playSin(int p_volume,int p_frequency, bool p_enable)
{
	printf("Function Activated: %s\n",__func__);
	return true;
}

bool doMute(void)
{
	printf("Function Activated: %s\n",__func__);
	return true;
}

bool doDeMute(void)
{
	printf("Function Activated: %s\n",__func__);
	return true;
}


bool change_band(char p_new_band)
{
	printf("Function Activated: %s\n",__func__);
	return true;
}

void get_band(char p_band, char p_subband)
{
	printf("Function Activated: %s\n",__func__);
}

bool set_frequency(int p_frequency, char p_band, char p_subAM_band)
{
	printf("Function Activated: %s\n",__func__);
	return true;
}

int get_frequency()
{
	printf("Function Activated: %s\n",__func__);
	return true;
}


