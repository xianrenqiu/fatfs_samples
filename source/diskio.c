/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */

#include <stdio.h>
#include <string.h>

#if USE_RAM
BYTE disk[SEC_SIZE * SEC_COUNT];
#else
int f_fatfs = -1;
#endif

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
  DISK_DEBUG("  [%s fs] disk_initialize\n", USE_RAM ? "RAM" : "DISK");
  
#if (USE_RAM == 0)
	if (f_fatfs == -1)
		f_fatfs = open(DISK_NAME, O_SYNC | O_RDWR, S_IRUSR | S_IWUSR);

	lseek(f_fatfs, 0, SEEK_SET);
#endif

  return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DISK_DEBUG("  [%s fs] disk_read %ld %d\n", USE_RAM ? "RAM" : "DISK", sector, count);
#if USE_RAM	
	void *p_disk = (void *)(disk + sector * SEC_SIZE);
	memcpy(buff, p_disk, count * SEC_SIZE);
#else
	lseek(f_fatfs, sector * SEC_SIZE, SEEK_SET);
	read(f_fatfs, buff, count * SEC_SIZE);
#endif

	return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DISK_DEBUG("  [%s fs] disk_write %ld %d\n", USE_RAM ? "RAM" : "DISK", sector, count);	
#if USE_RAM	
	void *p_disk = (void *)(disk + sector * SEC_SIZE);
	memcpy(p_disk, buff, count * SEC_SIZE);
#else
	lseek(f_fatfs, sector * SEC_SIZE, SEEK_SET);
	write(f_fatfs, buff, count * SEC_SIZE);
#endif

	return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DISK_DEBUG("  [%s fs] disk_ioctl %d %d\n", USE_RAM ? "RAM" : "DISK", pdrv, cmd);	

	if (cmd == CTRL_SYNC) 
	  return RES_OK;	  

	if (cmd == GET_SECTOR_COUNT) 
	{
	  *((DWORD*)buff) = SEC_COUNT;
	  return RES_OK;	  
	}

	if (cmd == GET_BLOCK_SIZE) 
	{
	  *((WORD*)buff) = (WORD)(SEC_COUNT * SEC_SIZE);
	  return RES_OK;
	}

	return RES_PARERR;
}

DWORD get_fattime (void)
{

	return 0;
}
