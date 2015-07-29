#include "PFMeasure.h"


void Init_Iin(uint32_t* ArrayIn)
{
    unsigned short i;
    float fx;
    for(i=0; i<NPT; i++)
    {
        fx = 1500 + 1500 * sin(PI2 * i * 50.0 / Fs);
        *(ArrayIn+i) = ((signed short)fx) << 16;
    }
}

void Init_Vin(uint32_t* ArrayIn)
{
    unsigned short i;
    float fx;
    for(i=0; i<NPT; i++)
    {
        fx = 1500 + 1500 * sin(PI2 * i * 50.0 / Fs + 10);
        *(ArrayIn+i) = ((signed short)fx) << 16;
    }
}

void GetPowerMag(uint32_t* ArrayOut, uint32_t* ArrayIn)
{
    signed short lX,lY;
    float X,Y,Mag;
    uint8_t i;
//		printf("i,\tf,\tMag,\tX,\tY\n");
    for(i=0; i<NPT/2; i++)
    {
        lX = ArrayIn[i] & 0xffff;
        lY = (ArrayIn[i] >> 16) & 0xffff;
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y) / NPT;
        if(i == 0)
            *(ArrayOut+i) = (uint32_t)(Mag * 32768);
        else
            *(ArrayOut+i) = (uint32_t)(Mag * 65536);
//				printf("%d,\t%d,\t%f,\t%f,\t%f\n",i,i*50,Mag,X,Y);
    }
}

float GetBasePercent(uint32_t* ArrayIn)
{
	float valuePercent;
	uint32_t valueBase=0,valueTotal=0,valueTemp=0;
	uint16_t i;
	valueBase=ArrayIn[1];
	for(i=1;i<NPT;i++)
	{
		valueTemp=ArrayIn[i];
		if(valueTemp>=20) valueTotal+=valueTemp;		
	}
	valuePercent=valueBase*1.0/valueTotal;
	return valuePercent;
}
	


float GetPhase(uint32_t* ArrayIn)
{
	uint16_t X,Y;
	float phase;
	X = ArrayIn[1] & 0xffff;
	Y = (ArrayIn[1] >> 16) & 0xffff;
	phase=atan2(Y,X)*180/PI;
	return phase;
	
}

float GetPFValue(float percent, float phase1, float phase2)
{
	float valuePF;
	valuePF=percent*cos(phase1-phase2);
	return valuePF;
}




