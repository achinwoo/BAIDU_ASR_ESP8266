/*
 * ESPRSSIF MIT License
 *
 * Copyright (c) 2015 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "esp_common.h"
#include "lwip/lwip/sockets.h"
#include "i2s.h"
#include "gpio.h"
#include "espressif/spi_flash.h"
#include "espressif/smartconfig.h"
#include "espressif/airkiss.h"
#include "espressif/esp_sta.h"
#include "apps/sntp.h"
#include "user_config.h"
#include "hw_timer.h"
#include "pdm.h"
#include "rest.h"
#include "freertos/queue.h"
#include "ram_fifo.h"
#include "vad.h"
#include "vad.h"
//#include "esr.h"

unsigned int currDMABuff;
int atcmd_sid;
int8_t led_stu;
int32_t led_num;
int8_t str_cmd[64];
char *utf8=buffer+1024;
void set_config();
void atcmd_task(void *pvParameters )
{
	u8 *PA;
	u8 *PB;
	PA=strstr(str_cmd,"\r\n");
	PA[0]=0;
	switch(atcmd_sid)
	{
	case 0:
		printf("NOK\r\n");
	break;
	case 1:
		if(strstr(str_cmd,"?"))
		printf("MICGAIN=%s\r\n",mic_gain);
		else
		{
		if(atoi(str_cmd)>32)
			printf("NOK\r\n");
		else
			{
			sprintf(mic_gain,str_cmd);
			set_config();
			printf("OK\r\n");
			}
		}
	break;
	case 2:
		if(strstr(str_cmd,"?"))
		printf("KWS=%s\r\n",at_kws);
		else
		{
		sprintf(at_kws,str_cmd);
		set_config();
		printf("OK\r\n");
		}
	break;
	case 3:
		if(strstr(str_cmd,"?"))
		printf("VAD=%s\r\n",at_VAD);
		else
		{
			if((!strstr(str_cmd,"ON"))&&(!strstr(str_cmd,"OFF")))
			{
				printf("NOK\r\n");
			}
			else
			{
				sprintf(at_VAD,str_cmd);
				set_config();
				printf("OK\r\n");
			}
		}
	break;
	case 4:
		if(strstr(str_cmd,"?"))
		printf("SPEECH=%s\r\n",speech);
		else
		{
			if(((!strstr(str_cmd,"START"))&&(!strstr(str_cmd,"STOP")))||(strstr(at_VAD,"ON"))||(GPIO_INPUT_GET(0)==0))
			{
				printf("NOK\r\n");
			}
			else
			{
				sprintf(speech,str_cmd);
				printf("OK\r\n");
			}
		}
	break;
	case 5:
		if(strstr(str_cmd,"?"))
		printf("SSID=%s,%s\r\n",SSID,Password);
		else
		{
			PA=strstr(str_cmd,",");
			PB=str_cmd;
			memcpy(SSID,str_cmd,PA-PB);
			SSID[PA-PB]=0;
			PA+=1;
			//PA=strstr(PB,"\r\n");
			memcpy(Password,PA,strlen(PA));
			Password[strlen(PA)]=0;
		set_config();
		printf("OK\r\n");
		}
	break;
	case 6:
		if(strstr(str_cmd,"?"))
		printf("APIKEY=%s,%s\r\n",BaiDu_API_Key,BaiDu_Secret_Key);//"APIKEY:%s,%s\r\n",BaiDu_API_Key,BaiDu_Secret_Key
		else
		{
			PA=strstr(str_cmd,",");
			PB=str_cmd;
			memcpy(BaiDu_API_Key,str_cmd,PA-PB);
			BaiDu_API_Key[PA-PB]=0;
			PA+=1;
			//PA=strstr(PB,"\r\n");
			memcpy(BaiDu_Secret_Key,PA,strlen(PA));
			BaiDu_Secret_Key[strlen(PA)]=0;
		set_config();
		printf("OK\r\n");
		}
	break;
	case 7:
		if(strstr(str_cmd,"?"))
		printf("SPHMOD=%s\r\n","FAST");
		else
		{
			if((!strstr(str_cmd,"FAST"))&&(!strstr(str_cmd,"NORMAL")))
			{
				printf("NOK\r\n");
			}
			else
			{
				sprintf(SPHMODE,str_cmd);
				set_config();
				printf("OK\r\n");
			}
		}
	break;
	case 8:
		if(strstr(str_cmd,"?"))
		printf("DEVPID=%s\r\n",DEVPID);
		else
		{
		sprintf(DEVPID,str_cmd);
		set_config();
		printf("OK\r\n");
		}
	break;
	case 9:
		if(strstr(str_cmd,"?"))
		printf("CMD=%s,%s,%s\r\n",command1,command2,command3);
		else
		{
			if((!strstr(str_cmd,"GPIO4"))&&(!strstr(str_cmd,"GPIO5")))
			{
				printf("NOK\r\n");
			}
			else
			{
				PA=strstr(str_cmd,",");
				PB=str_cmd;
				memcpy(command1,str_cmd,PA-PB);
				command1[PA-PB]=0;
				PA+=1;
				PB=strstr(PA,",");
				memcpy(command2,PA,PB-PA);
				command2[PB-PA]=0;
				PB+=1;
				//PA=strstr(PB,"\r\n");
				memcpy(command3,PB,strlen(PB));
				command3[strlen(PB)]=0;
				//printf("command %s",command3);
				//sprintf(command1,str_cmd);
				set_config();
				printf("OK\r\n");
			}
		}
	break;
	case 10:
		if(strstr(str_cmd,"?"))
		printf("LIC=%s\r\n",LICENSE);
		else
		{
			{
				sprintf(LICENSE,str_cmd);
				set_config();
				printf("OK\r\n");
			}
		}
	break;
	case 11:
				sprintf(SSID,"KHUA_0707");
				sprintf(Password,"20070707");
				sprintf(mic_gain,"32");
				sprintf(at_kws,"你好小智");
				sprintf(at_VAD,"ON");
				sprintf(command1,"GPIO4");
				sprintf(command2,"打开灯光");
				sprintf(command3,"关闭灯光");
				sprintf(BaiDu_API_Key,"kwVIQLGBbBmwFxtufuMiBM08");
				sprintf(BaiDu_Secret_Key,"9db9c8d03a4b3b8241f2b54171f2fe54");
				sprintf(SPHMODE,"NORMAL");
				sprintf(DEVPID,"1536");
				sprintf(BaiDu_Access_token,"25.d92da52ba1ce37bad4eed45d4b8fb9ef.315360000.1864211502.282335-10688552");
				//sprintf(LICENSE,"bbb53aed");
				set_config();
				printf("OK\r\n");
	break;
	case 19:
		printf("%s\r\n",REST_VER);
	break;
	case 20:
	printf("**************************BAIDU REST*************************\r\n");
	printf("* AT+MICGAIN [AT+MICGAIN=1-32]                              *\r\n");
	printf("* AT+KWS [AT+KWS=Hello,XiaoDu]                              *\r\n");
	printf("* AT+VAD [AT+VAD=ON/OFF]                                    *\r\n");
	printf("* AT+SPEECH [AT+SPEECH=START/STOP MAX 60S MUST STOP]        *\r\n");
	printf("* AT+SSID [AT+SSID=SSID,PWD]                                *\r\n");
	printf("* AT+CMD [AT+CMD=GPIO4/5,TURN ON,TURN OFF]                  *\r\n");
	printf("* AT+APIKEY [AT+APIKEY=APIKey,SecretKey for BAIDU]          *\r\n");
	printf("* AT+SPHMOD [AT+SPHMOD=FAST/NORMAL DEVPID=80001-FAST]       *\r\n");
	printf("* AT+DEVPID [AT+DEVPID=1536/1537/1737/1637/1837/1936/80001] *\r\n");
	printf("* AT+LIC [AT+LIC=MD5 FROM QQ14510690]                       *\r\n");
	printf("* AT+VER [AT+VER=?]                                         *\r\n");
	printf("*************************************************************\r\n");
	break;
	default:
	break;
	}

	vTaskDelete(NULL);
}
void at_cmd(int8 *cmd)
{
	u8 *PA;
	u8 *PB;
	atcmd_sid=0;
	PA=strstr(cmd,"AT+");
	if(PA==NULL)
	{
		//printf("NOK\r\n");
		//return ;
		PA=cmd;
		PB=strstr(PA,"\r\n");

		sprintf(str_cmd,PB);
		goto OUT;
	}
	PA+=strlen("AT+");
	PB=strstr(PA,"MICGAIN=");
	if(PB)
	{
		atcmd_sid=1;
		PB+=strlen("MICGAIN=");
		//printf("%s",PB);
		sprintf(str_cmd,PB);
		goto OUT;
	}
	PB=strstr(PA,"KWS=");
	if(PB)
	{
		atcmd_sid=2;
		PB+=strlen("KWS=");
		//printf("%s",PB);
		sprintf(str_cmd,PB);
		goto OUT;
	}
	PB=strstr(PA,"VAD=");
	if(PB)
	{
		atcmd_sid=3;
		PB+=strlen("VAD=");
		//printf("%s",PB);
		sprintf(str_cmd,PB);
		goto OUT;
	}
	PB=strstr(PA,"SPEECH=");
	if(PB)
	{
		atcmd_sid=4;
		PB+=strlen("SPEECH=");
		//printf("%s",PB);
		sprintf(str_cmd,PB);
		goto OUT;
	}
	PB=strstr(PA,"SSID=");
	if(PB)
	{
		atcmd_sid=5;
		PB+=strlen("SSID=");
		//printf("%s",PB);
		sprintf(str_cmd,PB);
		goto OUT;
	}
	PB=strstr(PA,"APIKEY=");
	if(PB)
	{
		atcmd_sid=6;
		PB+=strlen("APIKEY=");
		//printf("%s",PB);
		sprintf(str_cmd,PB);
	}
	PB=strstr(PA,"SPHMOD=");
	if(PB)
	{
		atcmd_sid=7;
		PB+=strlen("SPHMOD=");
		//printf("%s",PB);
		sprintf(str_cmd,PB);
		goto OUT;
	}
	PB=strstr(PA,"DEVPID=");
	if(PB)
	{
		atcmd_sid=8;
		PB+=strlen("DEVPID=");
		//printf("%s",PB);
		sprintf(str_cmd,PB);
		goto OUT;
	}
	PB=strstr(PA,"CMD=");
	if(PB)
	{
		atcmd_sid=9;
		PB+=strlen("CMD=");
		//printf("%s",PB);
		sprintf(str_cmd,PB);
		goto OUT;
	}
	PB=strstr(PA,"LIC=");
	if(PB)
	{
		atcmd_sid=10;
		PB+=strlen("LIC=");
		//printf("%s",PB);
		sprintf(str_cmd,PB);
		goto OUT;
	}
	PB=strstr(PA,"FORMAT=");
	if(PB)
	{
		atcmd_sid=11;
		PB+=strlen("FORMAT=");
		//printf("%s",PB);
		sprintf(str_cmd,PB);
		goto OUT;
	}
	PB=strstr(PA,"RESET=");
	if(PB)
	{
		printf("OK\r\n");
		system_restart();
	}
	PB=strstr(PA,"VER=?");
	if(PB)
	{
		atcmd_sid=19;
		PB+=strlen("VER=");
		//printf("%s\r\n",REST_VER);
		sprintf(str_cmd,PB);
		goto OUT;
	}
	PB=strstr(PA,"HELP=?");
	if(PB)
	{
		PB+=strlen("HELP=?");
		atcmd_sid=20;
		sprintf(str_cmd,PB);
		goto OUT;
	}
	else
	{
		sprintf(str_cmd,PA);
	}
OUT:
	xTaskCreate(atcmd_task, "rest_task", 512, NULL, 1, NULL);
}

void audio_init(void)
{

	i2s_audio_init();


}
int GetObject(u8 *my_config,u8 *get_config,u8 *str)
{
	u8 *PA;
	u8 *PB;
		if(strstr(my_config,"%"))
			return -1;
	PA=strstr(my_config,str);
	if(PA==NULL)
		return -1;
	PA=PA+strlen(str)+1;
	PB=strstr(PA,"&");
	if(PB==NULL)
	{
		memcpy(get_config,PA,strlen(PA));
		get_config[strlen(PA)]=0;
		return 1;
	}
	memcpy(get_config,PA,PB-PA);
	get_config[PB-PA]=0;
	return 0;
}
void get_config()
{
	char *PA,*PB;
	char	*my_config	=	(char	*)zalloc(1024);
	char	*temp_config	=	(char	*)zalloc(128);
	os_printf("Boot:0x%x\r\n",system_get_userbin_addr());
	spi_flash_read((add_config)*4096,(uint32 *)my_config,1024);
	if(GetObject(my_config,temp_config,"SSID")>=0)
	{
		temp_config[127]=0;
		PA=strstr(temp_config,",");
		PB=temp_config;
		memcpy(SSID,temp_config,PA-PB);
		SSID[PA-PB]=0;
		PA+=1;
		//PB=strstr(PA,"&");

		memcpy(Password,PA,strlen(PA));
		Password[strlen(PA)]=0;

		os_printf("SSID:%s,%s\r\n",SSID,Password);
	}
	else
	{
		sprintf(SSID,"KHUA_0707");
		sprintf(Password,"20070707");
		os_printf("SSID:%s,%s\r\n",SSID,Password);
	}

	if(GetObject(my_config,temp_config,"MICGAIN")>=0)
	{
		temp_config[15]=0;
		sprintf(mic_gain,temp_config);
		os_printf("MICGAIN:%s\r\n",mic_gain);
	}
	else
	{
		sprintf(mic_gain,"32");
		os_printf("MICGAIN:%s\r\n",mic_gain);
	}

	if(GetObject(my_config,temp_config,"KWS")>=0)
	{
		temp_config[15]=0;
		sprintf(at_kws,temp_config);
		os_printf("KWS:%s\r\n",at_kws);
	}
	else
	{
		sprintf(at_kws,"你好小智");
		os_printf("KWS:%s\r\n",at_kws);
	}

	if(GetObject(my_config,temp_config,"VAD")>=0)
	{
		temp_config[7]=0;
		sprintf(at_VAD,temp_config);
		os_printf("VAD:%s\r\n",at_VAD);
	}
	else
	{
		sprintf(at_VAD,"ON");
		os_printf("VAD:%s\r\n",at_VAD);
	}
	if(GetObject(my_config,temp_config,"CMD")>=0)
	{
		temp_config[63+8]=0;
		PA=strstr(temp_config,",");
		PB=temp_config;
		memcpy(command1,temp_config,PA-PB);
		command1[PA-PB]=0;
		PA+=1;
		PB=strstr(PA,",");
		memcpy(command2,PA,PB-PA);
		command2[PB-PA]=0;
		PB+=1;
		//PA=strstr(PB,"&");
		//if(PA)
		{
		memcpy(command3,PB,strlen(PB));
		command3[strlen(PB)]=0;
		}
		//sprintf(command1,temp_config);
		os_printf("CMD:%s,%s,%s\r\n",command1,command2,command3);
	}
	else
	{
		sprintf(command1,"GPIO4");
		sprintf(command2,"打开电灯");
		sprintf(command3,"关闭电灯");
		os_printf("CMD:%s,%s,%s\r\n",command1,command2,command3);
	}
	if(GetObject(my_config,temp_config,"APIKEY")>=0)
	{
		temp_config[127]=0;
		PA=strstr(temp_config,",");
		PB=temp_config;
		memcpy(BaiDu_API_Key,temp_config,PA-PB);
		BaiDu_API_Key[PA-PB]=0;
		PA+=1;
		//PB=strlen(PA);
		//if(PB>0)
		{
		memcpy(BaiDu_Secret_Key,PA,strlen(PA));
		BaiDu_Secret_Key[strlen(PA)]=0;
//		sprintf(BaiDu_API_Key,temp_config);
//		sprintf(BaiDu_Secret_Key,temp_config+64);
		os_printf("APIKEY:%s,%s\r\n",BaiDu_API_Key,BaiDu_Secret_Key);
		}
	}
	else
	{
		sprintf(BaiDu_API_Key,"kwVIQLGBbBmwFxtufuMiBM08");
		sprintf(BaiDu_Secret_Key,"9db9c8d03a4b3b8241f2b54171f2fe54");
		os_printf("APIKEY:%s,%s\r\n",BaiDu_API_Key,BaiDu_Secret_Key);
	}

	if(GetObject(my_config,temp_config,"TOKEN")>=0)
	{
		temp_config[127]=0;
		sprintf(BaiDu_Access_token,temp_config);
		//sprintf(BaiDu_Access_token,"25.d92da52ba1ce37bad4eed45d4b8fb9e0.315360000.1864211502.282335-10688552");
		os_printf("TOKEN:%s\r\n",BaiDu_Access_token);
	}
	else
	{
		sprintf(BaiDu_Access_token,"25.d92da52ba1ce37bad4eed45d4b8fb9ef.315360000.1864211502.282335-10688552");
		os_printf("TOKEN:%s\r\n",BaiDu_Access_token);
	}

	if(GetObject(my_config,temp_config,"SPHMODE")>=0)
	{
		temp_config[7]=0;
		sprintf(SPHMODE,temp_config);
		os_printf("SPHMODE:%s\r\n",SPHMODE);
	}
	else
	{
		sprintf(SPHMODE,"NORMAL");
		os_printf("SPHMODE:%s\r\n",SPHMODE);
	}

	if(GetObject(my_config,temp_config,"DEVPID")>=0)
	{
		temp_config[15]=0;
		sprintf(DEVPID,temp_config);
		os_printf("DEVPID:%s\r\n",DEVPID);
	}
	else
	{
		sprintf(DEVPID,"1536");
		os_printf("DEVPID:%s\r\n",DEVPID);
	}

	if(GetObject(my_config,temp_config,"LIC")>=0)
	{
		temp_config[15]=0;
		sprintf(LICENSE,temp_config);
		os_printf("LIC:%s\r\n",LICENSE);
	}
	else
	{
		sprintf(LICENSE,"88888888");
		os_printf("LIC:%s\r\n",LICENSE);
	}
	sprintf(speech,"END");
	free(temp_config);
	free(my_config);
}
void set_config()
{
	char	*my_config	=	(char	*)zalloc(1024*2);


	sprintf(my_config,"SSID=%s,%s&MICGAIN=%s&KWS=%s&VAD=%s&CMD=%s,%s,%s&APIKEY=%s,%s&TOKEN=%s&SPHMODE=%s&DEVPID=%s&LIC=%s",\
			SSID,Password,mic_gain,at_kws,at_VAD,command1,command2,command3,BaiDu_API_Key,BaiDu_Secret_Key,BaiDu_Access_token,SPHMODE,DEVPID,LICENSE);
	//printf("set_config %s\r\n",my_config);
	i2s_stop();
	//vTaskDelay(100	/	portTICK_RATE_MS);
	spi_flash_erase_sector(add_config);
	spi_flash_write((add_config)*4096,(uint32 *)my_config,1024);
	i2s_tx_start();
	free(my_config);
}
void config_flash_read()
{
	get_config();
}
/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void wifi_set_config()
{
	//    wifi_set_event_handler_cb(et_wifi_event_cb);
	//printf("connect \r\n");
	  wifi_set_opmode(STATION_MODE);
	  struct station_config * wifi_config = (struct station_config *) zalloc(sizeof(struct station_config));
	  sprintf(wifi_config->ssid, SSID);
	  sprintf(wifi_config->password, Password);
	  wifi_station_set_config(wifi_config);
	  wifi_station_connect();

	  free(wifi_config);
}


void gpio_init()
{
    //gpio16_output_conf();
    gpio16_input_conf();
    //gpio16_output_set(0);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U,FUNC_GPIO0);//
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);//LED
	//PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U,FUNC_GPIO15);//
	if(strstr(command1,"GPIO4"))
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U,FUNC_GPIO4);//
	if(strstr(command1,"GPIO5"))
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U,FUNC_GPIO5);//
	//GPIO_OUTPUT_SET(15, 1);
	//GPIO_DIS_OUTPUT(15);

}

const unsigned int ICACHE_RODATA_ATTR fakePwm[]={ 0x00000010, 0x00000410, 0x00400410, 0x00400C10, 0x00500C10, 0x00D00C10, 0x20D00C10, 0x21D00C10, 0x21D80C10,
	0xA1D80C10, 0xA1D80D10, 0xA1D80D30, 0xA1DC0D30, 0xA1DC8D30, 0xB1DC8D30, 0xB9DC8D30, 0xB9FC8D30, 0xBDFC8D30, 0xBDFE8D30,
	0xBDFE8D32, 0xBDFE8D33, 0xBDFECD33, 0xFDFECD33, 0xFDFECD73, 0xFDFEDD73, 0xFFFEDD73, 0xFFFEDD7B, 0xFFFEFD7B, 0xFFFFFD7B,
	0xFFFFFDFB, 0xFFFFFFFB, 0xFFFFFFFF};

static int sampToI2sPwm(short s) {
	//Okay, when this is enabled it means a speaker is connected *directly* to the data output. Instead of
	//having a nice PCM signal, we fake a PWM signal here.
	static int err=0;
	int samp=s;
	samp=(samp+32768);	//to unsigned
	samp-=err;			//Add the error we made when rounding the previous sample (error diffusion)
	//clip value
	if (samp>65535) samp=65535;
	if (samp<0) samp=0;
	//send pwm value for sample value
	samp=fakePwm[samp>>11];
	err=(samp&0x7ff);	//Save rounding error.
	return samp;
}
static et_uint64 sampToI2sDeltaSigma(short s) {
	int x;
	et_uint64 val=0;
	int w;
	int i1v=0, i2v=0;
	int outReg=0;
	for (x=0; x<64; x++) {
		val<<=1; //next bit
		w=s;
		if (outReg>0) w-=32767; else w+=32767; //Difference 1
		w+=i1v; i1v=w; //Integrator 1
		if (outReg>0) w-=32767; else w+=32767; //Difference 2
		w+=i2v; i2v=w; //Integrator 2
		outReg=w;		//register
		if (w>0) val|=1; //comparator
	}
	return val;
}
et_uint32 APPPDM[32];
et_uint32 APPPCM[16];
int32_t pcm_buf[32];


#define HTONS(A) ((((uint16_t)(A)&0XFF00)>>8)|(((uint16_t)(A)&0X00FF)<<8))
void recode_task(void *pvParameters)
{
	uint32_t index,temp,i,k;
	uint32_t *recod_buff;

	for(;;)
		{
		spiRamFifoWrite((uint8_t *)APPPDM,128);
		printf("RamFifoFree %d\r\n",spiRamFifoFree());
		printf("RamFifoLen %d\r\n",spiRamFifoLen());
		printf("RamGetOverrunCt %d\r\n",spiRamGetOverrunCt());
		printf("RamGetUnderrunCt %d\r\n",spiRamGetUnderrunCt());
//	i2s_clock_set(64000);
//	i2s_rx_start();
	vTaskDelay(100	/	portTICK_RATE_MS);
	spiRamFifoWrite((uint8_t *)APPPDM,128);
	printf("RamFifoFree %d\r\n",spiRamFifoFree());
	printf("RamFifoLen %d\r\n",spiRamFifoLen());
	printf("RamGetOverrunCt %d\r\n",spiRamGetOverrunCt());
	printf("RamGetUnderrunCt %d\r\n",spiRamGetUnderrunCt());
//	i2s_Sample_set(16000);
//	i2s_start();
	vTaskDelay(100	/	portTICK_RATE_MS);

		}
	for(;;)
	{
		xQueueReceive(dmaQueue, &temp, portMAX_DELAY);
		switch (temp)
		{
			case 0:
				recod_buff=i2s_tx_buff1;
			break;
			case 1:
				recod_buff=i2s_tx_buff2;
			break;
			case 2:
				recod_buff=i2s_tx_buff3;
			break;
			default:
			break;
		}
		for(i=0;i<4;i++)
		{
			for(index=0;index<64;index++)
				((uint16_t *)APPPDM)[index]=HTONS(((uint16_t *)recod_buff)[index+i*64]);

			PDM_TO_PCM((uint8_t *)APPPDM,(uint16_t *)APPPCM,&pdm_hander);
			for(k=0;k<16;k++)
				((int16_t *)pcm_buf)[i*16+k]=((uint16_t *)APPPCM)[k*2];
		}
		simpleVAD((int16_t *)pcm_buf,64);
	}

}

void player_task(void *pvParameters)
{
	uint32_t index,temp,i;
	uint32_t *player_buff;
	uint32_t odffset=0;
	int32_t k;
	static int8_t bit=0,bit1=0;
	i2s_Sample_set(32000);
	i2s_tx_start();
	audio_voice_data=1;
	while(1)
	for(i=44;i<wav_len;i+=IIS_RX_BUF_LEN/4)
	{
		xQueueReceive(dmaQueue, &temp, portMAX_DELAY);
		switch (temp)
		{
			case 0:
				player_buff=i2s_tx_buff1;
			break;
			case 1:
				player_buff=i2s_tx_buff2;
			break;
			default:
			break;
		}
		//printf("odffset %d\r\n",wav_add+i);
		spi_flash_read(wav_add+i, (et_uint32 *)player_buff, IIS_RX_BUF_LEN/4);
		for(k=63;k>=0;k--)
		{
			((et_int64 *)player_buff)[k]=sampToI2sDeltaSigma(((et_int16 *)player_buff)[k]);
			//((et_int32 *)player_buff)[k]=sampToI2sPwm(((et_int16 *)player_buff)[k]);
			//((et_int16 *)player_buff)[k*2]=((et_int16 *)player_buff)[k];
		}

	}

}

void main_task(void *pvParameters )
{
	uint32_t index,temp,i,k;
	uint32_t *recod_buff;
	int dummy,vad,value;
	i2s_Sample_set(32000);
	//i2s_stop();
	i2s_rx_start();
	for(;;)
	{
		xQueueReceive(dmaQueue, &temp, portMAX_DELAY);
		switch (temp)
		{
			case 0:
				recod_buff=i2s_tx_buff1;
			break;
			case 1:
				recod_buff=i2s_tx_buff2;
			break;
			case 2:
				recod_buff=i2s_tx_buff3;
			break;
			default:
			break;
		}
		for(i=0;i<4;i++)
		{
			for(index=0;index<64;index++)
				((uint16_t *)APPPDM)[index]=HTONS(((uint16_t *)recod_buff)[index+i*64]);

			PDM_TO_PCM((uint8_t *)APPPDM,(uint16_t *)APPPCM,&pdm_hander);
			for(k=0;k<16;k++)
				((int16_t *)pcm_buf)[i*16+k]=((uint16_t *)APPPCM)[k*2];
		}
		//if(strstr(speech,"START"))
			//if((strstr(speech,"START"))&&(strstr(at_VAD,"OFF")))

		if((strstr(speech,"START"))&&(strstr(at_VAD,"OFF")))
		{

			if(vad_vol.delays++>=40)
			{
				vad_vol.start=1;
				vad_vol.delay1=0;
				//printf("*");
				if(vad_vol.delays>=MAX_TIMES)
				{
					sprintf(speech,"END");
					vad_vol.start=0;
					vad_vol.delays=0;
					GPIO_OUTPUT_SET(2, 1);
				}
				if((wifi_status)&&(vad_vol.delays==41))
				{
					xQueueSend(restQueue,  &dummy, portMAX_DELAY );
					GPIO_OUTPUT_SET(2, 0);
				}
			}
		}
		if((strstr(speech,"STOP"))&&(strstr(at_VAD,"OFF")))
		{
			if(vad_vol.delay1++>100)
			{
			sprintf(speech,"END");
			vad_vol.start=0;
			vad_vol.delays=0;
			GPIO_OUTPUT_SET(2, 1);
			}
		}
		if((strstr(at_VAD,"OFF"))&&(strstr(speech,"END")))
		{
			value = GPIO_INPUT_GET(0);
			if(value==0)
			{
				if(vad_vol.delays++>=40)
				{
					vad_vol.start=1;
					vad_vol.delay1=0;

					if((wifi_status)&&(vad_vol.delays==41))
					{
						xQueueSend(restQueue,  &dummy, portMAX_DELAY );
						GPIO_OUTPUT_SET(2, 0);
					}
					if(vad_vol.delays>=MAX_TIMES)
					{
						vad_vol.start=0;
						vad_vol.delays=0;
						GPIO_OUTPUT_SET(2, 1);
					}
				}
			}
			else
			{
				if(vad_vol.delay1++>100)
				{
				vad_vol.start=0;
				vad_vol.delays=0;
				GPIO_OUTPUT_SET(2, 1);
				}
			}
		}
		if(strstr(at_VAD,"ON"))
		{
			vad=vad_process((int16_t *)pcm_buf,64);
			if((wifi_status)&&(vad==vad_satrt))
			{
				xQueueSend(restQueue,  &dummy, portMAX_DELAY );
				//GPIO_OUTPUT_SET(2, 0);
			}
			if(led_num++>200)
			{
				led_num=0;
				if(wakeup_times--<0)
				{
					wakeup=0;
					wakeup_times=0;
				}
				if(wakeup)
				{
					if(led_stu)
					{

						led_stu=0;
						GPIO_OUTPUT_SET(2, 0);
					}
					else
					{
						led_stu=1;
						GPIO_OUTPUT_SET(2, 1);
					}

				}
				else
					GPIO_OUTPUT_SET(2, 1);
			}
		}
//		if(vad==vad_end)
//			GPIO_OUTPUT_SET(2, 1);

		if(spiRamFifoFree()>=128)
			spiRamFifoWrite((uint8_t *)pcm_buf,128);
		else
		{
			//if(vad_vol.start==0)
			spiRamFifoRead((uint8_t *)APPPDM,128);
			spiRamFifoWrite((uint8_t *)pcm_buf,128);
			//printf("out");
		}
		if((wifi_station_get_connect_status()==STATION_GOT_IP)&&(wifi_status==0))
		{

			if(host_name_statu==0)
			{
				host_name_statu=1;
				int name_ip;
				name_ip=get_host_name_ip("vop.baidu.com");
		     	host_vop_baidu_ip=name_ip;
		     	sprintf(vop_baidu_ip,"%s",inet_ntoa((struct in_addr){name_ip}));
		     	name_ip=get_host_name_ip("openapi.baidu.com");
		     	host_api_baidu_ip=name_ip;
				sprintf(api_baidu_ip,"%s",inet_ntoa((struct in_addr){name_ip}));
				GPIO_OUTPUT_SET(2, 1);

				//xQueueSend(restQueue,  &dummy, portMAX_DELAY );
			}
			wifi_status=1;
			//printf("Wifi connect success\r\n");
		}
	}
	vTaskDelete(NULL);
}
void resolution(char *utf8)
{


	if((strstr(utf8,at_kws))&&(strstr(at_VAD,"ON")))
	{
	wakeup=1;
	wakeup_times=20;
	led_stu=0;
	GPIO_OUTPUT_SET(2, 0);
	}
	if(((strstr(utf8,command2))&&(wakeup))||((strstr(utf8,command2))&&(strstr(at_VAD,"OFF"))))
	{
		if(strstr(command1,"GPIO4"))
		{
		wakeup=0;
		wakeup_times=0;
		GPIO_OUTPUT_SET(4, 1);
		}
		if(strstr(command1,"GPIO5"))
		{
		wakeup=0;
		wakeup_times=0;
		GPIO_OUTPUT_SET(5, 1);
		}
	}
	if(((strstr(utf8,command3))&&(wakeup))||((strstr(utf8,command3))&&(strstr(at_VAD,"OFF"))))
		{
			if(strstr(command1,"GPIO4"))
			{
			wakeup=0;
			wakeup_times=0;
			GPIO_OUTPUT_SET(4, 0);
			}
			if(strstr(command1,"GPIO5"))
			{
			wakeup=0;
			wakeup_times=0;
			GPIO_OUTPUT_SET(5, 0);
			}
		}
}
void rest_task(void *pvParameters )
{
#define recve_len 2048
uint32_t smg;
int	sta_socket;
//uint8_t	*recve_buff	=	(uint8_t	*)malloc(recve_len);

    //sprintf(BaiDu_Access_token,"24.0a576f4d81a31fd908054334a95d66e0.2592000.1574057945.282335-10688552");
	for(;;)
	{
		xQueueReceive(restQueue, &smg, portMAX_DELAY);

		if(!socket_Create(sta_socket,vop_baidu_ip,80))
		{
			//printf("Create_baidu ok\r\n");
			if(!send_baidu(sta_socket))
			{
				//printf("send_baidu ok\r\n");

				if(!recve_baidu(sta_socket,buffer,recve_len))
				{
					//printf("recve_baidu ok\r\n");
					int ret = ojson_rest(buffer);
					if((ret==3302)||(ret==3300))
					{
						//printf("TOKEN FAIL\r\n");
						//i2s_stop();
						if(!socket_Create(sta_socket,api_baidu_ip,80))
						 {

							if(!send_token(sta_socket))
							{
								//printf("send_token ok\r\n");

								if(!recve_baidu(sta_socket,(u8*)buffer,2048))
								{
									//printf("recve_token ok\r\n");
									//printf("%s\r\n",buffer);
									if(ojson_token((u8*)buffer)==0)
									{
										printf("get_token: ok\r\n");
										set_config();

										system_restart();
									}
								}
							}
						 }
					}
					if(!ret)
					{
						utf8togbk(utf8,buffer);
						printf("语音:%s\r\n",utf8);
						resolution(utf8);
					}
					//printf("ESP8266_free_RAM:%d\n",system_get_free_heap_size());
				}

			}
		}
		//vTaskDelay(1	/	portTICK_RATE_MS);
	}
	//free(recve_buff);
}


extern void os_update_cpu_frequency(int mhz);

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
	SET_PERI_REG_MASK(0x3ff00014, BIT(0));
	os_update_cpu_frequency(160);
	uart_init_new();
    printf("SDK version:%s\n", system_get_sdk_version());
    config_flash_read();
//    sprintf(SSID,"KHUA_0707");
//    sprintf(Password,"20070707");
    gpio_init();
    spiRamFifoInit();
    dmaQueue=xQueueCreate( 4, sizeof(uint32_t) );
    restQueue=xQueueCreate( 4, sizeof(uint32_t) );
    //xTaskCreate(recode_task, "recode_task", 512, NULL, 1, NULL);
    //xTaskCreate(player_task, "player_task", 512, NULL, 1, NULL);
    xTaskCreate(main_task, "client_task", 512, NULL, 6, NULL);
    xTaskCreate(rest_task, "rest_task", 512, NULL, 1, NULL);
    PDM_init();
    audio_init();
    wifi_set_config();
    GPIO_OUTPUT_SET(2, 0);
    printf("free_RAM:%d\n",system_get_free_heap_size());
    //main_task();


}

