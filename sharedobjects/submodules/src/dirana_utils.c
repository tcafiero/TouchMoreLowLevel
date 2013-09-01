///******************************************************///
///******************************************************///
///*************   libDirana  Source file   *************///
///*************   Akhela                   *************///
///******************************************************///
///******************************************************///

///**** System's includes ****///
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>

///**** Library's includes ****///
#include "i2c-dev.h"
#include "libdirana.h"
#include "dirana_utils.h"
#include "utils.h"

///* global statics *///
int g_I2C_hndl;


/* Buffer to allow a switch on secondary i2c bus */
static unsigned char g_tuner_SwitchOnSecBusBuf[] =
{
  0x00, 0xFF, 0xFF
};

#define SUPERPOS_VOLUME_DB_STEPS 68
// Superposition block volume array
const unsigned short superpos_volume_table[SUPERPOS_VOLUME_DB_STEPS] =
{
    0x000, // -inf
    0xFFE, //-66
    0xFFE, //-65
    0xFFE, //-64
    0xFFE, //-63
    0xFFE, //-62
    0xFFE, //-61
    0xFFD, //-60
    0xFFD, //-59
    0xFFD, //-58
    0xFFD, //-57
    0xFFC, //-56
    0xFFC, //-55
    0xFFB, //-54
    0xFFB, //-53
    0xFFA, //-52
    0xFFA, //-51
    0xFF9, //-50
    0xFF8, //-49
    0xFF7, //-48
    0xFF6, //-47
    0xFF5, //-46
    0xFF4, //-45
    0xFF3, //-44
    0xFF1, //-43
    0xFEF, //-42
    0xFED, //-41
    0xFEB, //-40
    0xFE9, //-39
    0xFE6, //-38
    0xFE3, //-37
    0xFDF, //-36
    0xFDB, //-35
    0xFD7, //-34
    0xFD2, //-33
    0xFCC, //-32
    0xFC6, //-31
    0xFBF, //-30
    0xFB7, //-29
    0xFAE, //-28
    0xFA4, //-27
    0xF99, //-26
    0xF8C, //-25
    0xF7E, //-24
    0xF6F, //-23
    0xF5D, //-22
    0xF49, //-21
    0xF33, //-20
    0xF1A, //-19
    0xEFE, //-18
    0xEDE, //-17
    0xEBB, //-16
    0xE93, //-15
    0xE67, //-14
    0xE35, //-13
    0xDFD, //-12
    0xDBE, //-11
    0xD78, //-10
    0xD29, //-9
    0xCD0, //-8
    0xC6D, //-7
    0xBFD, //-6
    0xB80, //-5
    0xAF3, //-4
    0xA56, //-3
    0x9A5, //-2
    0x8DE, //-1
    0x800, //0

};

const unsigned short bass_coefficients_table[10] =
{
  //a1H,  a1L,    a2H,    a2L,   b0H,    b0L,    b1H,    b1L,    b2H,    b2L
  /*first order shelving *///0x03EE, 0x051F, 0x000, 0x0000, 0x0008, 0x0570, 0x0008, 0x0570, 0x0000, 0x0000
  /*second order she lving*/ //0x7D5, 0x450, 0xC29, 0x706,0x000, 0x12A, 0x000, 0x255, 0x000, 0x12A
  0x3EE, 0x51F, 0x000,0x000, 0x008, 0x570, 0x008, 0x570, 0x000, 0x000
};

#define NXP_PATCH_ROUTER_SIZE  24	// 21 byte + 3 byte di indirizzo;
#define NXP_PATCH_RADIO_1_SIZE 111 // 108 byte + 3 byte di indirizzo;
#define NXP_PATCH_RADIO_2_SIZE   29// 26 byte + 3 byte di indirizzo;

const unsigned int NXPPatch_Router[NXP_PATCH_ROUTER_SIZE] =
{
	0x0E8000, /* address of the ADSP*/
	0x50000125,
	0x4200044A,
	0x7880F444,
	0x5E006000,
	0x2F406000,
	0x7880F445,
	0x5E006000,
	0x2F406000,
	0x7880F446,
	0x5E006000,
	0x2F406000,
	0x7880F447,
	0x5E006000,
	0x2F406000,
	0x7880F448,
	0x5E006000,
	0x2F406000,
	0x7880F449,
	0x7FD00000,
	0x5E406000,
	0x2F406000
};
const unsigned int NXPPatch_Radio_1[NXP_PATCH_RADIO_1_SIZE] =
{
	0x048000,  /* address of the DSP1e*/
	0x5C00F000,
	0x68000008,
	0x14005010,
	0x1400A050,
	0x1400F000,
	0x5C00F001,
	0x68FF00FF,
	0x14005010,
	0x14009050,
	0x68000400,
	0x1400A050,
	0x1400F001,
	0x40000101,
	0x4800801A,
	0x410003AF,
	0x74100028,
	0x410003B1,
	0x74120018,
	0x741207E8,
	0x400006EF,
	0x49100000,
	0x400000FF,
	0x48008034,
	0x7FFC0102,
	0x7C000000,
	0x7C000000,
	0x7FFC0102,
	0x6C10F684,
	0x3810F0C6,
	0x41000026,
	0x34106000,
	0x410003B0,
	0x34106400,
	0x410003AF,
	0x34106800,
	0x14005050,
	0x14003000,
	0x7E70802E,
	0x7C000000,
	0x7C000000,
	0x14001090,
	0x14003000,
	0x7E785410,
	0x7FF08031,
	0x7C000000,
	0x7C000000,
	0x14003090,
	0x14003000,
	0x7C185410,
	0x7FFC0009,
	0x2600F3B0,
	0x2600F026,
	0x5C00F018,
	0x68000007,
	0x14005010,
	0x14009050,
	0x68000001,
	0x14003040,
	0x7C608057,
	0x5C00F021,
	0x400006EC,
	0x5E006C00,
	0x7C00001E,
	0x14005010,
	0x164030D0,
	0x08006000,
	0x7C00001D,
	0x410003B2,
	0x5E166400,
	0x14146128,
	0x14004038,
	0x14000010,
	0x7C000020,
	0x1400F0B1,
	0x1400F6EE,
	0x16406000,
	0x7C00001F,
	0x5C00F6EF,
	0x5C03F1B8,
	0x14006320,
	0x14004010,
	0x14003000,
	0x7E787410,
	0x7E8854D0,
	0x1400F6F0,
	0x1400F1B8,
	0x7FF0805E,
	0x5C00F018,
	0x68000007,
	0x14005010,
	0x14009050,
	0x68000003,
	0x14003040,
	0x7C608069,
	0xF400001D,
	0x140050D0,
	0x040B6000,
	0x68F33333,
	0x14005050,
	0x040C6000,
	0xF400005D,
	0x140050D0,
	0x040B6000,
	0x14005050,
	0x040C6000,
	0x7FFC0100,
	0x7C000000,
	0x7C000000
};

const unsigned int NXPPatch_Radio_2[NXP_PATCH_RADIO_2_SIZE] =
{
	0x068000,  /* address of the DSP2*/
	0x5C00F000,
	0x68000008,
	0x14005010,
	0x1400A050,
	0x1400F000,
	0x5C00F001,
	0x68FF00FF,
	0x14005010,
	0x14009050,
	0x68000400,
	0x1400A050,
	0x1400F001,
	0x7FFC0181,
	0x40000180,
	0x48008014,
	0x40000199,
	0x48001E3D,
	0x7FFC0199,
	0x40000198,
	0x48008017,
	0x7FFC0181,
	0x6C10F584,
	0x3810F149,
	0x7FFC0199,
	0x6C10F584,
	0x3810F149
};


unsigned int DiranaInit_125_Vector[400][3] =
{   /*START DIRANA INIT VECTOR*/
    { ADSP_P_Router_main, NXP_PATCH_ROUTER_SIZE, (unsigned int)NXPPatch_Router },   /*Load NXP Router patch*/
    { DSPs_HW_CTR_REG /*0x000010*/, X_REG_SIZE, 0x001249 },             /*Reset all DSPs*/
	{ 0x0, 0x0, 0x0 },                          	/*delay 2mS*/
	{ 0x0, 0x0, 0x0 },                          	/*delay 2mS*/
	{ 0x0, 0x0, 0x0 },                          	/*delay 2mS*/
	{ 0x0, 0x0, 0x0 },                          	/*delay 2mS*/
	{ 0x0, 0x0, 0x0 },                          	/*delay 2mS*/
    { CLKPLL_CTR_REG /*0x000020*/, X_REG_SIZE, 0x000CC6 },             /* set DSP speed to 135.2MHz */
    { 0x40000, 4, 0x7C000000 },
    { Radio_patch_1_DSP1e_addr, NXP_PATCH_RADIO_1_SIZE, (unsigned int)NXPPatch_Radio_1 },  /*Load NXP Radio patch 1*/
    { 0x60000, 4, 0x7C000000 },
    { Radio_patch_2_DSP2_addr, NXP_PATCH_RADIO_2_SIZE, (unsigned int)NXPPatch_Radio_2 },   /*Load NXP Radio patch 2*/

    { APD_CTRL1 /*0x000040*/, Y_REG_SIZE, 0x3A20 },    /* Disable RGPDAC1, RGPDAC2 (Radio General Purpose DAC)*/
    { CLK_EN_REG /*0x000050*/, Y_REG_SIZE, 0x0FFF },    /* (old 0x07ff) Enable PDC1 Clock (Primary Decimation Chain)*/
    { 0x050FD4, X_REG_SIZE,0x070240 },     /* PDC2 default changed (Primary Decimation Chain)*/
    { 0x050FCA, X_REG_SIZE,0xEA7DBF },    /* PDC2 AGC gain default changed (Primary Decimation Chain)  - step8*/
    { 0x050FC0, X_REG_SIZE,0x0FF000 },    /* Write PDC2 AGC gain default, required to activate the AGC_GAIN_1_2 setting*/

    { 0x010FD4, X_REG_SIZE,0x070240 },    /* PDC1 default changed */
    { 0x010FCA, X_REG_SIZE,0xEA7DBF },   /* PDC1 AGC gain default changed */
    { 0x010FC0, X_REG_SIZE,0x0FF000 },    /* Write PDC1 AGC gain default, required to activate the AGC_GAIN_1_2 setting */
    { DSPs_HW_CTR_REG /*0x000010*/, X_REG_SIZE, 0x001248 },     /*Release TDSP1*/
    { TDSP1_X_FW_KeyCod, X_REG_SIZE,0x339500 },  /* Key code for Phase diversity true for DIRANA2 */
    { TDSP1_X_FW_KeyCod, X_REG_SIZE, 0x000001  },    /*Kick-Off Radio program*/
    { DSPs_HW_CTR_REG /*0x000010*/, X_REG_SIZE, 0x001208 },    /*Release TDSP1E*/
    { DSPs_HW_CTR_REG /*0x000010*/, X_REG_SIZE, 0x000000 },    /*Release TDSP2, SDSP and ADSP*/
    { 0x0, 0x0, 0x0 },                          /*delay 2mS*/
    { 0x0, 0x0, 0x0 },                          /*delay 2mS*/

    { DSPs_HW_CTR_REG /*0x000010*/, X_REG_SIZE, 0x002000 },                /*Enable dynamic PRAM for NXP Router patch*/
    { 0x0, 0x0, 0x0 },                          /*delay 2mS*/
	{ 0x0, 0x0, 0x0 },                          /*delay 2mS*/
    { IIC_IIS_IO_ConTRol /*0x0D0FF6*/, X_REG_SIZE, 0x06B6DB },
    { ADSP_MISC_IO_CTRL /*0x0D0FF1*/, X_REG_SIZE, 0x000000 },

    { ADSP_X_KeyCode , X_REG_SIZE, 0x8F0CB7 },   	/* OLD keyCode*/
    { ADSP_X_KeyCode , X_REG_SIZE, 0x7DA232 },   	/* NEW keyCode*/
    { ADSP_X_KickOffADSP ,X_REG_SIZE, 0x000001 },   /* Kick-Off ADSP*/

    { SDSP_X_CtrlTS1, X_REG_SIZE, 0x800000 },    /*Enable SRC1*/
    { SDSP_X_CtrlTS2, X_REG_SIZE, 0x000000 },    /*Enable SRC2*/
    { SDSP_X_CtrlTS3, X_REG_SIZE, 0x000000 },    /*Disable SRC3*/
    { SDSP_X_CtrlTS4, X_REG_SIZE, 0x000000 },    /*Disable SRC4*/
    { SDSP_X_CtrlTS5, X_REG_SIZE, 0x000000 },    /*Disable SRC5*/

    { ADSP_X_SProcSelPntr, X_REG_SIZE, ADSP_X_DCfilt_OutSL_REL },
    { ADSP_X_SChanSelPntr, X_REG_SIZE, ADSP_X_SRC1InL_REL },
    { ADSP_X_Loudf_InPntr, X_REG_SIZE, ADSP_X_Vol_OutScalSL_REL },
    { ADSP_X_SVolInPntr, X_REG_SIZE, ADSP_X_Loudf_OutL_REL },
    { ADSP_X_GC1InPntr, X_REG_SIZE, ADSP_X_Vol_OutSL_REL },
    { ADSP_X_GC2InPntr, X_REG_SIZE, ADSP_X_Vol_OutSL_REL },
    { ADSP_X_FSupInPntr, X_REG_SIZE, ADSP_X_GC1outL_REL },
    { ADSP_X_RSupInPntr, X_REG_SIZE, ADSP_X_GC2outL_REL },
    { ADSP_X_EasyP_Index, X_REG_SIZE, ADSP_EASYP_SupPos_PDC2Enable },
    { ADSP_X_FInPntr, X_REG_SIZE, ADSP_X_FrontOutL_REL },
    { ADSP_X_RInPntr, X_REG_SIZE, ADSP_X_RearOutL_REL },
    { ADSP_X_HIIS3Outpntr, X_REG_SIZE, ADSP_X_FrontOutL_REL },

    { ADSP_X_CompExp_switch, X_REG_SIZE, 0x800000 },   	/*Disable compressor expander*/
    { ADSP_Y_CompExp_ExpL, Y_REG_SIZE, 0x0000 },   		/*Disable compressor expander*/
    { ADSP_X_GPF6_InPntr, X_REG_SIZE, ADSP_X_Eq_OutFL_REL },
    { ADSP_X_GPF7_InPntr, X_REG_SIZE, ADSP_X_Eq_OutFR_REL },
    { ADSP_X_GPF4_InPntr, X_REG_SIZE, ADSP_X_Eq_OutRL_REL },
    { ADSP_X_GPF5_InPntr, X_REG_SIZE, ADSP_X_Eq_OutRR_REL },
    { ADSP_X_Router_2InPntr1, X_REG_SIZE, ADSP_X_GPF6_OutL_REL },
    { ADSP_X_Router_2InPntr2, X_REG_SIZE, ADSP_X_GPF7_OutL_REL },
    { ADSP_X_EasyP_Index, X_REG_SIZE, ADSP_EASYP_GPF67_Enable },
    { ADSP_X_Delay_F_InPntr, X_REG_SIZE, ADSP_X_Router_2Out1_REL },

    { ADSP_X_Delay_R_InPntr, X_REG_SIZE, ADSP_X_GPF4_OutL_REL },

    { ADSP_X_PJumpRouter, X_REG_SIZE, ADSP_P_Router_main },

    { ADSP_X_EasyP_Index, X_REG_SIZE, ADSP_EASYP_Limit_LimitRSOnSec },

    { ADSP_X_SwSupInPntr, X_REG_SIZE, ADSP_X_Vol_OutFL_REL },
    { ADSP_X_SSupInPntr, X_REG_SIZE, ADSP_X_Vol_OutRL_REL },
    { ADSP_X_FDACpntr, X_REG_SIZE, ADSP_X_SwOutL_REL },
    { ADSP_X_RDACpntr, X_REG_SIZE, ADSP_X_SecOutL_REL },
    { ADSP_X_SWDACpntr, X_REG_SIZE, ADSP_X_Vol_OutSwL_REL },

    { ADSP_X_SCD_SWFlags, X_REG_SIZE, 0x000007 },
    { ADSP_X_FCLim_FlgGenMaskFR, X_REG_SIZE, 0x000001 },
    { ADSP_X_SCD_InPntr00, X_REG_SIZE, ADSP_X_FrontOutL_REL },
    { ADSP_X_SCD_InPntr01, X_REG_SIZE, ADSP_X_RearOutL_REL },
    { ADSP_X_FCLim_FlgGenMaskSwL, X_REG_SIZE, 0x000002 },
    { ADSP_X_SCD_InPntr1, X_REG_SIZE, ADSP_X_MkSubwLeft_REL },
   	{ ADSP_X_FCLim_FlgGenMaskSwR, X_REG_SIZE, 0x000004 },
   	{ ADSP_X_SCD_InPntr2, X_REG_SIZE, ADSP_X_MkSubwRight_REL },

    { ADSP_X_SwFInPntr, X_REG_SIZE, 0x000000 },
	{ ADSP_X_SwRInPntr, X_REG_SIZE, 0x000000 },

    { ADSP_X_Sup_ExtInPntr, X_REG_SIZE, 0x000000 },
	{ ADSP_X_Sup_ChiInPntr, X_REG_SIZE, 0x000000 },
 //TODO: da modificare per poter sentire audio su i canali Sw e Center (0x7ff è il max 0db);
	{ ADSP_Y_Sup_ChionSW, Y_REG_SIZE, 0x0300 },
	{ ADSP_Y_Sup_ChionSwR, Y_REG_SIZE, 0x0300 },
	{ ADSP_Y_Sup_ExtonSW, Y_REG_SIZE, 0x0300 },
	{ ADSP_Y_Sup_ExtonSwR, Y_REG_SIZE, 0x0300 },
	{ ADSP_Y_Sup_ChionSL, Y_REG_SIZE, 0x0300 },
	{ ADSP_Y_Sup_ChionSR, Y_REG_SIZE, 0x0300 },
	{ ADSP_Y_Sup_ExtonSL, Y_REG_SIZE, 0x0300 },
	{ ADSP_Y_Sup_ExtonSR, Y_REG_SIZE, 0x0300 },

	{ ADSP_Y_Vol_SrcScalN, Y_REG_SIZE, 0x7FF },
	{ ADSP_Y_Vol_SrcScalT, Y_REG_SIZE, 0x7FF },

	{ ADSP_X_EasyP_Index, X_REG_SIZE,ADSP_EASYP_PEQ_Center_IIR4_headroom },
    { ADSP_X_EasyP_Index, X_REG_SIZE, ADSP_EASYP_PEQ_Sw_IIR4_headroom },
    { ADSP_X_GPF_LInPntr, X_REG_SIZE,ADSP_X_Eq_OutSwL_REL },
    { ADSP_X_GPF_RInPntr, X_REG_SIZE,ADSP_X_GPF_OutL_REL },
    { ADSP_X_GPF3_LInPntr, X_REG_SIZE,ADSP_X_GPF_OutR_REL },
    { ADSP_X_GPF3_RInPntr, X_REG_SIZE,ADSP_X_GPF3_OutL_REL },
    { ADSP_X_GPF2_InPntr, X_REG_SIZE,ADSP_X_GPF3_OutR_REL },
    { ADSP_X_NChanSelPntr, X_REG_SIZE,ADSP_X_GPF2_OutL_REL },
    { ADSP_X_Delay_C_InPntr, X_REG_SIZE,ADSP_X_Navb4EQ_REL },
    { ADSP_X_GEqInPntr, X_REG_SIZE,ADSP_X_Eq_OutSwR_REL },
    { ADSP_X_TChanSelPntr, X_REG_SIZE,ADSP_X_GEq_OutL_REL },
    { ADSP_X_Delay_Sw_InPntr, X_REG_SIZE,ADSP_X_Phonb4EQ_REL },
    { ADSP_X_EasyP_Index, X_REG_SIZE,ADSP_EASYP_GPF2_headroom },
    { ADSP_X_EasyP_Index, X_REG_SIZE,ADSP_EASYP_GPF3_headroom },
    { ADSP_X_EasyP_Index, X_REG_SIZE,ADSP_EASYP_GPF4_headroom },
    { ADSP_X_EasyP_Index, X_REG_SIZE,ADSP_EASYP_GPChannels_Enable },
/* Subwoofer path definition:										*/
/* Configure subwoofer left and right as stereo subwoofers			*/
/* (left = (FL + RL )/2, right = (FR +  RR)/2)						*/

    { ADSP_Y_Vol_FLonSwL, Y_REG_SIZE, 0x400 },
    { ADSP_Y_Vol_FRonSwL, Y_REG_SIZE, 0x000 },
    { ADSP_Y_Vol_RLonSwL, Y_REG_SIZE, 0x400 },
    { ADSP_Y_Vol_RRonSwL, Y_REG_SIZE, 0x000 },
    { ADSP_Y_Vol_FLonSwR, Y_REG_SIZE, 0x000 },
    { ADSP_Y_Vol_FRonSwR, Y_REG_SIZE, 0x400 },
    { ADSP_Y_Vol_RLonSwR, Y_REG_SIZE, 0x000 },
    { ADSP_Y_Vol_RRonSwR, Y_REG_SIZE, 0x400 },

/* Activation additional 4 Filter of GraphEQ (EQ4_xxxx)				*/
    { ADSP_X_EasyP_Index, X_REG_SIZE, ADSP_EASYP_GraphEQ_additional4Band },

    //FIXME

    { AD34_CTRL_REGISTER, X_REG_SIZE, 0x006DD2 },
    { ADSP_X_GC3InPntr, X_REG_SIZE, ADSP_X_AD34InL_REL },
    { ADSP_X_GC4InPntr, X_REG_SIZE, ADSP_X_AD34InR_REL },
    { ADSP_X_Router_1InPntr1, X_REG_SIZE, ADSP_X_GC3outL_REL },
    { ADSP_X_Router_1InPntr2, X_REG_SIZE, ADSP_X_GC4outL_REL },
    { ADSP_X_HIIS1Outpntr, X_REG_SIZE, ADSP_X_Router_1Out1_REL },
    { ADSP_Y_Mute_GC3, Y_REG_SIZE, 0x0000 },
    { ADSP_Y_Mute_GC4, Y_REG_SIZE, 0x0000 },
    { ADSP_Y_Mute_T, Y_REG_SIZE, 0x000 },
    { ADSP_Y_Mute_N, Y_REG_SIZE, 0x000 },
    { ADSP_X_HIIS2Outpntr, X_REG_SIZE, ADSP_X_SRC3InL_REL},
    { ADSP_Y_Mute_P, Y_REG_SIZE,0x0800 },
    { ADSP_Y_Mute_S , Y_REG_SIZE,0x0800 },
    { ADSP_Y_Vol_Main2P, Y_REG_SIZE,0x0080 },
    { ADSP_Y_Vol_Main1S, Y_REG_SIZE,0x0800 },
    { ADSP_Y_Vol_Main2S, Y_REG_SIZE,0x0080 },
    { ADSP_Y_Mute_GC1, Y_REG_SIZE,0x0800 },
    { ADSP_Y_Mute_GC2, Y_REG_SIZE,0x0800 },
    { ADSP_X_EasyP_Index, X_REG_SIZE,ADSP_EASYP_SrcSw_SPDIF1onB },   // da verificare CE
    { ADSP_Y_IIR2GPF2_b0H, Y_REG_SIZE,0x400 },
    { ADSP_Y_IIR2GPF3L_b0H, Y_REG_SIZE,0x400 },
    { ADSP_Y_IIR2GPF3R_b0H, Y_REG_SIZE,0x400 },
    { ADSP_Y_EqN_b00H, Y_REG_SIZE,0x400 },
    { ADSP_Y_EqN_b10H, Y_REG_SIZE,0x400 },
    { ADSP_Y_EqT_b00H, Y_REG_SIZE,0x400 },
    { ADSP_Y_EqT_b10H, Y_REG_SIZE,0x400 },
    { ADSP_Y_Vol_UpScalF, Y_REG_SIZE,0x0F00 },
    { ADSP_Y_Vol_UpScalR, Y_REG_SIZE,0x0F00 },
    { ADSP_Y_Vol_UpScalSwL, Y_REG_SIZE,0x0400 },
    { ADSP_Y_Vol_UpScalSwR, Y_REG_SIZE,0x0400 },

    { PDC2_AGC_CTRL_0/*0x050FF4*/, X_REG_SIZE, 0x704317 },    // PDC2_AGC_CTRL_0
    { PDC1_AGC_CTRL_3 /*0x010FF1*/, X_REG_SIZE, 0x479C00 },   // PDC1_AGC_CTRL_3
    { PDC1_AGC_CTRL_0 /*0x010FF4*/, X_REG_SIZE, 0x704317 },    // PDC1_AGC_CTRL_0
    { DAC_DELAY_REG /*0x0000C0*/, Y_REG_SIZE, 0x88 },    /* DAC_DELAY  */

    { TDSP1E_Y_IntVal5 /*0x0311B6*/, Y_REG_SIZE, 0x07F5 },		/* TDSP1E_Y_IntVal5 */
	{ TDSP1E_Y_IntVal6 /*0x0311B7*/, Y_REG_SIZE, 0x07CF },     	/* TDSP1E_Y_IntVal6 */
	{ TDSP1E_X_IntVal7 /*0x030189*/, X_REG_SIZE, 0x0621A5 },    	/* TDSP1E_Y_IntVal7 */
	{ TDSP1E_X_IntVal8/*0x03018A*/, X_REG_SIZE, 0x1F47D0 },   	/* TDSP1E_Y_IntVal8 */

    { 0x01003A, X_REG_SIZE, 0x007080 },   /* register TDSP1_X_pFW_TunDly */

    { TDSP1E_X_ACIS_1_pIFIntDly, X_REG_SIZE, 0x028080 },    /* Corrected AM noise blankler value */
    { TDSP1_X_FW_RadMod, X_REG_SIZE, 0x000352 },    /* Set Radio mode to FM -step27*/
    { ADSP_X_EasyP_Index /*0x0D006F*/, 3, 0x000013 },    /* 0x0D006A Main FM to primary channel N115 -step28 (LUPIN)*/

    { ADSP_Y_Sup_NonFL, Y_REG_SIZE, 0x0000 },
    { ADSP_Y_Sup_NonFR, Y_REG_SIZE, 0x0000 },
    { ADSP_Y_Sup_NonRL, Y_REG_SIZE, 0x0000 },
    { ADSP_Y_Sup_NonRR, Y_REG_SIZE, 0x0000 },
    { ADSP_Y_Sup_TonFL, Y_REG_SIZE, 0x0000 },
    { ADSP_Y_Sup_TonFR, Y_REG_SIZE, 0x0000 },
    { ADSP_Y_Sup_TonRL, Y_REG_SIZE, 0x0000 },
    { ADSP_Y_Sup_TonRR, Y_REG_SIZE, 0x0000 },
    { ADSP_Y_Sup_PDC2onFL, Y_REG_SIZE, 0x0000 },
    { ADSP_Y_Sup_PDC2onFR, Y_REG_SIZE, 0x0000 },
    { ADSP_Y_Sup_PDC2onRL, Y_REG_SIZE, 0x0000 },
    { ADSP_Y_Sup_PDC2onRR, Y_REG_SIZE, 0x0000 },
    { 0x0D03A4, X_REG_SIZE, 0x001828 },											/*enable pdc scaler*/
    { ADSP_X_Sup_NInPntr, X_REG_SIZE, ADSP_X_Vol_SrcScalOutFL_REL },			/*PDC*/

    { ADSP_X_SrcScal6_F_InPntr, X_REG_SIZE, 0x000000},
    { CLKPLL_CTR_REG, X_REG_SIZE, 0x200CC6 },
    { IIC_IIS_IO_ConTRol, X_REG_SIZE, HOST_IIS_IO_ENABLED },

    { APD_CTRL1, Y_REG_SIZE, APD_CTRL1_PLL_AUDIO_BIT_POWER_DOWN_HIGH },    /* enable the power down */
    { APD_CTRL1, Y_REG_SIZE, APD_CTRL1_DEFAULT_VALUE },                    /* disable the power-down of the Audio PLL */

    /* Start RDS management */
    { RDS1_SET_REG/*0x000034*/, Y_REG_SIZE, 0x5020 },   /* RDS1_SET_REG not defined */
    { RDS2_SET_REG/*0x00003C*/, Y_REG_SIZE, 0x5020 },   /* RDS2_SET_REG not defined */
    { TDSP1E_MISC_CTR_REG, X_REG_SIZE, 0x0318DB },
    { RDS_CONN_TDSP1_REG, Y_REG_SIZE, 0x0001 },
    /* End RDS management */

    /* Fixed band width setting */
    { TDSP1_X_ACIB_1_FirCtlFix, X_REG_SIZE, 0x000005 },
    /* End Fixed band width setting */
    { 0x0, 0x0, 0x0 },                          /*delay 2mS*/
    { 0x0, 0x0, 0x0 },                          /*delay 2mS*/
    { 0x0, 0x0, 0x0 },                          /*delay 2mS*/
    { 0x0, 0x0, 0x0 },                          /*delay 2mS*/
    { 0x0, 0x0, 0x0 },                          /*delay 2mS*/
    { 0x0, 0x0, 0x0 },                          /*delay 2mS*/
 /*add-on inizializzazione dirana*/
//softmute
    { TDSP1E_Y_FCVR_1_LvlRanSlp, Y_REG_SIZE, 0x0F02 },
    { TDSP1E_Y_FCVR_1_LvlRanOfs, Y_REG_SIZE, 0x0039 },
    { TDSP1E_Y_FCVR_1_LvlRanMax, Y_REG_SIZE, 0x04CD },

//softmute
    { TDSP1E_Y_FCVR_1_LvlGainFast, Y_REG_SIZE, 0x005A },
    { TDSP1E_Y_FCVR_1_LvlGainSlow, Y_REG_SIZE, 0x0066 },
    { TDSP1E_Y_FCVR_1_LvlRtaFast, Y_REG_SIZE, 0x0013 },
    { TDSP1E_Y_FCVR_1_LvlRtaSlow, Y_REG_SIZE, 0x0013 },
    { TDSP1E_X_FCVR_1_LvlRtdFast, X_REG_SIZE, 0xFF7EF2 },
    { TDSP1E_X_FCVR_1_LvlRtdSlow, X_REG_SIZE, 0xFF7EF2 },

//stereo blend
//level
    { TDSP1E_Y_FCSB_1_LvlRanSlp, Y_REG_SIZE, 0x0E5B },
    { TDSP1E_Y_FCSB_1_LvlRanOfs, Y_REG_SIZE, 0x00FD },
    { TDSP1E_Y_FCSB_1_LvlRtaSlow, Y_REG_SIZE, 0x0012 },
    { TDSP1E_X_FCSB_1_LvlRtdSlow, X_REG_SIZE, 0xFF7EF2 },

//Multipath
    { TDSP1E_Y_FCSB_1_MltRanSlp, Y_REG_SIZE, 0x0725 },
    { TDSP1E_Y_FCSB_1_MltRanOfs, Y_REG_SIZE, 0x0D12 },
    { TDSP1E_Y_FCSB_1_MltGainFast, Y_REG_SIZE, 0x0080 },
    { TDSP1E_Y_FCSB_1_MltRtaFast, Y_REG_SIZE, 0x0013 },
    { TDSP1E_X_FCSB_1_MltRtdFast, X_REG_SIZE, 0xFF7EF2 },
    { TDSP1E_Y_FCSB_1_MltGainSlow, Y_REG_SIZE, 0x0080 },
    { TDSP1E_Y_FCSB_1_MltRtaSlow, Y_REG_SIZE, 0x0013 },
    { TDSP1E_X_FCSB_1_MltRtdSlow, X_REG_SIZE, 0xFF7EF2 },

//High Cut
//level
    { TDSP1E_Y_FCAB_1_LvlRanSlp, Y_REG_SIZE, 0x0D5A },
    { TDSP1E_Y_FCAB_1_LvlRanOfs, Y_REG_SIZE, 0x00B6 },

//Multipath
    { TDSP1E_Y_FCAB_1_MltRanSlp, Y_REG_SIZE, 0x0200 },
    { TDSP1E_Y_FCAB_1_MltRanOfs, Y_REG_SIZE, 0x0F80 },
    { TDSP1E_Y_FCAB_1_MltGainFast, Y_REG_SIZE, 0x0080 },
    { TDSP1E_Y_FCAB_1_MltRtaFast, Y_REG_SIZE, 0x005B },
    { TDSP1E_X_FCAB_1_MltRtdFast, X_REG_SIZE, 0xFF28E8 },
    { TDSP1E_Y_FCAB_1_MltGainSlow, Y_REG_SIZE, 0x0033 },
    { TDSP1E_Y_FCAB_1_MltRtaSlow, Y_REG_SIZE, 0x0005 },
    { TDSP1E_X_FCAB_1_MltRtdSlow, X_REG_SIZE, 0xFFFEB6 },

//CutOFF
    { TDSP1E_Y_FDFR_1_LimFac, Y_REG_SIZE, 0x05FA },

//FM Noise Blanker
    { TDSP1_Y_FMID_1_MPXQpdCofRti, Y_REG_SIZE, 0X7Fc },
    { TDSP1_Y_FMID_1_MPXQpdCofRtd, Y_REG_SIZE, 0x002 },
    { TDSP1_Y_FMID_1_NseThrGn, Y_REG_SIZE, 0x199 },
    { TDSP1_X_FMID_1_MPXThr, X_REG_SIZE, 0x28f5c },
    { TDSP1_Y_FMID_1_RanSlp, Y_REG_SIZE, 0x3d },
    { TDSP1_X_FIIS_1_pMPXDlya, X_REG_SIZE, 0x04280 },
    { TDSP1_X_FMID_1_MPXHldStr, X_REG_SIZE, 0x000014 },

//FM Audio Filter
    { TDSP1E_Y_FDFR_1_Mabb0, Y_REG_SIZE, 0x0592 },
    { TDSP1E_Y_FDFR_1_Mabb1, Y_REG_SIZE, 0x0592 },
    { TDSP1E_Y_FDFR_1_Maba1, Y_REG_SIZE, 0x0CDC },

 //High Blend Disable
    { TDSP1E_Y_FCSN_1_LvlRanSlp, Y_REG_SIZE, 0x0000 },
    { TDSP1E_Y_FCSN_1_LvlRanOfs, Y_REG_SIZE, 0x0000 },
    { TDSP1E_Y_FCSN_1_UsnRanSlp, Y_REG_SIZE, 0x0000 },
    { TDSP1E_Y_FCSN_1_UsnRanOfs, Y_REG_SIZE, 0x0000 },
    { TDSP1E_Y_FCSN_1_ModRanSlp, Y_REG_SIZE, 0x0000 },
    { TDSP1E_Y_FCSN_1_ModRanOfs, Y_REG_SIZE, 0x0000 },
    { TDSP1E_Y_FCSN_1_PilRanSlp, Y_REG_SIZE, 0x0000 },
    { TDSP1E_Y_FCSN_1_PilRanOfs, Y_REG_SIZE, 0x07FF },

//FM click noise suppress
    { TDSP1_Y_FW_Clicknoise_Enable, Y_REG_SIZE, 0x0000 },
    { TDSP1_Y_FMIC_1_SupThr, Y_REG_SIZE, 0x0100 },

//softmute
    { TDSP1E_Y_ACVR_1_LvlRanSlp, Y_REG_SIZE, 0x0157 },
    { TDSP1E_Y_ACVR_1_LvlRanOfs, Y_REG_SIZE, 0x0057 },
    { TDSP1E_Y_ACVR_1_LvlRanMin, Y_REG_SIZE, 0x0500 },

//SoftMute
    { TDSP1E_X_ACVR_1_TimDec, X_REG_SIZE, 0x00810E },

//HighCut disabled
    { TDSP1E_Y_ACAB_1_LvlRanSlp, Y_REG_SIZE, 0x0000 },
    { TDSP1E_Y_ACAB_1_LvlRanOfs, Y_REG_SIZE, 0x07FF },

//High-pass (low cut) filter 170KHz
    { TDSP1E_Y_ADFR_1_Alfg1, Y_REG_SIZE, 0x07EE },
    { TDSP1E_Y_ADFR_1_Alfb11, Y_REG_SIZE, 0x0800 },
    { TDSP1E_Y_ADFR_1_Alfa11, Y_REG_SIZE, 0x07DC },
    { TDSP1E_Y_ADFR_1_Alfg2, Y_REG_SIZE, 0x07EE },
    { TDSP1E_Y_ADFR_1_Alfb12, Y_REG_SIZE, 0x0800 },
    { TDSP1E_Y_ADFR_1_Alfa12, Y_REG_SIZE, 0x07DC },

//High-pass cutoff frequency 3KHz
    { TDSP1E_Y_ADFR_1_Mabb2, Y_REG_SIZE, 0x0053 },
    { TDSP1E_Y_ADFR_1_Maba2, Y_REG_SIZE, 0x0D88 },
    { TDSP1E_Y_ADFR_1_Mabb0, Y_REG_SIZE, 0x0053 },
    { TDSP1E_Y_ADFR_1_Mabb1, Y_REG_SIZE, 0x0FF2 },
    { TDSP1E_Y_ADFR_1_Maba1, Y_REG_SIZE, 0x05DB },

    {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}    /*STOP DIRANA INIT VECTOR*/
};

int cordic_ctab [] = {0x3243F6A8,
          0x1DAC6705,
          0x0FADBAFC, 0x07F56EA6,
          0x03FEAB76, 0x01FFD55B,
          0x00FFFAAA, 0x007FFF55,
          0x003FFFEA, 0x001FFFFD,
          0x000FFFFF, 0x0007FFFF,
          0x0003FFFF, 0x0001FFFF,
          0x0000FFFF, 0x00007FFF,
          0x00003FFF, 0x00001FFF,
          0x00000FFF, 0x000007FF,
          0x000003FF, 0x000001FF,
          0x000000FF, 0x0000007F,
          0x0000003F, 0x0000001F,
          0x0000000F, 0x00000008,
          0x00000004, 0x00000002,
          0x00000001, 0x00000000 };

///***************************************************///
///*  DSP Layer interfaces                           *///
///***************************************************///

/* ************************************************  */
/*   API    : dspStart                               */
/*   RETURN : OK or ERROR                            */
/* ************************************************* */
bool dspStart(int p_devnum)
{
   char l_devname[MAX_BUF];
   bool  l_res = OK;
   g_I2C_hndl=0;
   snprintf(l_devname,MAX_BUF,"/dev/i2c-%d",p_devnum);

   int fd = open_device (l_devname);

   if ( fd < 0 )
   {
      l_res=ERROR;
   }

   if(l_res == OK)
   {
	  l_res = setSlvAddr(SAF7730ADDR);
   }

   if(l_res == OK)
     {
   		l_res = ExecuteDiranaInit_125(); //EOL
     }

 return (l_res);
}

/** ************************************************  **/
/**   API    : dspWrite                                */
/**   RETURN : OK or ERROR                             */
/** ************************************************* **/
bool dspWrite(unsigned int p_address,
                unsigned char p_reg_size,
				unsigned int p_value_to_write)
{
   unsigned char l_data[6];
   struct i2c_rdwr_ioctl_data l_work_queue;
   struct i2c_msg l_msg_vect[2];  
   int l_ret=0;
   unsigned char *l_p_data;
   unsigned char l_add_H=0;
   unsigned char l_add_M=0;
   unsigned char l_add_L=0;
   unsigned char l_radio_address=SAF7730ADDR;
   
   bool l_ret_status=OK;
   
   if(g_I2C_hndl<=0)
   {
      PrintMessage(RED_PRINT,"dspWrite: I2C handle is not valid!!!");
	  l_ret_status=ERROR;
   }
   
   if(OK == l_ret_status)
   {


     l_p_data=(unsigned char *)&p_address;

     l_add_H=*(l_p_data);
     l_add_M=*(++l_p_data);
     l_add_L=*(++l_p_data);
     l_data[0]=l_add_L;
     l_data[1]=l_add_M;
     l_data[2]=l_add_H;

     l_p_data=(unsigned char *)&p_value_to_write;

     if(p_reg_size==X_REG_SIZE)
     {
//       PrintMessage(RED_PRINT,"dspWrite: writing 0x%06X at address 0x%06X with reg size %d \n",p_value_to_write, p_address, p_reg_size );
       l_data[3] = l_p_data[2];
       l_data[4] = l_p_data[1];
       l_data[5] = l_p_data[0];
     }


     if(p_reg_size==Y_REG_SIZE)
     {
//       PrintMessage(RED_PRINT,"dspWrite: writing 0x%04X at address 0x%06X with reg size %d \n",p_value_to_write, p_address, p_reg_size );
       l_data[3] = l_p_data[1];
       l_data[4] = l_p_data[0];
     }

     if(p_reg_size== PATCH_REG_SIZE)
          {
//            PrintMessage(RED_PRINT,"dspWrite: writing 0x%08X at address 0x%06X with reg size %d \n",p_value_to_write, p_address, p_reg_size );
            l_data[3] = l_p_data[3];
            l_data[4] = l_p_data[2];
            l_data[5] = l_p_data[1];
            l_data[6] = l_p_data[0];
          }

     l_msg_vect[0].addr=l_radio_address;        /** SETTO L'INDIRIZZO DELLO SLAVE SUL QUALE VOGLIO LEGGERE **/
     l_msg_vect[0].flags=0x0;                 /** operazione di scrittura dato **/
     l_msg_vect[0].len=p_reg_size+3;             /** 3 byte indirizzo registro + reg_size **/
     l_msg_vect[0].buf=(char *)&l_data[0];  /** buffer con address e valore da scrivere**/
     l_work_queue.nmsgs=1;
     l_work_queue.msgs=(struct i2c_msg*)&l_msg_vect[0];


     l_ret = ioctl(g_I2C_hndl,I2C_RDWR,(unsigned long)&l_work_queue);
//     PrintMessage(RED_PRINT,"\t ioctl l_work_queue with ret: %d\n",l_ret);

    if ( l_ret < 0 ) 
    {

       PrintMessage(RED_PRINT,"\tdspWrite:Error during I2C_RDWR ioctl with error code: %d\n",l_ret);
       l_ret_status=ERROR;
    }
    else
    {
      l_ret_status=OK;
    }
  }
  
  return(l_ret_status);
   
}


/** ************************************************  **/
/**   API    : dspRead                                 */
/**   RETURN : OK or ERROR                             */
/** ************************************************* **/
bool dspRead( unsigned int p_addr,
                unsigned char p_reg_size, 
				int *p_read_value )
{


   struct i2c_rdwr_ioctl_data l_work_queue;
   struct i2c_msg l_msg_vect[2]; 
   int l_ret;
   
   unsigned char l_data[7];
   unsigned char *l_p_data;
   unsigned char l_add_H=0;
   unsigned char l_add_M=0;
   unsigned char l_add_L=0;
   unsigned char l_radio_address=SAF7730ADDR;


   if(g_I2C_hndl<=0)
   {
      PrintMessage(RED_PRINT,"dspRead: I2C handle is not valid!!!");
      return (ERROR);
   }


   l_p_data=(unsigned char *)&p_addr;

   if (p_reg_size <= 3) 
   {
     l_add_H=*(l_p_data);
      l_add_M=*(++l_p_data);
      l_add_L=*(++l_p_data);

      l_data[0]=l_add_L;
      l_data[1]=l_add_M;
      l_data[2]=l_add_H;
      l_data[3]=0x00;
      l_data[4]=0x00;
      l_data[5]=0x00;


      l_msg_vect[0].addr=l_radio_address;        /** SETTO L'INDIRIZZO DELLO SLAVE SUL QUALE VOGLIO LEGGERE **/
      l_msg_vect[0].flags=0x4000;                 /** operazione di scrittura **/
      l_msg_vect[0].len=3;                     /** 3 byte **/
      l_msg_vect[0].buf=(char *)&l_data[0];  /** buffer da scrivere**/

      l_msg_vect[1].addr=l_radio_address;        /** SETTO L'INDIRIZZO DELLO SLAVE SUL QUALE VOGLIO LEGGERE **/
      l_msg_vect[1].flags=1;                /** operazione di lettura **/
      l_msg_vect[1].len=p_reg_size;               /** 3 byte **/
      l_msg_vect[1].buf=(char *)&l_data[3];  /** buffer da riempire con i valori letti**/

      l_work_queue.nmsgs=2;
      l_work_queue.msgs=(struct i2c_msg*)&l_msg_vect[0];

      ioctl(g_I2C_hndl,I2C_TIMEOUT,2);  /* set the timeout    */
      ioctl(g_I2C_hndl,I2C_RETRIES,1);  /* set the retries    */
      l_ret = ioctl(g_I2C_hndl,I2C_RDWR,(unsigned long)&l_work_queue);

      if ( l_ret < 0 ) 
      {
         PrintMessage(RED_PRINT,"\tdspRead:Error during I2C_RDWR ioctl with error code: %d\n",l_ret);
         return (ERROR);
      }
      else
      {
         if( p_reg_size == Y_REG_SIZE ){

         p_read_value[0] = l_data[3]<<8 | l_data[4];
//         PrintMessage(RED_PRINT,"dspRead: reading 0X%04X at address 0X%06X ",*p_read_value, p_addr);

         }
         else if( p_reg_size == X_REG_SIZE )
         {
          p_read_value[0] = l_data[3]<<16 | l_data[4]<<8 | l_data[5];
//          PrintMessage(RED_PRINT,"dspRead: reading 0X%06X at address 0X%06X ",*p_read_value, p_addr);
          }
         else
         {
//            PrintMessage(RED_PRINT,"\tdspRead: ERROR wrong register size!!!");
            return (ERROR);
         }

        return (OK);
      }
   }
   else
   {
      PrintMessage(RED_PRINT,"\tdspRead: ERROR wrong size");
   }

   return (ERROR);

}


/** ************************************************  **/
/**   API    : dspWrite                                */
/**   RETURN : OK or ERROR                             */
/** ************************************************* **/
bool dspTunerWrite(unsigned int p_tuner_address,
		unsigned char p_reg_size,
		unsigned char *p_value_to_write)
{
   struct i2c_rdwr_ioctl_data l_work_queue;
   struct i2c_msg l_msg_vect[2];
   int l_ret=0;
   unsigned char l_radio_address = SAF7730ADDR;

   bool l_ret_status=OK;

   if(g_I2C_hndl<=0)
   {
      PrintMessage(RED_PRINT,"dspTunerWrite: I2C handle is not valid!!!");
	  l_ret_status=ERROR;
   }

   if(OK == l_ret_status)
   {
//     PrintMessage(RED_PRINT,"dspTunerWrite: writing 0x%X at address 0x%06X with reg size %d ", p_value_to_write, p_tuner_address, p_reg_size );

     /* WRITE 3 bytes of register address */

     l_msg_vect[0].addr = l_radio_address;    /** SETTO L'INDIRIZZO DELLO SLAVE SUL QUALE VOGLIO LEGGERE **/
     l_msg_vect[0].flags = 0x0;               /** operazione di scrittura dato **/
     l_msg_vect[0].len = X_REG_SIZE;//sizeof( g_tuner_SwitchOnSecBusBuf );    /** 3 byte size indirizzo  tuner **/
     l_msg_vect[0].buf =(char*) &g_tuner_SwitchOnSecBusBuf[0];   /** 3 byte indirizzo  tuner **/


     /* Write data into DIRANA */
     l_msg_vect[1].addr = p_tuner_address;//indirizzo leafdice main tuner
     l_msg_vect[1].flags = 0x0;
     l_msg_vect[1].len = p_reg_size;
     l_msg_vect[1].buf =(char *) &p_value_to_write[0];
//     int i=0;
//   	 for(i=0;i<TUNER_REG_SIZE_WRITE;i++)
//	  {
//		 PrintMessage(RED_PRINT,"\tp_value_to_write[%d] = 0x%x",i, p_value_to_write[i]);
//	   }


     l_work_queue.nmsgs=2;
     l_work_queue.msgs=(struct i2c_msg*)&l_msg_vect[0];

     /* Request Write Operation to TUNER */
     l_ret = ioctl(g_I2C_hndl,I2C_RDWR,(unsigned long)&l_work_queue);
     if ( l_ret < 0 )
    {
       PrintMessage(RED_PRINT,"\tdspTunerWrite:Error during I2C_RDWR ioctl with error code: %d\n",l_ret);
       l_ret_status=ERROR;
    }
    else
    {
      l_ret_status=OK;
    }
  }

  return(l_ret_status);

}


bool dspTunerRead(unsigned int p_tuner_address,
		unsigned char p_reg_size,
		unsigned char *p_value_read)
{
	bool l_ret_status=OK;
	int l_ret_ioctl= 0 ;
	struct i2c_rdwr_ioctl_data l_work_queue;
	struct i2c_msg l_msg_vect[2];

	if(g_I2C_hndl<=0)
	  {
		 PrintMessage(RED_PRINT,"dspTunerRead: I2C handle is not valid!!!");
	  l_ret_status=ERROR;
	  }

	  if(OK == l_ret_status)
	  {
//		PrintMessage(RED_PRINT,"dspTunerRead: reading at address 0x%06X with reg size %d ", p_tuner_address, p_reg_size );

        /* WRITE 3 bytes of register address */
	   l_msg_vect[0].addr = SAF7730ADDR ;    /** SETTO L'INDIRIZZO DELLO SLAVE SUL QUALE VOGLIO LEGGERE **/
	   l_msg_vect[0].flags = 0x0;               /** operazione di scrittura dato **/
	   l_msg_vect[0].len = X_REG_SIZE;//sizeof( g_tuner_SwitchOnSecBusBuf );    /** 3 byte size indirizzo  tuner **/
	   l_msg_vect[0].buf =(char*) &g_tuner_SwitchOnSecBusBuf[0];   /** 3 byte indirizzo  tuner **/


	   l_msg_vect[1].addr=p_tuner_address;        /** SETTO L'INDIRIZZO DELLO SLAVE SUL QUALE VOGLIO LEGGERE **/
	   l_msg_vect[1].flags=1;                /** operazione di lettura **/
	   l_msg_vect[1].len=p_reg_size;               /** 3 byte **/
	   l_msg_vect[1].buf=(char *)&p_value_read[0];  /** buffer da riempire con i valori letti**/


	 l_work_queue.nmsgs=2;
	 l_work_queue.msgs=(struct i2c_msg*)&l_msg_vect[0];

    /* Request Write Operation to TUNER */
        l_ret_ioctl = ioctl(g_I2C_hndl,I2C_RDWR,(unsigned long)&l_work_queue);

        if(l_ret_ioctl<0)
        {
        	l_ret_status = ERROR;
        }
//        else
//        {
//          int i=0;
//		  for(i=0;i<p_reg_size;i++)
//		   {
//			 PrintMessage(RED_PRINT,"\tREAD tuner[%d] = 0x%x",i, p_value_read[i]);
//		  }
//        }
	  }


  return l_ret_status;
}


int open_device (char *p_devname)
{
  int l_ret = EXIT_SUCCESS;	
  
  printf("Apro il device %s\n",p_devname);
  g_I2C_hndl = open(p_devname,O_RDWR);
  if ( g_I2C_hndl < 0 )
  {
    printf("Non riesco ad aprire il file %s\n",p_devname);
	l_ret = EXIT_FAILURE;
  }

  return(l_ret);
}

int close_device ()
{
  return close (g_I2C_hndl);
}


bool setSlvAddr(int p_addr)
{
   bool l_res = OK;

   if ( ioctl(g_I2C_hndl,I2C_SLAVE,p_addr) < 0 )
   {
      printf("Errore %d %s\n",errno,strerror(errno));
      l_res = ERROR;
   }

   return(l_res);

}

bool initTuner()
{
	int l_res;

	unsigned char tuner_read[TUNER_REG_SIZE_READ] ={ 0x00 };
	unsigned char tuner_init[] = {
		        0x30,0x87,0xAA,0xE2,0x07,0x00,0x04
		    };
//	int i=0;
//	for(i=0;i<TUNER_REG_SIZE_WRITE;i++)
//	  {
//	     PrintMessage(RED_PRINT,"\ttuner_init[%d] = 0x%x",i, tuner_init[i]);
//	  }
	l_res = dspTunerWrite( TUNER_T1A_ADDRESS, TUNER_REG_SIZE_WRITE, &tuner_init[0] );

if (l_res==OK )
	   {


		l_res = dspTunerRead(TUNER_T1A_ADDRESS, TUNER_REG_SIZE_READ, &tuner_read[0]);
//		for(i=0;i<TUNER_REG_SIZE_READ;i++)
//			  {
//			     		 PrintMessage(RED_PRINT,"\ttuner_read[%d] = 0x%x",i, tuner_read[i]);
//			  }
  	  }

return l_res;
}


/***************************************************************************/
/*   NAME: LoadNXPPatch_Router                                            */
/*                                                                         */
/*   GOAL:  (*)                                                            */
/*                                                                         */
/* RESULT: true (Success) false (Failed)                                     */
/*                                                                         */
/***************************************************************************/
bool LoadNXPPatch(unsigned int* p_vect_patch)
{
  PrintMessage(GREEN_PRINT,"LoadNXPPatch");
  bool l_res = OK;
  char l_i = 0;
  unsigned int l_starting_address = p_vect_patch[0]; // ADSP_P_Router_main;

  for(l_i = 0 ; ( OK == l_res ) && ( l_i < 21 ); l_i++ )
  {
    l_res = dspWrite( l_starting_address + l_i, PATCH_REG_SIZE, p_vect_patch[l_i+1]);
    usleep(100);
  }
  return l_res;
}


bool ExecuteDiranaInit_125( void )
{

	PrintMessage(GREEN_PRINT,"ExecuteDiranaInit_125 ");
    bool l_res = OK ;
    int l_i;
    int Diranainit_Retry = 0;

    DIRANA_STATUS Diranainit_Status = DIRANA_WAIT;
    int l_value = 0;
    dspRead( ADSP_DIGSRC_SEL, X_REG_SIZE, &l_value );
    int Diranainit_TaskTime = 0;

    for(l_i=0; l_i < MAXROW_DIRANA_INIT_VECTOR; l_i++)
    {

    if ( ( Diranainit_TaskTime != 0xFFFFFFFF )&& ( Diranainit_Status == DIRANA_WAIT ))
    {
        if ( ( DiranaInit_125_Vector[l_i][1] != 0xFFFFFFFF ) &&
             ( DiranaInit_125_Vector[l_i][1] != 0x00000000 )
             )
        {
//        	PrintMessage(GREEN_PRINT,"  DiranaInit_125_Vector[%d][0] = 0X%X",   l_i,DiranaInit_125_Vector[l_i][0]);
//        	PrintMessage(GREEN_PRINT,"  DiranaInit_125_Vector[%d][1] = 0X%X",   l_i,DiranaInit_125_Vector[l_i][1]);
//        	PrintMessage(GREEN_PRINT,"  DiranaInit_125_Vector[%d][2] = 0X%X",   l_i,DiranaInit_125_Vector[l_i][2]);
            if ( DiranaInit_125_Vector[l_i][1] > 0x00000004 ) //per registri più grandi di 4B o buffer di dati
            {//per NXP_PATCH_ROUTER_SIZE

            	l_res = LoadNXPPatch((unsigned int*)DiranaInit_125_Vector[l_i][2]);
            }
            else
            {
                l_res = dspWrite( (unsigned int)DiranaInit_125_Vector[l_i][0],
                              (char)DiranaInit_125_Vector[l_i][1],
                              (unsigned int)DiranaInit_125_Vector[l_i][2] );
            }
        }
        else
        {
            if ( DiranaInit_125_Vector[l_i][1] == 0xFFFFFFFF )//fine del buffer
            {
                Diranainit_Status = OK;
                Diranainit_TaskTime = 0xFFFFFFFF;
                dspRead( ADSP_DIGSRC_SEL, X_REG_SIZE, &l_value );  //CE
            }
            else
            {}
        }
        if ( l_res == OK )
        {
            if ( Diranainit_TaskTime != 0xFFFFFFFF )
            {
                Diranainit_TaskTime++;
                Diranainit_Status = DIRANA_WAIT;
            }
            Diranainit_Retry = 0;
        }
        else
        {
            Diranainit_Retry++;
            if ( Diranainit_Retry == 10 )
            {
                Diranainit_Status = DIRANA_ERROR;
                Diranainit_Retry = 0;
            }
            else
            {
                Diranainit_Status = DIRANA_WAIT;
            }
        }
    }
    usleep(2000);//task a 2ms
    }

return l_res;
}


void DIRANA_get_waveform_registers(unsigned int p_frequency, unsigned short *p_hi, unsigned short *p_low)
{
  int l_temp = 0;

  l_temp = DIRANA_sinusoid_angle_2_30(p_frequency);
  l_temp = DIRANA_cordic_cosine(l_temp);

  DIRANA_2_30_to_9_23_notation_HiLo(l_temp, p_hi, p_low);

}

int DIRANA_sinusoid_angle_2_30(unsigned int p_frequency)
{
  return (int)((long)TWOPIdiv44100*(long)p_frequency);
  return 1;
}

int DIRANA_cordic_cosine(int p_theta)
{
  int l_cos = 0, l_temp = 0 ;

  if( p_theta > HALF_PI || p_theta  < 0) // || (p_theta >> 31 ) != 0
  {
    // PI/2  < theta < PI
    cordic32(HALF_PI - ( p_theta - HALF_PI) , &l_temp, &l_cos);
    // cordic32 returns -cos (frequencies after ~14000 give theta values in this range)
    l_cos = -l_cos;
  }
  else
  {
    cordic32(p_theta, &l_temp, &l_cos);
  }
  // we don't need the sine
  return l_cos;
}

/**
 *
 * @param p_theta
 * @param p_sin
 * @param p_cos
 */
void cordic32(int p_theta, int *p_sin, int *p_cos)
{
  int32_t k, d, tx, ty, tz;
  int32_t x = CORDIC_1K, y = 0,z = p_theta;
  for (k=0; k < CORDIC_NTAB; ++k)
  {
    d = z>>31;
    // d = z>=0 ? 0 : -1;
    tx = x - (((y>>k) ^ d) - d); //x - ( y*2^(-k) * d )
    ty = y + (((x>>k) ^ d) - d); //y + ( x*2^(-k) * d )
    tz = z - ((cordic_ctab[k] ^ d) - d);
    x = tx; y = ty; z = tz;
  }
  *p_cos = x; *p_sin = y;
}

void DIRANA_2_30_to_9_23_notation_HiLo(int p_value, unsigned short *p_hi, unsigned short *p_low)
{
  // 2.30 to 9.23
  p_value = (p_value >> 7);

  *p_hi  = (p_value & 0x00FFF000) >> 12;
  *p_low = (p_value & 0x00000FFF) >> 1; // first bit must be 0
}
