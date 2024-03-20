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
     
#include <string.h>
#include "91x_lib.h"
#include "waypoints.h"
#include "uart1.h"

// the waypoints list
#define WPLISTLEN 20
Waypoint_t WPList[WPLISTLEN];
u8 WPIndex = 0;
u8 WPNumber = 0;


u8 WPList_Init(void)
{
     return WPList_Clear();
}

u8 WPList_Clear(void)
{
    u8 i;
    WPIndex = 0;
    WPNumber = 0;
    NaviData.WaypointNumber = WPNumber;
    NaviData.WaypointIndex = WPIndex;

    for(i = 0; i < WPLISTLEN; i++)
    {
        WPList[i].Position.Status = INVALID;
        WPList[i].Position.Latitude = 0;
        WPList[i].Position.Longitude = 0;
        WPList[i].Position.Altitude = 0;
        WPList[i].Heading = -1;
        WPList[i].ToleranceRadius = 0;    // in meters, if the MK is within that range around the target, then the next target is triggered
        WPList[i].HoldTime = 0;            // in seconds, if the was once in the tolerance area around a WP, this time defines the delay before the next WP is triggered
        WPList[i].Event_Flag = 0;        // future implementation
    }
    return TRUE;        
}

u8 WPList_GetCount(void)
{
     return WPNumber;
}

u8 WPList_Append(Waypoint_t* pwp)
{
     if(WPNumber < WPLISTLEN) // id there is still some space in the list
    {
        memcpy(&WPList[WPNumber], pwp, sizeof(Waypoint_t)); // copy wp data to list entry
        WPList[WPNumber].Position.Status = NEWDATA;         // mark as new data
         WPNumber++;                                         // increment list length
        NaviData.WaypointNumber = WPNumber;
        return TRUE;
    }
    else return FALSE;
}

// rewind to the begin of the list, and returns the first waypoint
Waypoint_t* WPList_Begin(void)
{
     WPIndex = 0; // reset list index
    NaviData.WaypointIndex = WPIndex + 1;
    if(WPNumber > 0) 
    {
        NaviData.WaypointIndex = WPIndex + 1;    
        return(&(WPList[WPIndex])); // if list is not empty return pointer to first waypoint in the list
    }
    else
    {
        NaviData.WaypointIndex = 0;
        return NULL; // else return NULL
    }

}

// jump to the end of the list, and returns the last waypoint
Waypoint_t* WPList_End(void)
{
    if(WPNumber > 0)
    {
        NaviData.WaypointIndex = WPNumber;
        WPIndex = WPNumber - 1;
        return(&(WPList[WPIndex])); // if list is not empty return pointer to first waypoint in the list
    }
    else
    {
        return NULL; // else return NULL
    }

}

// returns a pointer to the next waypoint or NULL if the end of the list has been reached
Waypoint_t* WPList_Next(void)
{
    if((WPIndex + 1) < WPNumber) // if the next WPIndex exist
    {
        WPIndex++; // goto next
        NaviData.WaypointIndex = WPIndex + 1;
        return(&(WPList[WPIndex]));    // return pointer to this waypoint
    }
    else return(NULL);
}    
 
Waypoint_t* WPList_GetAt(u8 index)
{
    if(index < WPNumber) return(&(WPList[index]));    // return pointer to this waypoint
    else return(NULL);
}
