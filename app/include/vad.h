/*
 * vad.h
 *
 *  Created on: 2019Äê10ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef APP_INCLUDE_VAD_H_
#define APP_INCLUDE_VAD_H_

#include "esp_common.h"
#define debug 0
#define vad_value 200
#define MAX_TIMES 12000
#define vad_satrt 1
#define vad_end 2
struct vad_queue
{
	uint32	times;
	uint32	delays;
	uint32	delay1;
	uint32	start;
	uint32	end;
};

struct vad_queue vad_vol;

int simpleVAD(short* waveData, int frameSize);
int vad_process(short* waveData, int frameSize);
#endif /* APP_INCLUDE_VAD_H_ */
