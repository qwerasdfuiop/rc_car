#include "FS-iA6B.h"

FSiA6B_iBus iBus;
// 체크섬 계산 함수
unsigned char iBus_Check_CHKSUM(unsigned char * data, unsigned char len)
{
	unsigned short chksum = 0xffff;

	for(int i=0; i<len-2;i++){
		chksum = chksum - data[i];
	}

	return ((chksum&0x00ff)==data[30]) && ((chksum>>8)==data[31]);
}
// 조종기 신호 파싱 함수
void iBus_Parsing(unsigned char* data, FSiA6B_iBus * iBus)
{
	iBus->RH = (data[2] | data[3]<<8) & 0x0fff;
	iBus->RV = (data[4] | data[5]<<8) & 0x0fff;
	iBus->LV = (data[6] | data[7]<<8) & 0x0fff;
	iBus->LH = (data[8] | data[9]<<8) & 0x0fff;
	iBus->SwA = (data[10] | data[11]<<8) & 0x0fff;
	iBus->SwC = (data[12] | data[13]<<8) & 0x0fff;

	iBus->FailSafe = (data[13] >> 4);
}
// 조종 수신기로부터 수신되는 조종 송신기 <-> 조종 수신기 간의 fail 정보 확인하는 함수
unsigned char iBus_isActiveFailsafe(FSiA6B_iBus * iBus)
{
	return iBus->FailSafe != 0;
}
