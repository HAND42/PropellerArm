#ifndef __GPS_H
#define __GPS_H

#include "waypoints.h"

typedef struct
{
    s16 Nick;
    s16 Roll;
    s16 Yaw;
}  __attribute__((packed)) GPS_Stick_t;

extern GPS_Pos_t GPS_HomePosition;
extern GPS_Pos_t * GPS_pTargetPosition;
extern Waypoint_t* GPS_pWaypoint;

extern GPS_Stick_t GPS_Stick;

void GPS_Navigation(void);
void GPS_Init(void);
extern u8 GPS_CopyPosition(GPS_Pos_t * pGPSPosSrc, GPS_Pos_t* pGPSPosTgt);

#endif //__GPS_H

