#include "port_usb_functions.h"

using namespace port_usb_functions;

// constructeur
port_usb::port_usb()
{
    this->open = false;
}

// destructeur
port_usb::~port_usb()
{
    if(this->open)
    {
        FT_Close(handle);
    }
    DeleteCriticalSection(&cs);
}

bool port_usb::open_port(unsigned int baudrateIn)
{
    int deviceNum = 0;

    vector<DeviceInfo> devices = GetConnectedDeviceInfos();

    if(devices.size() == 0)
    {
        fprintf(stderr, "Error: No FTD2XX device found\n");
        return false;// Error: No FTD2XX device found
    }

    else if(devices.size() == 1)// only one device found, use that directly (more convenient)
    {
        printf("Only one device found - let's use that\n");
        deviceNum = 0;
    }
    else if(devices.size()>1)// more than one found, user should select a device
    {
        printf("Found %d FTD2XX device(s)\n",devices.size());
        printf("==========================\n");
        printf("Device serial numbers:\n");
        for(unsigned int i=0; i<devices.size(); i++)
        {
            printf("%d [%s]",i,devices[i].serialNum.c_str());
        }
        printf("==========================\n");
        printf("PICK A NUMBER:\n");

//        scanf_s("%d",&deviceNum);
        // POUR QT LA FONCTION SCANF_S N'EXISTE PAS
        // FGETS REMPLACE SCANF_S
        char temp_choice[2];
        fgets(temp_choice, sizeof(temp_choice), stdin);
        deviceNum = atoi(temp_choice);

        if(deviceNum<0 || deviceNum>=int(devices.size()))
        {
            fprintf(stderr, "Error: Illegal Choice\n");
            return false;                                            // Error: Illegal choice
        }

    }
    this->start(devices.at(unsigned(deviceNum)),baudrateIn);        // trying to open FTDI port...
    return this->open; //will be zero if failed to open a port
}

// close
void port_usb::close(void)
{
    if(this->open)
    {
        FT_Close(handle);
        this->open = false;
    }
}

// start connection
void port_usb::start(DeviceInfo& devInfo, DWORD baud, DWORD readTimeout, DWORD writeTimeout)
{
    InitializeCriticalSection(&cs);

    if(FT_OK==FT_OpenEx(devInfo.ftinfo.SerialNumber,FT_OPEN_BY_SERIAL_NUMBER,&handle))
    {
        FT_SetTimeouts(handle,readTimeout,writeTimeout);
        FT_SetBaudRate(handle,baud);
        FT_SetDataCharacteristics(handle,FT_BITS_8,FT_STOP_BITS_1,FT_PARITY_NONE);
        this->open = true;
    }
}

bool port_usb::is_open(void)
{
    return this->open;
};

unsigned int port_usb::NumBytesRx(void)
{
    if(!this->open) return 0;
    DWORD dwRxBytes=0;
    this->lock();
    FT_GetQueueStatus(handle,&dwRxBytes);
    this->unlock();
    return unsigned(dwRxBytes);
}

unsigned int port_usb::ReadAvailable(const void* buf, const unsigned int maxNumToRead)
{
    if(!this->open)
        return 0;
    DWORD read=0;
    DWORD dwRxBytes=0;
    this->lock();
    FT_GetQueueStatus(handle,&dwRxBytes);
    if(dwRxBytes>0)
    {
        FT_Read(handle,LPVOID(buf),MIN(dwRxBytes,maxNumToRead),&read);
    }
    this->unlock();
    return unsigned(read);
}

unsigned int port_usb::Read(const void* buf, const unsigned int maxNumToRead)
{
    if(!this->open)
        return 0;
    DWORD read;
    this->lock();
    FT_Read(handle,(void*)buf,maxNumToRead,&read);
    this->unlock();
    return unsigned(read);
}

unsigned int port_usb::Write(const void* buf, const unsigned int size)
{
    if(!this->open)
        return 0;
    DWORD written=0;
    this->lock();
    FT_Write(handle,(void*)buf,size,&written);
    this->unlock();
    return written;
}

void port_usb::lock(void)
{
    EnterCriticalSection(&this->cs);
}

void port_usb::unlock(void)
{
    LeaveCriticalSection(&this->cs);
}

std::vector<DeviceInfo> port_usb::GetConnectedDeviceInfos(void)
{
    DWORD num=0;
    vector<DeviceInfo> ret;

    FT_STATUS ftStatus = FT_CreateDeviceInfoList(&num);
    if(ftStatus!=FT_OK)
    {// error. return empty vector
        return ret;
    }

    FT_DEVICE_LIST_INFO_NODE * buf = new FT_DEVICE_LIST_INFO_NODE[int(num)];

    ftStatus = FT_GetDeviceInfoList(buf,&num);
    if(ftStatus!=FT_OK)
    {// error. return empty vector.
        delete [] buf;
        return ret;
    }

    for(int i=0;i<int(num);i++)
    {
        DeviceInfo n;
        n.ftinfo = buf[i];
        n.serialNum = buf[i].SerialNumber;
        ret.push_back(n);
    }

    delete [] buf;

    return ret;
}

void port_usb::SetLatencyTimer(int msLatency)
{
    FT_SetLatencyTimer(handle,UCHAR(msLatency));
}

/*
MON CODE
#include "portFunctions.h"

using namespace portFunctions;

// constructeur 1
Connection::Connection(DeviceInfo& devInfo, DWORD baud=38400, DWORD readTimeout=5, DWORD writeTimeout=5)
{
    InitializeCriticalSection(&cs);
    this->open = false;

    if(FT_OK==FT_OpenEx(devInfo.ftinfo.SerialNumber,FT_OPEN_BY_SERIAL_NUMBER,&handle))
    {
        this->open = true;
        FT_SetTimeouts(handle,readTimeout,writeTimeout);
        FT_SetBaudRate(handle,baud);
        FT_SetDataCharacteristics(handle,FT_BITS_8,FT_STOP_BITS_1,FT_PARITY_NONE);
    }
}

// constructeur 2
Connection::Connection(string devSerialNum, DWORD baud=38400, DWORD readTimeout=5, DWORD writeTimeout=5)
{
    InitializeCriticalSection(&cs);
    this->open = false;
    if(FT_OK==FT_OpenEx((void*)devSerialNum.c_str(),FT_OPEN_BY_SERIAL_NUMBER,&handle))
    {
        this->open = true;
        FT_SetTimeouts(handle,readTimeout,writeTimeout);
        FT_SetBaudRate(handle,baud);
        FT_SetDataCharacteristics(handle,FT_BITS_8,FT_STOP_BITS_1,FT_PARITY_NONE);
    }
}

// destructeur
Connection::~Connection()
{
    if(this->open)
    {
        FT_Close(handle);
    }
    DeleteCriticalSection(&cs);
}

Connection* Connection::open_port(unsigned int baudrateIn)
{
    int deviceNum = 0;

    vector<DeviceInfo> devices = GetConnectedDeviceInfos();

    if(devices.size() == 0)
    {
        fprintf(stderr, "Error: No FTD2XX device found\n");
        return 0;// Error: No FTD2XX device found
    }

    else if(devices.size() == 1)// only one device found, use that directly (more convenient)
    {
        printf("Only one device found - let's use that\n");
        deviceNum = 0;
    }
    else if(devices.size()>1)// more than one found, user should select a device
    {
        printf("Found %d FTD2XX device(s)\n",devices.size());
        printf("==========================\n");
        printf("Device serial numbers:\n");
        for(unsigned int i=0; i<devices.size(); i++)
        {
            printf("%d [%s]",i,devices[i].serialNum.c_str());
        };
        printf("==========================\n");
        printf("PICK A NUMBER:\n");

        scanf_s("%d",&deviceNum);

        if(deviceNum<0 || deviceNum>=(int)devices.size())
        {
            fprintf(stderr, "Error: Illegal Choice\n");
            return 0;                                            // Error: Illegal choice
        }

    }
    Connection* newPort;
    newPort = new Connection(devices[deviceNum],baudrateIn);        // trying to open FTDI port...
  return newPort; //will be zero if failed to open a port
}

bool Connection::is_open(void)
{
    return this->open;
};

virtual unsigned int Connection::NumBytesRx(void)
{
    if(!this->open) return 0;
    DWORD dwRxBytes=0;
    this->lock();
    FT_GetQueueStatus(handle,&dwRxBytes);
    this->unlock();
    return (unsigned int)dwRxBytes;
}

virtual unsigned int Connection::ReadAvailable(const void* buf, const unsigned int maxNumToRead)
{
    if(!this->open)
        return 0;
    DWORD read=0;
    DWORD dwRxBytes=0;
    this->lock();
    FT_GetQueueStatus(handle,&dwRxBytes);
    if(dwRxBytes>0)
    {
        FT_Read(handle,(LPVOID)buf,MIN(dwRxBytes,maxNumToRead),&read);
    }
    this->unlock();
    return (unsigned int)read;
}

virtual unsigned int Connection::Read(const void* buf, const unsigned int maxNumToRead)
{
    if(!this->open)
        return 0;
    DWORD read;
    this->lock();
    FT_Read(handle,(void*)buf,maxNumToRead,&read);
    this->unlock();
    return (unsigned int)read;
}

virtual unsigned int Connection::Write(const void* buf, const unsigned int size)
{
    if(!this->open)
        return 0;
    DWORD written=0;
    this->lock();
    FT_Write(handle,(void*)buf,size,&written);
    this->unlock();
    return (unsigned int)written;
}

std::vector<DeviceInfo> Connection::GetConnectedDeviceInfos(void)
{
    DWORD num=0;
    vector<DeviceInfo> ret;

    FT_STATUS ftStatus = FT_CreateDeviceInfoList(&num);
    if(ftStatus!=FT_OK)
    {// error. return empty vector
        return ret;
    }

    FT_DEVICE_LIST_INFO_NODE * buf = new FT_DEVICE_LIST_INFO_NODE[(int)num];

    ftStatus = FT_GetDeviceInfoList(buf,&num);
    if(ftStatus!=FT_OK)
    {// error. return empty vector.
        delete [] buf;
        return ret;
    }

    for(int i=0;i<(int)num;i++)
    {
        DeviceInfo n;
        n.ftinfo = buf[i];
        n.serialNum = buf[i].SerialNumber;
        ret.push_back(n);
    }

    delete [] buf;

    return ret;
}

void Connection::SetLatencyTimer(int msLatency)
{
    FT_SetLatencyTimer(handle,(UCHAR) msLatency);
}
*/
/*
#include "portFunctions.h"

std::vector<portFunctions::DeviceInfo> portFunctions::GetConnectedDeviceInfos()
{
    DWORD num=0;
    vector<DeviceInfo> ret;

    FT_STATUS ftStatus = FT_CreateDeviceInfoList(&num);
    if(ftStatus!=FT_OK)
    {// error. return empty vector
        return ret;
    }

    FT_DEVICE_LIST_INFO_NODE * buf = new FT_DEVICE_LIST_INFO_NODE[(int)num];

    ftStatus = FT_GetDeviceInfoList(buf,&num);
    if(ftStatus!=FT_OK)
    {// error. return empty vector.
        delete [] buf;
        return ret;
    }

    for(int i=0;i<(int)num;i++)
    {
        DeviceInfo n;
        n.ftinfo = buf[i];
        n.serialNum = buf[i].SerialNumber;
        ret.push_back(n);
    }

    delete [] buf;

    return ret;
}

portFunctions::Connection* portFunctions::open_port(unsigned int baudrateIn)
{
    int deviceNum = 0;

    vector<DeviceInfo> devices = GetConnectedDeviceInfos();

    if(devices.size() == 0)
    {
        fprintf(stderr, "Error: No FTD2XX device found\n");
        return 0;// Error: No FTD2XX device found
    }

    else if(devices.size() == 1)// only one device found, use that directly (more convenient)
    {
        printf("Only one device found - let's use that\n");
        deviceNum = 0;
    }else if(devices.size()>1)// more than one found, user should select a device
    {
        printf("Found %d FTD2XX device(s)\n",devices.size());
        printf("==========================\n");
        printf("Device serial numbers:\n");
        for(unsigned int i=0; i<devices.size(); i++)
        {
            printf("%d [%s]",i,devices[i].serialNum.c_str());
        };
        printf("==========================\n");
        printf("PICK A NUMBER:\n");

        scanf_s("%d",&deviceNum);

        if(deviceNum<0 || deviceNum>=(int)devices.size())
        {
            fprintf(stderr, "Error: Illegal Choice\n");
            return 0;                                            // Error: Illegal choice
        }

    }
    Connection* newPort;
    newPort = new Connection(devices[deviceNum],baudrateIn);        // trying to open FTDI port...
  return newPort; //will be zero if failed to open a port
}

*/
