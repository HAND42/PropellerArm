#ifndef _WAYPOINTS_H
#define _WAYPOINTS_H

#include "ubx.h"

typedef struct
{
     GPS_Pos_t Position;        // the gps position of the waypoint, see ubx.h for details
    s16 Heading;            // orientation, future implementation
    u8  ToleranceRadius;    // in meters, if the MK is within that range around the target, then the next target is triggered
    u8  HoldTime;            // in seconds, if the was once in the tolerance area around a WP, this time defines the delay before the next WP is triggered
    u8  Event_Flag;            // future implementation
    u8  Index;              // to indentify different waypoints, workaround for bad communications PC <-> NC
    u8  reserve[11];        // reserve
} __attribute__((packed)) Waypoint_t;

u8 WPList_Init(void);
u8 WPList_Clear(void);
u8 WPList_GetCount(void);
u8 WPList_Append(Waypoint_t* pwp);
Waypoint_t* WPList_Begin(void);
Waypoint_t* WPList_End(void);
Waypoint_t* WPList_Next(void);
Waypoint_t* WPList_GetAt(u8 index);

#endif // _WAYPOINTS_H
