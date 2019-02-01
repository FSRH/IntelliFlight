#ifndef __ICM20689_DEFS_H__
#define __ICM20689_DEFS_H__

/*! CPP guard */
#ifdef __cplusplus
extern "C"
{
#endif

/****************************************************************/
/*! @name		Header includes				*/
/****************************************************************/
#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/kernel.h>
#else
#include <stdint.h>
#include <stddef.h>
#endif

/****************************************************************/
/*! @name		Common macros				*/
/****************************************************************/
#ifdef __KERNEL__
#if !defined(UINT8_C) && !defined(INT8_C)
#define INT8_C(x)       S8_C(x)
#define UINT8_C(x)      U8_C(x)
#endif

#if !defined(UINT16_C) && !defined(INT16_C)
#define INT16_C(x)      S16_C(x)
#define UINT16_C(x)     U16_C(x)
#endif

#if !defined(INT32_C) && !defined(UINT32_C)
#define INT32_C(x)      S32_C(x)
#define UINT32_C(x)     U32_C(x)
#endif

#if !defined(INT64_C) && !defined(UINT64_C)
#define INT64_C(x)      S64_C(x)
#define UINT64_C(x)     U64_C(x)
#endif
#endif

/*! @name C standard macros */
#ifndef NULL
#ifdef __cplusplus
#define NULL   0
#else
#define NULL   ((void *) 0)
#endif
#endif

/****************************************************************/
/*! @name		ICM20689 Macros				*/
/****************************************************************/

/*! @name GYROSCOPE SELF-TEST REGISTERS */
#define ICM20689_SELF_TEST_X_GYRO UINT8_C(0x00)
#define ICM20689_SELF_TEST_Y_GYRO UINT8_C(0x01)
#define ICM20689_SELF_TEST_Z_GYRO UINT8_C(0x02)

/*! @name AACCELEROMETER SELF-TEST REGISTERS */
#define ICM20689_SELF_TEST_X_ACCEL UINT8_C(0x0D)
#define ICM20689_SELF_TEST_Y_ACCEL UINT8_C(0x0E)
#define ICM20689_SELF_TEST_Z_ACCEL UINT8_C(0x0F)

/*! @name GYRO OFFSET ADJUSTMENT REGISTER */
#define ICM20689_XG_OFFS_USRH UINT8_C(0x13)
#define ICM20689_XG_OFFS_USRL UINT8_C(0x14)
#define ICM20689_YG_OFFS_USRH UINT8_C(0x15)
#define ICM20689_YG_OFFS_USRL UINT8_C(0x16)
#define ICM20689_ZG_OFFS_USRH UINT8_C(0x17)
#define ICM20689_ZG_OFFS_USRL UINT8_C(0x18)

/*! @name  SAMPLE RATE DIVIDER */
#define SMPLRT_DIV UINT8_C(0x19)

/*! @name CHIP CONFIGURATION */
#define CONFIG UINT8_C(0x1A)
#define GYRO_CONFIG UINT8_C(0x15)
#define ACCEL_CONFIG UINT8_C(0x15)
#define ACCEL_CONFIG_2 UINT8_C(0x15)
#define LP_MODE_CFG UINT8_C(0x15)
#define ACCEL_WOM_X_THR UINT8_C(0x15)
#define ACCEL_WOM_Y_THR UINT8_C(0x15)
#define ACCEL_WOM_Z_THR UINT8_C(0x15)
#define FIFO_EN UINT8_C(0x15)
#define FSYNC_INT UINT8_C(0x15)
#define INT_PIN_CFG UINT8_C(0x15)
#define INT_ENABLE UINT8_C(0x15)
#define DMP_INT_STATUS UINT8_C(0x15)
#define INT_STATUS UINT8_C(0x15)
#define ACCEL_XOUT_H UINT8_C(0x15)
#define ACCEL_XOUT_L UINT8_C(0x15)
#define ACCEL_YOUT_H UINT8_C(0x15)
#define ACCEL_YOUT_L UINT8_C(0x15)
#define ACCEL_ZOUT_H UINT8_C(0x15)
#define ACCEL_ZOUT_L UINT8_C(0x15)
#define TEMP_OUT_H UINT8_C(0x15)
#define TEMP_OUT_L UINT8_C(0x15)
#define GYRO_XOUT_H UINT8_C(0x15)
#define GYRO_XOUT_L UINT8_C(0x15)
#define GYRO_YOUT_H UINT8_C(0x15)
#define GYRO_YOUT_L UINT8_C(0x15)
#define GYRO_ZOUT_H UINT8_C(0x15)
#define GYRO_ZOUT_L UINT8_C(0x15)
#define SIGNAL_PATH_RESET UINT8_C(0x15)


/*! @name Chip IDs */
#define BMP280_CHIP_ID1	UINT8_C(0x56)

#endif //__ICM20689_DEFS_H__
