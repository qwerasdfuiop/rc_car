#ifndef __FSIA6B_H
#define __FSIA6B_H
#ifdef __cplusplus
	extern "C"{
#endif

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
// 체크섬 계산 함수
unsigned char iBus_Check_CHKSUM(unsigned char * data, unsigned char len);
// 조종기 신호 파싱 함수
void iBus_Parsing(unsigned char* data, FSiA6B_iBus * iBus);
// 조종 수신기로부터 수신되는 조종 송신기 <-> 조종 수신기 간의 fail 정보 확인하는 함수
unsigned char iBus_isActiveFailsafe(FSiA6B_iBus * iBus);

#ifdef __cplusplus
	}
#endif
#endif
