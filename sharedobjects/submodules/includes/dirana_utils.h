/******************************************************/
/*****   libDirana  Header file  **********************/
/*****   Akhela                  **********************/
/******************************************************/

#ifndef DSP_H
#define DSP_H

#include <stdbool.h>

#include "Radio_774x.h"
#include "ri_audio_7741.h"
#include "SDSP_7741.h"
//#include "ri_dsp_define.h"

#include "def_variables_DSP.h"


#define PATCH_REG_SIZE 4
#define X_REG_SIZE 3
#define Y_REG_SIZE 2
#define TUNER_REG_SIZE_WRITE 7
#define TUNER_REG_SIZE_READ 11
#define SAF7730ADDR (0x38>>1)

#define MAX_BUF 256
#define MAXROW_DIRANA_INIT_VECTOR 400


/* Hw control registers for dsps. */
#define DSPs_HW_CTR_REG            0x000010
/* Control register for Clocks and PLLs */
#define CLKPLL_CTR_REG             0x000020
/* I2s Control �1� register analog power-down  register */
#define APD_CTRL1                  0x000040

/*clock enable register*/
#define CLK_EN_REG				   0x000050

/* Connection register for the TDSP1 for RDS */
#define RDS_CONN_TDSP1_REG         0x010FF0

/* Miscellaneous control register for TDSP1E */
#define TDSP1E_MISC_CTR_REG        0x030ff9

/* RDS1 control register */
#define RDS1_CTR_REG               0x000035
/* RDS2 control register */
#define RDS2_CTR_REG               0x00003D


#define RDS1_SET_REG               0x000034
#define RDS2_SET_REG               0x00003C

/* Frequency mixer �0� register for PDC2 */
#define PDC2_FMIX_0_REG            0x050FF5
/* Frequency mixer �1� register for PDC2 */
#define PDC2_FMIX_1_REG            0x050FD8

/* Frequency mixer �0� register for PDC1 */
#define PDC1_FMIX_0_REG            0x010FF5
/* Frequency mixer �1� register for PDC1 */
#define PDC1_FMIX_1_REG            0x010FD8

/* I2s input output Control register */
#define IIC_IIS_IO_ConTRol         0x0D0FF6

/* BANDWIDTH DURING AFUPDATE  */
#define FCIB_1_MinBw_AFU           0x010327

#define TUNER_T1A_ADDRESS  0x62   /* main_tuner   ->   Lupin: 0xC4 */
#define TUNER_T1B_ADDRESS  0x60   /*  sub_tuner   ->   Lupin: 0xC0 */

#define ADSP_DIGSRC_SEL       0x0D0FF7
#define AD34_CTRL_REGISTER      0x0D0FF4  // controls the input selection of AD34
#define HOST_IIS_IO_ENABLED     0x0736DB  // Enable HOST IO AS SLAVE MODE
#define HOST_IIS_IO_DISABLED    0x06B6DB  // DISABLE HOST IO (DSP DEFAULT CFG)
#define DIRANA_DIG_2_AN_RATE 			0xFFFF42
#define DIRANA_AN_2_DIG_RATE 			0xFFFF42
#define DIRANA_MINUS_DIG_2_AN_RATE 		0x0000BE
#define DIRANA_MINUS_AN_2_DIG_RATE		0x0000BE
#define DIRANA_BLEND_GAIN_3DB           0x5A6
#define Radio_patch_1_DSP1e_addr		0x048000
#define Radio_patch_2_DSP2_addr			0x068000

#define ADSP_X_Router_1InPntr1                     0x0D0444
#define ADSP_X_Router_1InPntr2                     0x0D0445
#define ADSP_X_Router_2InPntr1                     0x0D0446
#define ADSP_X_Router_2InPntr2                     0x0D0447

#define ADSP_X_Router_1Out1_REL                    0x00044A
#define ADSP_X_Router_1Out2_REL                    0x00044B

#define ADSP_X_Router_2Out1_REL                    0x00044C
#define ADSP_X_PJumpRouter                         0x0D03C5

#define ADSP_MISC_IO_CTRL                          0x0D0FF1
#define APD_CTRL1_PLL_AUDIO_BIT_POWER_DOWN_HIGH    0x4A20
#define APD_CTRL1_DEFAULT_VALUE                    0x0A20

/* ADSP P memory */
#define ADSP_P_Router_main                         0x0E8000

#define PDC2_AGC_CTRL_0 						   0x050FF4
#define PDC1_AGC_CTRL_3							   0x010FF1
#define PDC1_AGC_CTRL_0                            0x010FF4
#define DAC_DELAY_REG                              0x0000C0

#define TDSP1_X_pFW_TunDly                         0x01003A


/** \def <tt>BALANCE_MIN</tt>Min balance value*/
#define BALANCE_MIN          -11
/** \def <tt>BALANCE_MAX</tt>Max balance value*/
#define BALANCE_MAX           11
/** \def <tt>BALANCE_DEFAULT</tt>Default balance value*/
#define BALANCE_DEFAULT        0


/** \def <tt>BALANCE_MIN</tt>Min balance value*/
#define BALANCE_MIN          -11
/** \def <tt>BALANCE_MAX</tt>Max balance value*/
#define BALANCE_MAX           11
/** \def <tt>BALANCE_DEFAULT</tt>Default balance value*/
#define BALANCE_DEFAULT        0


/** \def <tt>FADER_MIN</tt>Min fader value*/
#define FADER_MIN            -11
/** \def <tt>FADER_MAX</tt>Max fader value*/
#define FADER_MAX             11
/** \def <tt>FADER_DEFAULT</tt>Default fader value*/
#define FADER_DEFAULT          0

/** \def <tt>BASS_MIN</tt>Min bass value*/
#define BASS_MIN             -10
/** \def <tt>BASS_MAX</tt>Max bass value*/
#define BASS_MAX              10
/** \def <tt>BASS_DEFAULT</tt>Default bass value*/
#define BASS_DEFAULT           0

/** \def <tt>TREBLE_MIN</tt>Min treble value*/
#define TREBLE_MIN           -10
/** \def <tt>TREBLE_MAX</tt>Max treble value*/
#define TREBLE_MAX            10
/** \def <tt>TREBLE_DEFAULT</tt>Default treble value*/
#define TREBLE_DEFAULT         0         0


//#define LOUDNESS_ON 1
//#define LOUDNESS_OFF 0

// Loudness treble filter
#define LOUDNESS_T_BOOST_MAX (0x3A3) // 9dB
#define LOUDNESS_T_COEFF 5

// Loudness bass filter
#define LOUDNESS_B_BOOST_MAX (0x614)   // 17dB
#define LOUDNESS_B_COEFF 8


/** \def <tt>TREBLE_DEFAULT</tt>Number of treble table coefficients */
#define TREBLE_NB_COEFF 6
/** \def <tt>TREBLE_DEFAULT</tt>Number of volume table coefficients */
#define VOLUME_NB_STEP 80

#define LOUDNESS_STEPS 15
/**
 * Maximum attenuation used in volume table calculation
 */
#define MAX_DB_ATTENUATION 120

#define SUPERPOS_VOLUME_DB_STEPS 68

#define VOLUMEINDBTABLE  121

/**
 * Balance table
 */
extern const uint16 balance_table[BALANCE_MAX*2+1][2];

/**
 * Fader table
 */
extern const uint16 fader_table[BALANCE_MAX*2+1][2];

extern const uint16 bass_table[BASS_MAX*2+1];

extern const uint16 bass_coefficients_table[10];

extern const uint16 treble_table[TREBLE_MAX*2+1][TREBLE_NB_COEFF];

extern const uint16 Loudness_treble_filter[LOUDNESS_T_COEFF];

extern const uint16 Loudness_bass_filter[LOUDNESS_B_COEFF];

extern const uint16 superpos_volume_table[SUPERPOS_VOLUME_DB_STEPS];

extern const uint16 loudness_boost_table[VOLUME_NB_STEP + 1];

/**** for play sin***/
/*
 * K constant, 2.30 notation, K ~ 0.607252935
 */
#define CORDIC_1K 0x26DD3B6A // K constant , 2.30 notation

/*
 * pi/2 , 2.30 notation
 */
#define HALF_PI 0x6487ED51
/*
 * Number of iteration used in CORDIC algorithm
 */
#define CORDIC_NTAB 32 // 32 bit calculation
/*
 * 2*pi/44100, 2.30 notation, constant
 * Used in angle calculation (see DIRANA2 user manual)
 */
#define TWOPIdiv44100 0x25596

int open_device (char *p_devname);
int close_device ();

bool dspStart(int p_devnum);
bool initTuner();

bool dspWrite(unsigned int p_address,
                unsigned char p_reg_size,
				unsigned int p_value_to_write);
bool dspRead( unsigned int p_addr, 
                unsigned char p_reg_size,
				int *p_read_value );
bool dspTunerWrite(unsigned int p_tuner_address,
		unsigned char p_reg_size,
		unsigned char * p_value_to_write);
bool dspTunerRead(unsigned int p_tuner_address,
		unsigned char p_reg_size,
		unsigned char *p_value_read);

bool setSlvAddr(int p_addr);

bool LoadNXPPatch(unsigned int* p_vect_patch);
bool ExecuteDiranaInit_125( void );

//playsin
void DIRANA_get_waveform_registers(unsigned int p_frequency, unsigned short *p_hi, unsigned short *p_low);
int DIRANA_sinusoid_angle_2_30(unsigned int p_frequency);
int DIRANA_cordic_cosine(int p_theta);
void cordic32(int p_theta, int *p_sin, int *p_cos);
void DIRANA_2_30_to_9_23_notation_HiLo(int p_value, unsigned short *p_hi, unsigned short *p_low);

#endif //#ifndef DSP_H
