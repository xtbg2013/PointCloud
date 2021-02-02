#include "UdpSocket.h"
#include <string.h>
#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

UdpServerSocket::UdpServerSocket(int port)
{
    m_port = port;
    m_socket_fd = 0;
    memset(&m_addr_client,0,sizeof(m_addr_client));
    m_addr_client_len = sizeof(sockaddr_in);
}

UdpServerSocket::~UdpServerSocket()
{
}

bool UdpServerSocket::Initialize()
{
    m_socket_fd = socket(AF_INET,SOCK_DGRAM,0);
    if(m_socket_fd < 0)
        return false;
    struct sockaddr_in addr_server;
    bzero(&addr_server,sizeof(addr_server));
    //memset(&addr_server,0,sizeof(addr_server));
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(m_port);
    addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
    int length =  sizeof(addr_server);
    if(bind(m_socket_fd,(sockaddr*)&addr_server,length) < 0 )
    {
        return false;
    }
     
    return true;
}

void UdpServerSocket::Release()
{
    close(m_socket_fd);
}

int  UdpServerSocket::Read(char *pbuffer,int length)
{
    return recvfrom(m_socket_fd,pbuffer,length,0,(sockaddr*)&m_addr_client,(socklen_t*)&m_addr_client_len);
}
int  UdpServerSocket::Write(char *pBuffer,int length)
{
    return sendto(m_socket_fd,pBuffer,length,0,(sockaddr*)&m_addr_client,m_addr_client_len);
}



UdpClentSocket::UdpClentSocket(string Ip,int port)
{
    bzero(&m_server_addr,sizeof(m_server_addr));
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = inet_addr(Ip.c_str());
    m_server_addr.sin_port = htons(port);
    m_server_addr_len = sizeof(m_server_addr);

    
    bzero(&m_local_addr,sizeof(m_local_addr));
    m_local_addr.sin_family = AF_INET;
    m_local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_local_addr.sin_port = htons(port);
    m_local_addr_len = sizeof(m_local_addr_len);
    


}
UdpClentSocket::~UdpClentSocket()
{

}
bool UdpClentSocket::Initialize()
{
    m_socket_fd = socket(AF_INET,SOCK_DGRAM,0);
    if(m_socket_fd < 0)
        return false;
   
    if(bind(m_socket_fd,(sockaddr*)&m_local_addr,sizeof(m_local_addr)) < 0 )
    {
        return false;
    }
    
    return true;
}

void UdpClentSocket::Release()
{
    close(m_socket_fd);
}

int  UdpClentSocket::Read(char *pbuffer,int length)
{
    return recvfrom(m_socket_fd,pbuffer,length,0,(sockaddr*)&m_local_addr,(socklen_t*)&m_local_addr_len);
}

int  UdpClentSocket::Write(char *pBuffer,int length)
{
    return sendto(m_socket_fd,pBuffer,length,0,(sockaddr*)&m_server_addr,m_server_addr_len);
}