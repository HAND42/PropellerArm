/*#######################################################################################*/
/* !!! THIS IS NOT FREE SOFTWARE !!!                                                       */
/*#######################################################################################*/
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Copyright (c) 2008 Ingo Busker, Holger Buss
// + Nur f�r den privaten Gebrauch / NON-COMMERCIAL USE ONLY
// + FOR NON COMMERCIAL USE ONLY
// + www.MikroKopter.com
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Es gilt f�r das gesamte Projekt (Hardware, Software, Bin�rfiles, Sourcecode und Dokumentation),
// + dass eine Nutzung (auch auszugsweise) nur f�r den privaten (nicht-kommerziellen) Gebrauch zul�ssig ist.
// + Sollten direkte oder indirekte kommerzielle Absichten verfolgt werden, ist mit uns (info@mikrokopter.de) Kontakt
// + bzgl. der Nutzungsbedingungen aufzunehmen.
// + Eine kommerzielle Nutzung ist z.B.Verkauf von MikroKoptern, Best�ckung und Verkauf von Platinen oder Baus�tzen,
// + Verkauf von Luftbildaufnahmen, usw.
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Werden Teile des Quellcodes (mit oder ohne Modifikation) weiterverwendet oder ver�ffentlicht,
// + unterliegen sie auch diesen Nutzungsbedingungen und diese Nutzungsbedingungen incl. Copyright m�ssen dann beiliegen
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Sollte die Software (auch auszugesweise) oder sonstige Informationen des MikroKopter-Projekts
// + auf anderen Webseiten oder sonstigen Medien ver�ffentlicht werden, muss unsere Webseite "http://www.mikrokopter.de"
// + eindeutig als Ursprung verlinkt werden
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Keine Gew�hr auf Fehlerfreiheit, Vollst�ndigkeit oder Funktion
// + Benutzung auf eigene Gefahr
// + Wir �bernehmen keinerlei Haftung f�r direkte oder indirekte Personen- oder Sachsch�den
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Die Portierung oder Nutzung der Software (oder Teile davon) auf andere Systeme (ausser der Hardware von www.mikrokopter.de) ist nur
// + mit unserer Zustimmung zul�ssig
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Die Funktion printf_P() unterliegt ihrer eigenen Lizenz und ist hiervon nicht betroffen
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Redistributions of source code (with or without modifications) must retain the above copyright notice,
// + this list of conditions and the following disclaimer.
// +   * Neither the name of the copyright holders nor the names of contributors may be used to endorse or promote products derived
// +     from this software without specific prior written permission.
// +   * The use of this project (hardware, software, binary files, sources and documentation) is only permitted
// +     for non-commercial use (directly or indirectly)
// +     Commercial use (for excample: selling of MikroKopters, selling of PCBs, assembly, ...) is only permitted
// +     with our written permission
// +   * If sources or documentations are redistributet on other webpages, out webpage (http://www.MikroKopter.de) must be
// +     clearly linked as origin
// +   * porting the sources to other systems or using the software on other systems (except hardware from www.mikrokopter.de) is not allowed
//
// +  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// +  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// +  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// +  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// +  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// +  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// +  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// +  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// +  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// +  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// +  POSSIBILITY OF SUCH DAMAGE.
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "91x_lib.h"
#include "uart1.h"
#include "ubx.h"
#include "led.h"
#include "timer1.h"

// ------------------------------------------------------------------------------------------------
// defines

#define DAYS_FROM_JAN01YEAR0001_TO_JAN6_1980 722819 // the year 0 does not exist!
#define DAYS_PER_YEAR         365
#define DAYS_PER_LEAPYEAR    366
#define DAYS_PER_4YEARS        1461     //((3 * DAYS_PER_YEAR) + DAYS_PER_LEAPYEAR) // years dividable by 4 are leap years
#define DAYS_PER_100YEARS    36524     //((25 * DAYS_PER_4YEARS) - 1) // years dividable by 100 are no leap years
#define DAYS_PER_400YEARS    146097    //((4 * DAYS_PER_100YEARS) + 1L) // but years dividable by 400 are leap years
#define SECONDS_PER_MINUTE    60
#define MINUTES_PER_HOUR    60
#define    HOURS_PER_DAY        24
#define DAYS_PER_WEEK        7
#define SECONDS_PER_HOUR    3600    //(SECONDS_PER_MINUTE * MINUTES_PER_HOUR)
#define SECONDS_PER_DAY        86400    //(SECONDS_PER_HOUR * HOURS_PER_DAY)
#define SECONDS_PER_WEEK    604800  //(SECONDS_PER_DAY * DAYS_PER_WEEK)

// days per month in normal and leap years
const u32     Leap[ 13 ]     = { 0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };
const u32 Normal[ 13 ]    = { 0,  31,  59,  90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

#define LEAP_SECONDS_FROM_1980    13

// message sync bytes
#define    UBX_SYNC1_CHAR    0xB5
#define    UBX_SYNC2_CHAR    0x62
// protocoll identifier
#define    UBX_CLASS_NAV    0x01
// message id
#define    UBX_ID_POSLLH    0x02
#define UBX_ID_SOL        0x06
#define    UBX_ID_VELNED    0x12

// ------------------------------------------------------------------------------------------------
// typedefs


// ubx parser state
typedef enum
{
    UBXSTATE_IDLE,
    UBXSTATE_SYNC1,
    UBXSTATE_SYNC2,
    UBXSTATE_CLASS,
    UBXSTATE_LEN1,
    UBXSTATE_LEN2,
    UBXSTATE_DATA,
    UBXSTATE_CKA,
    UBXSTATE_CKB
} ubxState_t;

typedef struct
{
    u32            itow;        // ms GPS Millisecond Time of Week
    s32            frac;        // ns remainder of rounded ms above
    s16            week;        // GPS week
    u8            GPSfix;        // GPSfix Type, range 0..6
    u8            Flags;        // Navigation Status Flags
    s32            ECEF_X;        // cm ECEF X coordinate
    s32            ECEF_Y;        // cm ECEF Y coordinate
    s32            ECEF_Z;        // cm ECEF Z coordinate
    u32            PAcc;        // cm 3D Position Accuracy Estimate
    s32            ECEFVX;        // cm/s ECEF X velocity
    s32            ECEFVY;        // cm/s ECEF Y velocity
    s32            ECEFVZ;        // cm/s ECEF Z velocity
    u32            SAcc;        // cm/s Speed Accuracy Estimate
    u16            PDOP;        // 0.01 Position DOP
    u8            res1;        // reserved
    u8            numSV;        // Number of SVs used in navigation solution
    u32            res2;        // reserved
    u8            Status;        // invalid/newdata/processed
} __attribute__((packed)) ubx_nav_sol_t;


typedef struct
{
    u32            itow;          // ms  GPS Millisecond Time of Week
    s32            VEL_N;         // cm/s  NED north velocity
    s32            VEL_E;         // cm/s  NED east velocity
    s32            VEL_D;         // cm/s  NED down velocity
    u32            Speed;         // cm/s  Speed (3-D)
    u32            GSpeed;     // cm/s  Ground Speed (2-D)
    s32            Heading;     // 1e-05 deg  Heading 2-D
    u32            SAcc;        // cm/s  Speed Accuracy Estimate
    u32            CAcc;         // deg  Course / Heading Accuracy Estimate
    u8            Status;        // invalid/newdata/processed
} __attribute__((packed)) ubx_nav_velned_t;

typedef struct
{
    u32            itow;        // ms GPS Millisecond Time of Week
    s32            LON;        // 1e-07 deg Longitude
    s32            LAT;        // 1e-07 deg Latitude
    s32            HEIGHT;        // mm Height above Ellipsoid
    s32            HMSL;        // mm Height above mean sea level
    u32            Hacc;        // mm Horizontal Accuracy Estimate
    u32            Vacc;        // mm Vertical Accuracy Estimate
    u8            Status;        // invalid/newdata/processed
} __attribute__((packed)) ubx_nav_posllh_t;



//------------------------------------------------------------------------------------
// global variables

// local buffers for the incomming ubx messages
volatile ubx_nav_sol_t        UbxSol      = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, INVALID};
volatile ubx_nav_posllh_t    UbxPosLlh = {0,0,0,0,0,0,0, INVALID};
volatile ubx_nav_velned_t    UbxVelNed = {0,0,0,0,0,0,0,0,0, INVALID};

// shared buffer
gps_data_t          GPSData = {{0,0,0,INVALID},0,0,0,0,0,0,0, INVALID};
DateTime_t             GPSDateTime = {0,0,0,0,0,0,0, INVALID};

#define UBX_TIMEOUT 500 // 500 ms
u32 UBX_Timeout = 0;

//------------------------------------------------------------------------------------
// functions

u8 IsLeapYear(u16 year)
{
    if((year%400 == 0) || ( (year%4 == 0) && (year%100 != 0) ) ) return 1;
    else return 0;
}
/********************************************************/
/*  Calculates the UTC Time from the GPS week and tow   */
/********************************************************/
void SetGPSTime(DateTime_t * pTimeStruct)
{
    u32 Days, Seconds, Week;
    u16 YearPart;
    u32 * MonthDayTab = 0;
    u8 i;


    // if GPS data show valid time data
    if((UbxSol.Status != INVALID) && (UbxSol.Flags & FLAG_WKNSET) && (UbxSol.Flags & FLAG_TOWSET) )
    {
        Seconds = UbxSol.itow / 1000L;
        Week = (u32)UbxSol.week;
        // correct leap seconds since 1980
        if(Seconds < LEAP_SECONDS_FROM_1980)
        {
            Week--;
             Seconds = SECONDS_PER_WEEK - LEAP_SECONDS_FROM_1980 + Seconds;
        }
        else Seconds -= LEAP_SECONDS_FROM_1980;

         Days = DAYS_FROM_JAN01YEAR0001_TO_JAN6_1980;
        Days += (Week * DAYS_PER_WEEK);
        Days += Seconds / SECONDS_PER_DAY; // seperate days from GPS seconds of week

        pTimeStruct->Year = 1;
        YearPart = (u16)(Days / DAYS_PER_400YEARS);
        pTimeStruct->Year += YearPart * 400;
        Days = Days % DAYS_PER_400YEARS;
        YearPart = (u16)(Days / DAYS_PER_100YEARS);
        pTimeStruct->Year += YearPart * 100;
        Days = Days % DAYS_PER_100YEARS;
        YearPart = (u16)(Days / DAYS_PER_4YEARS);
        pTimeStruct->Year += YearPart * 4;
        Days = Days % DAYS_PER_4YEARS;
        if(Days < (3* DAYS_PER_YEAR)) YearPart = (u16)(Days / DAYS_PER_YEAR);
        else YearPart = 3;
        pTimeStruct->Year += YearPart;
        // calculate remaining days of year
        Days -= (u32)(YearPart *  DAYS_PER_YEAR);
        Days += 1;
        // check if current year is a leap year
        if(IsLeapYear(pTimeStruct->Year)) MonthDayTab = (u32*)Leap;
        else MonthDayTab = (u32*)Normal;
        // seperate month and day from days of year
        for ( i = 0; i < 12; i++ )
        {
            if ( (MonthDayTab[i]< Days) && (Days <= MonthDayTab[i+1]) )
            {
                pTimeStruct->Month = i+1;
                pTimeStruct->Day = Days - MonthDayTab[i];
                i = 12;
            }
        }
        Seconds = Seconds % SECONDS_PER_DAY; // remaining seconds of current day
        pTimeStruct->Hour = (u8)(Seconds / SECONDS_PER_HOUR);
        Seconds = Seconds % SECONDS_PER_HOUR; // remaining seconds of current hour
        pTimeStruct->Min = (u8)(Seconds / SECONDS_PER_MINUTE);
        Seconds = Seconds % SECONDS_PER_MINUTE; // remaining seconds of current minute
        pTimeStruct->Sec = (u8)(Seconds);
        pTimeStruct->mSec  = (u16)(UbxSol.itow % 1000L);
        pTimeStruct->Valid = 1;
    }
    else
    {
        pTimeStruct->Valid = 0;
    }
}



/********************************************************/
/*                  Initialize UBX Parser               */
/********************************************************/
void UBX_Init(void)
{
    // mark msg buffers invalid
    UbxSol.Status = INVALID;
    UbxPosLlh.Status = INVALID;
    UbxVelNed.Status = INVALID;
    GPSData.Status = INVALID;

    UBX_Timeout = SetDelay(2 * UBX_Timeout);
}

/********************************************************/
/*            Upate GPS data stcructure                 */
/********************************************************/
void Update_GPSData (void)
{
    static u32 Msg_Count_Timeout = 0;
    static u8 Msg_Count = 0;

    // the timeout is used to detect the delay between two message sets
    // and is used for synchronisation so that always a set is collected
    // that belongs together
    // _______NAVSOL|POSLLH|VELNED|___________________NAVSOL|POSLLH|VELNED|_____________
    //              |  8ms | 8ms  |         184 ms          |      |      |
    // msg_count:   0      1      2                         0      1      2

    if(CheckDelay(Msg_Count_Timeout))    Msg_Count = 0;
    else Msg_Count++;
    Msg_Count_Timeout = SetDelay(100); // reset ubx msg timeout

    // if a new set of ubx messages was collected
    if((Msg_Count >= 2))
    {    // if set is complete
        if((UbxSol.Status == NEWDATA) && (UbxPosLlh.Status == NEWDATA) && (UbxVelNed.Status == NEWDATA))
        {
            UBX_Timeout = SetDelay(UBX_TIMEOUT);
            DebugOut.Analog[9]++;
            // update GPS data only if the status is INVALID or PROCESSED  and the last ubx message was received within less than 100 ms
            if(GPSData.Status != NEWDATA) // if last data were processed
            { // wait for new data at all neccesary ubx messages
                GPSData.Status = INVALID;
                // NAV SOL
                GPSData.Flags =                    UbxSol.Flags;
                GPSData.NumOfSats =             UbxSol.numSV;
                GPSData.SatFix =                 UbxSol.GPSfix;
                GPSData.Position_Accuracy =        UbxSol.PAcc;
                GPSData.Speed_Accuracy =         UbxSol.SAcc;
                SetGPSTime(&SystemTime); // update system time
                // NAV POSLLH
                GPSData.Position.Status =         INVALID;
                GPSData.Position.Longitude =      UbxPosLlh.LON;
                GPSData.Position.Latitude =      UbxPosLlh.LAT;
                GPSData.Position.Altitude =      UbxPosLlh.HMSL;
                GPSData.Position.Status =         NEWDATA;
                // NAV VELNED
                GPSData.Speed_East =             UbxVelNed.VEL_E;
                GPSData.Speed_North =             UbxVelNed.VEL_N;
                GPSData.Speed_Top     =             -UbxVelNed.VEL_D;
                GPSData.Speed_Ground =             UbxVelNed.GSpeed;
                GPSData.Heading =                 UbxVelNed.Heading;

                GPSData.Status = NEWDATA; // new data available
            } // EOF if(GPSData.Status != NEWDATA)
        } // EOF all ubx messages received
        // set state to collect new data
        UbxSol.Status =                 PROCESSED;    // ready for new data
        UbxPosLlh.Status =                 PROCESSED;    // ready for new data
        UbxVelNed.Status =                 PROCESSED;    // ready for new data
    }
}


/********************************************************/
/*                   UBX Parser                         */
/********************************************************/
void UBX_Parser(u8 c)
{
    static ubxState_t ubxState = UBXSTATE_IDLE;
    static u16 msglen;
    static u8 cka, ckb;
    static u8 *ubxP, *ubxEp, *ubxSp; // pointers to data currently transfered


    //state machine
    switch (ubxState)    // ubx message parser
    {
        case UBXSTATE_IDLE: // check 1st sync byte
            if (c == UBX_SYNC1_CHAR) ubxState = UBXSTATE_SYNC1;
            else ubxState = UBXSTATE_IDLE; // out of synchronization
            break;

        case UBXSTATE_SYNC1: // check 2nd sync byte
            if (c == UBX_SYNC2_CHAR) ubxState = UBXSTATE_SYNC2;
            else ubxState = UBXSTATE_IDLE; // out of synchronization
            break;

        case UBXSTATE_SYNC2: // check msg class to be NAV
            if (c == UBX_CLASS_NAV) ubxState = UBXSTATE_CLASS;
            else ubxState = UBXSTATE_IDLE; // unsupported message class
            break;

        case UBXSTATE_CLASS: // check message identifier
            switch(c)
            {
                case UBX_ID_POSLLH: // geodetic position
                    ubxP =  (u8 *)&UbxPosLlh; // data start pointer
                    ubxEp = (u8 *)(&UbxPosLlh + 1); // data end pointer
                    ubxSp = (u8 *)&UbxPosLlh.Status; // status pointer
                    break;

                case UBX_ID_SOL: // navigation solution
                    ubxP =  (u8 *)&UbxSol; // data start pointer
                    ubxEp = (u8 *)(&UbxSol + 1); // data end pointer
                    ubxSp = (u8 *)&UbxSol.Status; // status pointer
                    break;

                case UBX_ID_VELNED: // velocity vector in tangent plane
                    ubxP =  (u8 *)&UbxVelNed; // data start pointer
                    ubxEp = (u8 *)(&UbxVelNed + 1); // data end pointer
                    ubxSp = (u8 *)&UbxVelNed.Status; // status pointer
                    break;

                default:            // unsupported identifier
                    ubxState = UBXSTATE_IDLE;
                    break;
            }
            if (ubxState != UBXSTATE_IDLE)
            {
                ubxState = UBXSTATE_LEN1;
                cka = UBX_CLASS_NAV + c;
                ckb = UBX_CLASS_NAV + cka;
            }
            break;

        case UBXSTATE_LEN1: // 1st message length byte
            msglen = (u16)c; // lowbyte first
            cka += c;
            ckb += cka;
            ubxState = UBXSTATE_LEN2;
            break;

        case UBXSTATE_LEN2: // 2nd message length byte
            msglen += ((u16)c)<<8; // high byte last
            cka += c;
            ckb += cka;
            // if the old data are not processed so far then break parsing now
            // to avoid writing new data in ISR during reading by another function
            if ( *ubxSp == NEWDATA )
            {
                ubxState = UBXSTATE_IDLE;
                Update_GPSData(); //update GPS info respectively
            }
            else // data invalid or allready processd
            {
                *ubxSp = INVALID; // mark invalid during buffer filling
                ubxState = UBXSTATE_DATA;
            }
            break;

        case UBXSTATE_DATA: // collecting data
            if (ubxP < ubxEp)
            {
                *ubxP++ = c; // copy curent data byte if any space is left
                cka += c;
                ckb += cka;
                if (--msglen == 0)     ubxState = UBXSTATE_CKA; // switch to next state if all data was read
            }
            else // rx buffer overrun
            {
                ubxState = UBXSTATE_IDLE;
            }
            break;

        case UBXSTATE_CKA:
            if (c == cka) ubxState = UBXSTATE_CKB;
            else
            {
                *ubxSp = INVALID;
                ubxState = UBXSTATE_IDLE;
            }
            break;

        case UBXSTATE_CKB:
            if (c == ckb)
            {
                *ubxSp = NEWDATA; // new data are valid
                Update_GPSData(); //update GPS info respectively
            }
            else
            {    // if checksum not match then set data invalid
                *ubxSp = INVALID;
            }
            ubxState = UBXSTATE_IDLE; // ready to parse new data
            break;

        default: // unknown ubx state
            ubxState = UBXSTATE_IDLE;
            break;

    }
}
