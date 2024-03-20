#include "jetimenu.h"
#include "libfc.h"
#include "printf_P.h"
#include "main.h"
#include "spi.h"
#include "capacity.h"

#define JETIBOX_KEY_RIGHT    0x1F
#define JETIBOX_KEY_UP         0x2F
#define JETIBOX_KEY_DOWN     0x4F
#define JETIBOX_KEY_LEFT    0x8F
#define JETIBOX_KEY_NONE    0x0F
#define JETIBOX_KEY_UNDEF    0x00

#define JetiBox_printfxy(x,y,format, args...)  { LIBFC_JetiBox_SetPos(y * 16 + x); _printf_P(&LIBFC_JetiBox_Putchar, PSTR(format) , ## args);}
#define JetiBox_printf(format, args...)        {  _printf_P(&LIBFC_JetiBox_Putchar, PSTR(format) , ## args);}

// -----------------------------------------------------------
// the menu functions
// -----------------------------------------------------------

void Menu_Status(uint8_t key)
{                        //0123456789ABCDEF
    JetiBox_printfxy(0,0,"%2i.%1iV",UBat/10, UBat%10);
    if(NaviDataOkay)
    {
        JetiBox_printfxy(6,0,"%03dm %03d%c", GPSInfo.HomeDistance/10,GPSInfo.HomeBearing, 0xDF);
    }
    else
    {
        JetiBox_printfxy(6,0,"Status");
    }
    JetiBox_printfxy(0,1,"%4i %2i:%02i",Capacity.UsedCapacity,FlugSekunden/60,FlugSekunden%60);
    if(EE_Parameter.GlobalConfig & CFG_HOEHENREGELUNG)
    {
        JetiBox_printfxy(10,1,"%4im%c", (int16_t)(currentHeight/100),VarioCharacter);
    }
}

void Menu_Attitude(uint8_t key)
{                       //0123456789ABCDEF
    JetiBox_printfxy(0,0,"Nick Roll  Yaw");
    JetiBox_printfxy(0,1,"%4i %4i %4i", (int16_t)(angleIntegral.pitch/1024), (int16_t)(angleIntegral.roll/1024), (int16_t)(ErsatzKompass / GIER_GRAD_FAKTOR));
}

void Menu_Battery(uint8_t key)
{                       //0123456789ABCDEF
    JetiBox_printfxy(0,0,"%2i.%1iV  %3i.%1iA", UBat/10, UBat%10, Capacity.ActualCurrent/10, Capacity.ActualCurrent%10);
    JetiBox_printfxy(0,1,"%4iW %6imAh",Capacity.ActualPower, Capacity.UsedCapacity);
}

void Menu_PosInfo(uint8_t key)
{
    if(NaviDataOkay)
    {
        JetiBox_printfxy(0,0,"Sat:%02d", GPSInfo.NumOfSats);
        switch (GPSInfo.SatFix)
        {
            case SATFIX_NONE:
                JetiBox_printfxy(7,0,"NoFix");
                break;
            case SATFIX_2D:
                JetiBox_printfxy(7,0,"2DFix");
                break;
            case SATFIX_3D:
                JetiBox_printfxy(7,0,"3DFix");
                break;
            default:
                JetiBox_printfxy(7,0,"??Fix");
                break;
        }
        if(GPSInfo.Flags & FLAG_DIFFSOLN)
        {
            JetiBox_printfxy(9,0,"/DGPS");
        }
        JetiBox_printfxy(0,1,"Home:%03dm %03d%c", GPSInfo.HomeDistance/10, GPSInfo.HomeBearing, 0xDF);
    }
    else
    {                     //0123456789ABCDEF
        JetiBox_printfxy(2,0,"No NaviCtrl!");
    }
}


// -----------------------------------------------------------
// the menu topology
// -----------------------------------------------------------
typedef void (*pFctMenu) (uint8_t);  // the menu item handler function pointer

typedef struct{
  int8_t left;
  int8_t right;
  int8_t up;
  int8_t down;
  pFctMenu pHandler;
} MENU_ENTRY;


// the menu navigation structure
/*                        |

3 - 0 - 1 - 2 - 3 - 0

*/

const MENU_ENTRY JetiBox_Menu[] PROGMEM=
{ // l  r  u  d  pHandler
    {3, 1, 0, 0, &Menu_Status },     // 0
    {0, 2, 1, 1, &Menu_Attitude },    // 1
    {1, 3, 2, 2, &Menu_Battery },    // 2
    {2, 0, 3, 3, &Menu_PosInfo },    // 3
};

// -----------------------------------------------------------
// Update display buffer
// -----------------------------------------------------------
unsigned char JetiBox_Update(unsigned char key)
{
    static uint8_t item = 0, last_item = 0; // the menu item

    // navigate within the menu by key action
    last_item = item;
    switch(key)
    {
        case JETIBOX_KEY_LEFT:
            if (item == 0) return (1);                                    // switch back to jeti expander menu
             else item = pgm_read_byte(&JetiBox_Menu[item].left);        //trigger to left menu item
            break;
        case JETIBOX_KEY_RIGHT:
            item = pgm_read_byte(&JetiBox_Menu[item].right);    //trigger to right menu item
            break;
        case JETIBOX_KEY_UP:
            item = pgm_read_byte(&JetiBox_Menu[item].up);        //trigger to up menu item
            break;
        case JETIBOX_KEY_DOWN:
            item = pgm_read_byte(&JetiBox_Menu[item].down);        //trigger to down menu item
            break;
        default:
            break;
    }
    // if the menu item has been changed, do not pass the key to the item handler
    // to avoid jumping over to items
    if(item != last_item) key = JETIBOX_KEY_UNDEF;

    LIBFC_JetiBox_Clear();
    //execute menu item handler
    ((pFctMenu)(pgm_read_word(&(JetiBox_Menu[item].pHandler))))(key);
    
    return (0);
}

