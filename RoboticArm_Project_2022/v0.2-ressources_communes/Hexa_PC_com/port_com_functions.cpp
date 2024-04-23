// Serial.cpp
#include "port_com_functions.h"

port_com::port_com()
{
    memset( &this->m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
    memset( &this->m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );
    this->m_hIDComDev = nullptr;
    this->open = false;
}

port_com::~port_com()
{
    if(this->open)
        this->close();
}

bool port_com::open_port( int nPort, int nBaud )
{
    // vérifier les paramêtres
    assert(nPort > 0);
    assert(nBaud > 0);

    if(this->open)
        return(true);

    wchar_t szPort[15];
    wchar_t szComParams[50];
    DCB dcb;

    wsprintf( szPort, L"COM%d", nPort );
    this->m_hIDComDev = CreateFile( szPort, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, nullptr );
    if( this->m_hIDComDev == nullptr )
        return( false );

    memset( &this->m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
    memset( &this->m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );

    COMMTIMEOUTS CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
    CommTimeOuts.ReadTotalTimeoutConstant = 0;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
    CommTimeOuts.WriteTotalTimeoutConstant = 5000;
    SetCommTimeouts( this->m_hIDComDev, &CommTimeOuts );

    wsprintf( szComParams, L"COM%d:%d,n,8,1", nPort, nBaud );

    this->m_OverlappedRead.hEvent = CreateEvent( nullptr, true, false, nullptr );
    this->m_OverlappedWrite.hEvent = CreateEvent( nullptr, true, false, nullptr );

    dcb.DCBlength = sizeof( DCB );
    GetCommState( this->m_hIDComDev, &dcb );
    dcb.BaudRate = static_cast<unsigned long>(nBaud);
    dcb.ByteSize = 8;
    unsigned char ucSet;
    ucSet = static_cast<unsigned char>( ( FC_RTSCTS & FC_DTRDSR ) != 0 );
    ucSet = static_cast<unsigned char>( ( FC_RTSCTS & FC_RTSCTS ) != 0 );
    ucSet = static_cast<unsigned char>( ( FC_RTSCTS & FC_XONXOFF ) != 0 );
    if( !SetCommState( this->m_hIDComDev, &dcb ) ||
        !SetupComm( this->m_hIDComDev, 10000, 10000 ) ||
        this->m_OverlappedRead.hEvent == nullptr ||
        this->m_OverlappedWrite.hEvent == nullptr )
    {
        //DWORD dwError = GetLastError();
        if( this->m_OverlappedRead.hEvent != nullptr )
            CloseHandle( this->m_OverlappedRead.hEvent );
        if( this->m_OverlappedWrite.hEvent != nullptr )
            CloseHandle( this->m_OverlappedWrite.hEvent );
        CloseHandle( this->m_hIDComDev );
        return( false );
    }

    this->open = true;

    return( this->open );
}

void port_com::close( void )
{
//    if( !this->open || this->m_hIDComDev == nullptr )
//        return( true );

    if( this->m_OverlappedRead.hEvent != nullptr )
        CloseHandle( this->m_OverlappedRead.hEvent );
    if( this->m_OverlappedWrite.hEvent != nullptr )
        CloseHandle( this->m_OverlappedWrite.hEvent );
    CloseHandle( this->m_hIDComDev );
    this->open = false;
    this->m_hIDComDev = nullptr;
}

bool port_com::is_open(void)
{
    return(this->open);
}

unsigned int port_com::ReadAvailable(void* buffer, const unsigned int maxNumToRead)
{
    if(!this->open || this->m_hIDComDev == nullptr )
        return( 0 );

    BOOL bReadStatus;
    DWORD dwBytesRead, dwErrorFlags;
    COMSTAT ComStat;

    ClearCommError( this->m_hIDComDev, &dwErrorFlags, &ComStat );
    if( !ComStat.cbInQue )
        return( 0 );

    dwBytesRead = static_cast<DWORD>(ComStat.cbInQue);
    if( maxNumToRead < static_cast<unsigned int>(dwBytesRead) )
        dwBytesRead = static_cast<DWORD>(maxNumToRead);

    bReadStatus = ReadFile( this->m_hIDComDev, buffer, dwBytesRead, &dwBytesRead, &this->m_OverlappedRead );
    if( !bReadStatus )
    {
        if( GetLastError() == ERROR_IO_PENDING )
        {
            WaitForSingleObject(this->m_OverlappedRead.hEvent, 2000);
            return static_cast<unsigned int>(dwBytesRead);
        }
        return( 0 );
    }

    return static_cast<unsigned int>(dwBytesRead);
}

bool port_com::WriteCommByte(unsigned char ucByte)
{
    BOOL bWriteStat;
    DWORD dwBytesWritten;

    bWriteStat = WriteFile( this->m_hIDComDev, (LPSTR) &ucByte, 1, &dwBytesWritten, &this->m_OverlappedWrite);
    if( !bWriteStat && ( GetLastError() == ERROR_IO_PENDING ) ){
        if(WaitForSingleObject(this->m_OverlappedWrite.hEvent, 1000))
            dwBytesWritten = 0;
        else{
            GetOverlappedResult( this->m_hIDComDev, &this->m_OverlappedWrite, &dwBytesWritten, FALSE );
            this->m_OverlappedWrite.Offset += dwBytesWritten;
            }
        }

    return( TRUE );
}

unsigned int port_com::Write(const unsigned char* buffer, const int size)
{
    if( !this->open || this->m_hIDComDev == nullptr )
        return( 0 );

    DWORD dwBytesWritten = 0;
    int i;
    for( i=0; i<size; i++ )
    {
        this->WriteCommByte( buffer[i] );
        dwBytesWritten++;
    }

    return static_cast<unsigned int>(dwBytesWritten);
}
