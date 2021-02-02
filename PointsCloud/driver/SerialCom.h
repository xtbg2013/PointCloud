#pragma once
#include "ICom.h"
#define COM_ID 0
 
class SerialCom:public ICom
{
private:
    int m_port;
public:
    SerialCom(int port);
    virtual ~SerialCom();
    virtual bool Initialize();
    virtual void Release();
    virtual int  Read(char *pbuffer,int length);
    virtual int  Write(char *pBuffer,int length);
};