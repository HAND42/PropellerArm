#ifndef _PARAMS_H
#define _PARAMS_H

// ids 0...255
#define NCPARAMS_GPS_TARGETSPEED 0

extern void NCParams_Init();
extern u8 NCParams_SetValue(u8 id, s16* pvalue);
extern u8 NCParams_GetValue(u8 id, s16* pvalue);

#endif // _PARAMS_H
