/*
 * pdm.h
 *
 *  Created on: 2019��3��12��
 *      Author: achin
 */

#ifndef APP_INCLUDE_DRIVER_PDM_H_
#define APP_INCLUDE_DRIVER_PDM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_common.h"
#include "stdint.h"

//#define License "bbb53aed"
int8 LICENSE[16];

typedef int				INT;
typedef unsigned int	UINT;

/* This type MUST be 8-bit */
typedef unsigned char	BYTE;
typedef unsigned char	BYTE1;

/* These types MUST be 16-bit */
typedef short			SHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;
typedef long			LONG;
typedef unsigned long	DWORD;
typedef unsigned long long QWORD;

typedef unsigned long	_DWORD;
typedef unsigned short	 _WORD;

typedef long long __int64;

#define PDM_FILTER_ENDIANNESS_LE     ((uint16_t)0x0000)
#define PDM_FILTER_ENDIANNESS_BE     ((uint16_t)0x0001)

#define PDM_FILTER_BIT_ORDER_LSB     ((uint16_t)0x0000)
#define PDM_FILTER_BIT_ORDER_MSB     ((uint16_t)0x0001)

#define PDM_FILTER_DEC_FACTOR_48     ((uint16_t)0x0001)
#define PDM_FILTER_DEC_FACTOR_64     ((uint16_t)0x0002)
#define PDM_FILTER_DEC_FACTOR_80     ((uint16_t)0x0003)
#define PDM_FILTER_DEC_FACTOR_128    ((uint16_t)0x0004)
#define PDM_FILTER_DEC_FACTOR_16     ((uint16_t)0x0005)
#define PDM_FILTER_DEC_FACTOR_24     ((uint16_t)0x0006)
#define PDM_FILTER_DEC_FACTOR_32     ((uint16_t)0x0007)

#define PDM_FILTER_INIT_ERROR           ((uint16_t)0x0010)
#define PDM_FILTER_CONFIG_ERROR         ((uint16_t)0x0020)
#define PDM_FILTER_ENDIANNESS_ERROR     ((uint16_t)0x0001)
#define PDM_FILTER_BIT_ORDER_ERROR      ((uint16_t)0x0002)
#define PDM_FILTER_CRC_LOCK_ERROR       ((uint16_t)0x0004)
#define PDM_FILTER_DECIMATION_ERROR     ((uint16_t)0x0008)
#define PDM_FILTER_GAIN_ERROR           ((uint16_t)0x0040)
#define PDM_FILTER_SAMPLES_NUMBER_ERROR ((uint16_t)0x0080)
#define PDM2PCM_INTERNAL_MEMORY_SIZE 16

typedef struct{
  uint16_t bit_order;
  uint16_t endianness;
  uint32_t high_pass_tap;
  uint16_t in_ptr_channels;
  uint16_t out_ptr_channels;
  uint32_t pInternalMemory[PDM2PCM_INTERNAL_MEMORY_SIZE];
}PDM_Filter_Handler_t;

typedef struct{
  uint16_t decimation_factor;
  uint16_t output_samples_number;
  int16_t  mic_gain;
}PDM_Filter_Config_t;

//typedef short _int16;
PDM_Filter_Handler_t pdm_hander;
PDM_Filter_Config_t pdm_config;



uint32_t PDM_TO_PCM(void *a1, void *a2, PDM_Filter_Handler_t *a3);
//uint32_t PDM_Filter_setConfig(PDM_Filter_Handler_t *a1, PDM_Filter_Config_t *a2);
//uint32_t PDM_Filter_Init(PDM_Filter_Handler_t *HANDER);
void PDM_init(void);
#ifdef __cplusplus
}
#endif


#endif /* APP_INCLUDE_DRIVER_PDM_H_ */
