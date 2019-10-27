/*
 * ram_fifo.h
 *
 *  Created on: 2019年10月19日
 *      Author: Administrator
 */

#ifndef APP_INCLUDE_RAM_FIFO_H_
#define APP_INCLUDE_RAM_FIFO_H_

#define SPIRAMSIZE 8*1024

int ICACHE_FLASH_ATTR spiRamFifoInit();
void ICACHE_FLASH_ATTR spiRamFifoRead(char *buff, int len);
void ICACHE_FLASH_ATTR spiRamFifoWrite(char *buff, int len);
int ICACHE_FLASH_ATTR spiRamFifoFill();
int ICACHE_FLASH_ATTR spiRamFifoFree();
long ICACHE_FLASH_ATTR spiRamGetOverrunCt();
long ICACHE_FLASH_ATTR spiRamGetUnderrunCt();


#endif /* APP_INCLUDE_RAM_FIFO_H_ */
