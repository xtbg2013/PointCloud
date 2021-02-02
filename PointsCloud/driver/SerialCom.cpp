
#include "SerialCom.h"

SerialCom::SerialCom(int port)
{
    m_port = port;
}

SerialCom::~SerialCom()
{
}

bool SerialCom::Initialize()
{
    return true;
}

void SerialCom::Release()
{
     
}

int  SerialCom::Read(char *pbuffer,int length)
{
    return 0;
}
int  SerialCom::Write(char *pBuffer,int length)
{
    return 0;
}