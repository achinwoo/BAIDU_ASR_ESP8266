/*
 * rest.h
 *

 */

#ifndef APP_REST_REST_H_
#define APP_REST_REST_H_
#include "esp_common.h"
#include "freertos/queue.h"

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

xQueueHandle restQueue;

#define oem2uni 0x200
#define uni2oem 0x220
char buffer[1024*2];




static uint32_t host_name_statu=0;//
uint32_t host_vop_baidu_ip;
int8_t vop_baidu_ip[32];

u8 api_baidu_ip[32];

u32 host_api_baidu_ip;
uint32_t datalength,datalength1;
int32_t err_no;
char socket_Create(int sta_socket,u8 *SERVER_IP,u16 SERVER_PORT);
int send_baidu(int sta_socket);
int recve_baidu(int sta_socket, char *recv_buf,int len);
int send_token(int sta_socket);
int ojson_rest(u8 *recv_buf);
int ojson_token(u8 *recv_buf);
void utf8togbk(u8 *gbk , u8 *utf);
uint32_t get_host_name_ip(u8 *hostname);


#endif /* APP_REST_REST_H_ */
