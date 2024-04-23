#ifndef PORT_USB_FUNCTIONS_H
#define PORT_USB_FUNCTIONS_H

#include "ftd2xx.h"
#include <vector>
#include <string>
#include <assert.h>

using namespace std;

namespace port_usb_functions
{
    #ifndef MIN
    #define MIN(a,b) (((a)<(b))?(a):(b))
    #endif


    struct DeviceInfo
    {
        FT_DEVICE_LIST_INFO_NODE ftinfo;
        string serialNum;
    };

    class port_usb 
    {
        public:
            port_usb();
            ~port_usb();

            bool open_port(unsigned int);
            void close(void);
            bool is_open(void);
            unsigned int NumBytesRx(void);
            unsigned int ReadAvailable(const void*, const unsigned int);
            unsigned int Read(const void*, const unsigned int);
            unsigned int Write(const void*, const unsigned int);

            void SetLatencyTimer(int);

        private:
            void start(DeviceInfo& , DWORD baud=38400, DWORD readTimeout = 5, DWORD writeTimeout = 5);
            CRITICAL_SECTION cs;
            void lock(void);
            void unlock(void);

            vector<DeviceInfo> GetConnectedDeviceInfos(void);
            bool open;
            FT_HANDLE handle;
    }; // end class
} // end namespace
#endif

/*
#ifndef PORTFUNCTIONS_H
#define PORTFUNCTIONS_H

#include "ftd2xx.h"
#include <vector>
#include <string>

namespace portFunctions
{
  using namespace std;

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif


  inline int NumConnected(){
          DWORD num=0;
          FT_ListDevices(&num,NULL,FT_LIST_NUMBER_ONLY);
          return (int)num;
      }

  struct DeviceInfo{
          FT_DEVICE_LIST_INFO_NODE ftinfo;
          string serialNum;
      };

  inline vector<DeviceInfo> GetConnectedDeviceInfos();

  class Connection 
  {
      public:
      Connection(DeviceInfo& devInfo, DWORD baud=38400, DWORD readTimeout=5, DWORD writeTimeout=5)
      {
          InitializeCriticalSection(&cs);
          open = false;
          
          if(FT_OK==FT_OpenEx(devInfo.ftinfo.SerialNumber,FT_OPEN_BY_SERIAL_NUMBER,&handle))
          {
              open = true;
              FT_SetTimeouts(handle,readTimeout,writeTimeout);
              FT_SetBaudRate(handle,baud);
              FT_SetDataCharacteristics(handle,FT_BITS_8,FT_STOP_BITS_1,FT_PARITY_NONE);
          }
      }

      Connection(string devSerialNum, DWORD baud=38400, DWORD readTimeout=5, DWORD writeTimeout=5)
      {
          InitializeCriticalSection(&cs);
          open = false;
          if(FT_OK==FT_OpenEx((void*)devSerialNum.c_str(),FT_OPEN_BY_SERIAL_NUMBER,&handle))
          {
              open = true;
              FT_SetTimeouts(handle,readTimeout,writeTimeout);
              FT_SetBaudRate(handle,baud);
              FT_SetDataCharacteristics(handle,FT_BITS_8,FT_STOP_BITS_1,FT_PARITY_NONE);
          }
      }

      virtual unsigned int NumBytesRx()
      {
          if(!open) return 0;
          DWORD dwRxBytes=0;
          lock();
          FT_GetQueueStatus(handle,&dwRxBytes);
          unlock();
          return (unsigned int)dwRxBytes;
      }

      virtual unsigned int ReadAvailable(const void* buf, const unsigned int maxNumToRead)
      {
          if(!open) return 0;
          DWORD read=0;
          DWORD dwRxBytes=0;
          lock();
          FT_GetQueueStatus(handle,&dwRxBytes);
          if(dwRxBytes>0)
          {
              FT_Read(handle,(LPVOID)buf,MIN(dwRxBytes,maxNumToRead),&read);
          }
          unlock();
          return (unsigned int)read;
      }

      virtual unsigned int Read(const void* buf, const unsigned int maxNumToRead)
      {
          if(!open) return 0;
          DWORD read;
          lock();
          FT_Read(handle,(void*)buf,maxNumToRead,&read);
          unlock();
          return (unsigned int)read;
      }

      virtual unsigned int Write(const void* buf, const unsigned int size)
      {
          if(!open) return 0;
          DWORD written=0;
          lock();
          FT_Write(handle,(void*)buf,size,&written);
          unlock();
          return (unsigned int)written;
      }

      ~Connection()
      {
          if(open)
          {
              FT_Close(handle);
          }
              DeleteCriticalSection(&cs);
      }

      void SetLatencyTimer(int msLatency)
      {
        FT_SetLatencyTimer(handle,(UCHAR) msLatency);
      }

      bool is_open() const{ return open;};
      //private:
          void lock(){EnterCriticalSection(&cs);};
          void unlock(){LeaveCriticalSection(&cs);};
          CRITICAL_SECTION cs;
          bool open;
          FT_HANDLE handle;
  };

  Connection* open_port(unsigned int baudrateIn);
}
#endif
*/
//#############################################################
// fonctions non utilisées
//#############################################################

/*
  inline int NumConnected(){
          DWORD num=0;
          FT_ListDevices(&num,NULL,FT_LIST_NUMBER_ONLY);
          return (int)num;
      }
*/
