// Serial.h

#ifndef PORT_COM_FUNCTIONS_H
#define PORT_COM_FUNCTIONS_H

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

#include <windows.h>
#include <assert.h>

class port_com
{
    public:
        port_com();
        ~port_com();

        bool open_port(int nPort, int nBaud);
        void close( void );
        bool is_open(void);
        unsigned int ReadAvailable(void*, const unsigned int);
        unsigned int Write(const unsigned char*, const int);

    private:
        HANDLE m_hIDComDev;
        OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
        bool open;
        bool WriteCommByte(unsigned char);

};

#endif
