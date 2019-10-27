//#include "i2s_reg.h"
#include "gpio.h"
#include "slc_register.h"
#include "user_config.h"
#include "espressif/esp_common.h"
#include "espressif/spi_flash.h"
#include "c_types.h"
#include "freertos/queue.h"


et_uint32 audio_voice_data;
xQueueHandle dmaQueue;

os_timer_t rest_timer;

#define  I2S_RECEIVE
#define IIS_RX_BUF_LEN  512  //unit Byte
#define IIS_TX_BUF_LEN  512  //unit Byte

#define WAV_HEAD_LEN					78
#define MODE_INFO_FLASH_SECTOR		0xFC
#define SERVER_INFO_SECTOR				0xFF
#define APPKEY_UID_FASH_SECTOR		0xFE
#define AUDIO_FLASH_START_SECTOR		0x100
#define AUDIO_FLASH_STOP_SECTOR		0x1AE
#define AUDIO_FLASH_LEN				0xAE000
#define AUDIO_FLASH_READ_START_ADDR	AUDIO_FLASH_START_SECTOR * SPI_FLASH_SEC_SIZE + WAV_HEAD_LEN

//et_uint32 i2s_rx_buff1[IIS_RX_BUF_LEN/4];
//et_uint32 i2s_rx_buff2[IIS_RX_BUF_LEN/4];
//et_uint32 i2s_rx_test1[IIS_RX_BUF_LEN/8];


#ifdef I2S_RECEIVE
et_uint32 i2s_tx_buff1[IIS_TX_BUF_LEN/4];
et_uint32 i2s_tx_buff2[IIS_TX_BUF_LEN/4];
et_uint32 i2s_tx_buff3[IIS_TX_BUF_LEN/4];
//et_uint32 i2s_tx_buff4[IIS_TX_BUF_LEN/4];
//et_uint32 i2s_tx_test1[IIS_TX_BUF_LEN*2];
//et_uint32 i2s_tx_test2[IIS_TX_BUF_LEN*3];
//et_uint32 i2s_tx_test3[IIS_TX_BUF_LEN];
//et_uint32 i2s_tx_test4[IIS_TX_BUF_LEN];
et_uint32 i2s_tx_test;
#endif


struct sdio_queue
{
	uint32	blocksize:12;
	uint32	datalen:12;
	uint32	unused:5;
	uint32	sub_sof:1;
	uint32 	eof:1;
	uint32	owner:1;

	uint32	buf_ptr;
	uint32	next_link_ptr;
};


#define i2c_bbpll                                 0x67
#define i2c_bbpll_en_audio_clock_out            4
#define i2c_bbpll_en_audio_clock_out_msb        7
#define i2c_bbpll_en_audio_clock_out_lsb        7
#define i2c_bbpll_hostid                           4

#define i2c_writeReg_Mask(block, host_id, reg_add, Msb, Lsb, indata)  rom_i2c_writeReg_Mask(block, host_id, reg_add, Msb, Lsb, indata)
#define i2c_readReg_Mask(block, host_id, reg_add, Msb, Lsb)  rom_i2c_readReg_Mask(block, host_id, reg_add, Msb, Lsb)
#define i2c_writeReg_Mask_def(block, reg_add, indata) \
      i2c_writeReg_Mask(block, block##_hostid,  reg_add,  reg_add##_msb,  reg_add##_lsb,  indata)
#define i2c_readReg_Mask_def(block, reg_add) \
      i2c_readReg_Mask(block, block##_hostid,  reg_add,  reg_add##_msb,  reg_add##_lsb)

#define CONF_RXLINK_ADDR(addr)		CLEAR_PERI_REG_MASK(SLC_RX_LINK,SLC_RXLINK_DESCADDR_MASK);\
	SET_PERI_REG_MASK(SLC_RX_LINK, ((et_uint32)(addr)) & SLC_RXLINK_DESCADDR_MASK)
#define CONF_TXLINK_ADDR(addr) 		CLEAR_PERI_REG_MASK(SLC_TX_LINK,SLC_TXLINK_DESCADDR_MASK);\
	SET_PERI_REG_MASK(SLC_TX_LINK, ((et_uint32)(addr)) & SLC_TXLINK_DESCADDR_MASK)

#define START_RXLINK() 	SET_PERI_REG_MASK(SLC_RX_LINK, SLC_RXLINK_START)
#define START_TXLINK() 	SET_PERI_REG_MASK(SLC_TX_LINK, SLC_TXLINK_START)

#define STOP_RXLINK() 	CLEAR_PERI_REG_MASK(SLC_RX_LINK, SLC_RXLINK_START)
#define STOP_TXLINK() 	CLEAR_PERI_REG_MASK(SLC_TX_LINK, SLC_TXLINK_START)


#define SAMPLE_NUM   6
#define RX_NUM 128 //unit word

//Initialize the I2S module
void i2s_init();
void i2s_Sample_set(et_uint32 rate);
void i2s_tx_rx_start(void);
void i2s_tx_start(void);
void i2s_tx_start(void);
void i2s_stop(void);


//Initialize the SLC module for DMA function
void slc_init();

void slc_isr(void *para);

void audio_player(et_uint32 block_len,et_uint32 pcmdata_len);

void audio_record(et_int32 block_len,et_int32 pcmdata_len);

void i2s_audio_init();






