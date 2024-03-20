#ifndef _MKPROTOCOL_H
#define _MKPROTOCOL_H

// slave addresses
#define ANY_ADDRESS 0
#define FC_ADDRESS 1
#define NC_ADDRESS 2
#define MK3MAG_ADDRESS 3
#define MKOSD_ADDRESS 4

typedef struct
{
    u8* pData;
    u16 Size;
    u16 DataBytes;
    u16 Position;
    u8  Locked;
} __attribute__((packed)) Buffer_t;

extern void Buffer_Init(Buffer_t* pBuffer, u8* pDataBuffer, u16 DataBufferSize);
extern void Buffer_Clear(Buffer_t* pBuffer);
extern u8 Buffer_Copy(Buffer_t* pSrcBuffer, Buffer_t* pDstBuffer);

typedef struct
{
    u8 Address;
    u8 CmdID;
     u8* pData;
    u16 DataLen; 
} __attribute__((packed)) SerialMsg_t;

extern u8 MKProtocol_CollectSerialFrame(Buffer_t* pRxBuff, u8 c);
extern void MKProtocol_CreateSerialFrame(Buffer_t* pTxBuff, u8 CmdID, u8 Address, u8 numofbuffers , ...); //u8 *data, u8 len, ....;
extern void MKProtocol_DecodeSerialFrameHeader(Buffer_t* pRxBuff, SerialMsg_t* pSerialMsg);
extern void MKProtocol_DecodeSerialFrameData(Buffer_t* pRxBuff, SerialMsg_t* pSerialMsg);

#endif // _MKPROTOCOL_H
