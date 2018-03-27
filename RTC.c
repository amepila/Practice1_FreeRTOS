
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_i2c.h"
#include "FREErtos.h"
#include "event_groups.h"

#define SECONDSADDRESS (0x02)
#define MINUTESADDRESS (0x03)
#define HOURSADDRESS (0x04)
#define DAYADDRESS (0x05)
#define MONTHADDRESS (0x07)
#define YEARADDRESS (0x08)
#define RTCADDRESS (0x51)

i2c_master_handle_t g_m_handle;
volatile bool g_MasterCompletionFlag = false;


static void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle,
        status_t status, void * userData)
{

	if (status == kStatus_Success)
	{
		g_MasterCompletionFlag = true;
	}
}


uint8_t readRTC_sec()
{
	i2c_master_config_t masterConfig;
	i2c_master_transfer_t masterXfer;
	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle,i2c_master_callback, NULL);

	uint8_t secadd = SECONDSADDRESS;

	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = 0;
	masterXfer.subaddressSize = 0;
	masterXfer.data = &secadd;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferNoStopFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

	uint8_t seconds;

	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddress = 0;
	masterXfer.subaddressSize = 0;
	masterXfer.data = &seconds;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferRepeatedStartFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

	while (!g_MasterCompletionFlag){}
	g_MasterCompletionFlag = false;

	return seconds;

}

uint8_t readRTC_min()
{
	i2c_master_config_t masterConfig;
	i2c_master_transfer_t masterXfer;
	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle,i2c_master_callback, NULL);

	uint8_t minadd = MINUTESADDRESS;

	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = 0;
	masterXfer.subaddressSize = 0;
	masterXfer.data = &minadd;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferNoStopFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

	uint8_t minutes;

	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddress = 0;
	masterXfer.subaddressSize = 0;
	masterXfer.data = &minutes;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferRepeatedStartFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

	while (!g_MasterCompletionFlag){}
	g_MasterCompletionFlag = false;

	return minutes;

}

uint8_t readRTC_hour()
{
	i2c_master_config_t masterConfig;
	i2c_master_transfer_t masterXfer;
	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle,i2c_master_callback, NULL);

	uint8_t houradd = HOURSADDRESS;

	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = 0;
	masterXfer.subaddressSize = 0;
	masterXfer.data = &houradd;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferNoStopFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

	uint8_t hours;

	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddress = 0;
	masterXfer.subaddressSize = 0;
	masterXfer.data = &hours;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferRepeatedStartFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

	while (!g_MasterCompletionFlag){}
	g_MasterCompletionFlag = false;

	return hours;

}

uint8_t readRTC_day()
{
	i2c_master_config_t masterConfig;
	i2c_master_transfer_t masterXfer;
	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle,i2c_master_callback, NULL);

	uint8_t dayadd = DAYADDRESS;

	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = 0;
	masterXfer.subaddressSize = 0;
	masterXfer.data = &dayadd;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferNoStopFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

	uint8_t days;

	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddress = 0;
	masterXfer.subaddressSize = 0;
	masterXfer.data = &days;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferRepeatedStartFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

	while (!g_MasterCompletionFlag){}
	g_MasterCompletionFlag = false;

	return days;

}

uint8_t readRTC_month()
{
	i2c_master_config_t masterConfig;
	i2c_master_transfer_t masterXfer;
	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle,i2c_master_callback, NULL);

	uint8_t monthadd = MONTHADDRESS;

	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = 0;
	masterXfer.subaddressSize = 0;
	masterXfer.data = &monthadd;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferNoStopFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

	uint8_t months;

	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddress = 0;
	masterXfer.subaddressSize = 0;
	masterXfer.data = &months;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferRepeatedStartFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

	while (!g_MasterCompletionFlag){}
	g_MasterCompletionFlag = false;

	return months;

}

uint8_t readRTC_year()
{
	i2c_master_config_t masterConfig;
	i2c_master_transfer_t masterXfer;
	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle,i2c_master_callback, NULL);

	uint8_t yearadd = YEARADDRESS;

	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = 0;
	masterXfer.subaddressSize = 0;
	masterXfer.data = &yearadd;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferNoStopFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

	uint8_t years;

	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddress = 0;
	masterXfer.subaddressSize = 0;
	masterXfer.data = &years;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferRepeatedStartFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

	while (!g_MasterCompletionFlag){}
	g_MasterCompletionFlag = false;

	return years;

}

/*
 * SETTERS
 */

void setRTC_sec(uint8_t sec)
{
	i2c_master_config_t masterConfig;
	i2c_master_transfer_t masterXfer;
	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle,i2c_master_callback, NULL);


	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = SECONDSADDRESS;
	masterXfer.subaddressSize = 1;
	masterXfer.data = &sec;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

}



void setRTC_min(uint8_t min)
{
	i2c_master_config_t masterConfig;
	i2c_master_transfer_t masterXfer;
	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle,i2c_master_callback, NULL);


	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = MINUTESADDRESS;
	masterXfer.subaddressSize = 1;
	masterXfer.data = &min;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

}

void setRTC_hour(uint8_t hour)
{
	i2c_master_config_t masterConfig;
	i2c_master_transfer_t masterXfer;
	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle,i2c_master_callback, NULL);


	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = HOURSADDRESS;
	masterXfer.subaddressSize = 1;
	masterXfer.data = &hour;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

}

void setRTC_day(uint8_t day)
{
	i2c_master_config_t masterConfig;
	i2c_master_transfer_t masterXfer;
	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle,i2c_master_callback, NULL);


	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = DAYADDRESS;
	masterXfer.subaddressSize = 1;
	masterXfer.data = &day;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

}

void setRTC_month(uint8_t month)
{
	i2c_master_config_t masterConfig;
	i2c_master_transfer_t masterXfer;
	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle,i2c_master_callback, NULL);


	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = MONTHADDRESS;
	masterXfer.subaddressSize = 1;
	masterXfer.data = &month;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

}

void setRTC_year(uint16_t year)
{
	i2c_master_config_t masterConfig;
	i2c_master_transfer_t masterXfer;
	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(kCLOCK_BusClk));
	I2C_MasterTransferCreateHandle(I2C0, &g_m_handle,i2c_master_callback, NULL);


	masterXfer.slaveAddress = RTCADDRESS;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = YEARADDRESS;
	masterXfer.subaddressSize = 1;
	masterXfer.data = &year;
	masterXfer.dataSize = 1;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferNonBlocking(I2C0, &g_m_handle,&masterXfer);

}
