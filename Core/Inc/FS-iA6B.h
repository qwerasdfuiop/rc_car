#ifndef __FSIA6B_H
#define __FSIA6B_H
#ifdef __cplusplus
	extern "C"{
#endif

#include <stdint.h>
#include "stm32f4xx_hal.h"

// 조종 신호 파싱을 위한 구조체
typedef struct _FSiA6B_iBus
{
	unsigned short RH;
	unsigned short RV;
	unsigned short LV;
	unsigned short LH;
	unsigned short SwA;
	unsigned short SwB;
	unsigned short SwC;
	unsigned short SwD;
	unsigned short VrA;
	unsigned short VrB;

	unsigned char FailSafe;
}FSiA6B_iBus;

// 조종 신호 파싱을 위한 버퍼
extern FSiA6B_iBus iBus;
// 조종 통신 이상 fail 플래그
extern unsigned char sigfail_flag;
// 마지막 정상 프레임 수신 시각
extern uint32_t last_ibus_rx_tick;
// 체크섬 계산 함수
unsigned char iBus_Check_CHKSUM(unsigned char * data, unsigned char len);
// 조종기 신호 파싱 함수
void iBus_Parsing(unsigned char* data, FSiA6B_iBus * iBus);
// 조종 수신기로부터 수신되는 조종 송신기 <-> 조종 수신기 간의 fail 정보 확인하는 함수
unsigned char iBus_isActiveFailsafe(FSiA6B_iBus * iBus);
// 조종 통신 타임아웃 확인 함수
void iBus_Check_Timeout(void);

#ifdef __cplusplus
	}
#endif
#endif
