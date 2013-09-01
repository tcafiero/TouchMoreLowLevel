/** @file i2c-dev.h
*/

#ifndef I2C_DEV_H
#define I2C_DEV_H
#include <linux/types.h>
#include <sys/ioctl.h>
#include "string.h"


#define I2C_SLAVE 0x0703  /* Change slave address  */
#define I2C_RDWR  0x0707  /* Combined R/W transfer */


/* ----- commands for the ioctl like i2c_command call:
 * note that additional calls are defined in the algorithm and hw
 *	dependent layers - these can be listed here, or see the
 *	corresponding header files.
 */
				/* -> bit-adapter specific ioctls	*/
#define I2C_RETRIES	0x0701	/* number of times a device address      */
				/* should be polled when not            */
/* acknowledging 			*/
#define I2C_TIMEOUT	0x0702	/* set timeout - call with int 		*/

/*
 * I2C Message - used for pure i2c transaction, also from /dev interface
 */
struct i2c_msg
{
	__u16 addr;	/* slave address			*/
	unsigned short flags;
#define I2C_M_TEN	0x10	/* we have a ten bit chip address	*/
#define I2C_M_RD	0x01
#define I2C_M_NOSTART	0x4000
#define I2C_M_REV_DIR_ADDR	0x2000
#define I2C_M_IGNORE_NAK	0x1000
#define I2C_M_NO_RD_ACK		0x0800
	short len;		/* msg length				*/
	char *buf;		/* pointer to msg data			*/
};

/* This is the structure as used in the I2C_RDWR ioctl call */
struct i2c_rdwr_ioctl_data {
	struct i2c_msg *msgs;	/* pointers to i2c_msgs */
	int nmsgs;		/* number of i2c_msgs */
};

#endif // #ifndef I2C_DEV_H
