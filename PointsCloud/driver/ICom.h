#pragma once
class ICom
{
public:
    virtual ~ICom(){};
    virtual bool Initialize() = 0;
    virtual void Release() = 0;
    virtual int  Read(char *pbuffer,int length) = 0;
    virtual int  Write(char *pBuffer,int length) = 0;

};

 
