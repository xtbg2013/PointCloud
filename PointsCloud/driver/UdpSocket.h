#pragma once
#include "ICom.h"
#include <netinet/in.h>
#include <string>
using std::string;

class UdpServerSocket:public ICom
{
private:
    int m_port;
    int m_socket_fd;
    sockaddr_in m_addr_client;
    int m_addr_client_len ;
public:
    UdpServerSocket(int port);
    virtual ~UdpServerSocket();
    virtual bool Initialize();
    virtual void Release();
    virtual int  Read(char *pbuffer,int length);
    virtual int  Write(char *pBuffer,int length);
};

class UdpClentSocket:public ICom
{
private:
    sockaddr_in  m_server_addr;
    int m_server_addr_len ;
    sockaddr_in  m_local_addr;
    int m_local_addr_len ;
    int m_socket_fd;
public:
    UdpClentSocket(string Ip,int port);
    UdpClentSocket(string Ip,int remotePort,int localPort);
    virtual ~UdpClentSocket();
    virtual bool Initialize();
    virtual void Release();
    virtual int  Read(char *pbuffer,int length);
    virtual int  Write(char *pBuffer,int length);
};


