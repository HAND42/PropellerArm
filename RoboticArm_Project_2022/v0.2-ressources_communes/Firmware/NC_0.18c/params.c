#include "91x_lib.h"
#include "params.h"


s16 NCParams[255];

void NCParams_Init(void)
{
     NCParams[NCPARAMS_GPS_TARGETSPEED] = 50; // 5.0 m/s
}

u8 NCParams_SetValue(u8 id, s16* pvalue)
{
    NCParams[id] = *pvalue;
    return 1;    
}

u8 NCParams_GetValue(u8 id, s16* pvalue)
{
    if(pvalue == 0) return 0;
    *pvalue = NCParams[id];
    return 1;
}
