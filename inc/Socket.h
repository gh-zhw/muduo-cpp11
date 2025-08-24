#pragma once

#include "noncopyable.h"


class InetAddress;

// 封装socket fd
class Socket : noncopyable
{
public:
    explicit Socket(int sockfd)
        : sockfd_(sockfd)
    {
    }
    ~Socket();

    int fd() const { return sockfd_; }
    void bindAddress(const InetAddress &localaddr);
    void listen();
    int accept(InetAddress *peeraddr);

    void shutdownWrite();  // 设置半关闭

    void setTcpNoDelay(bool on);  // 设置 Nagle 算法
    void setReuseAddr(bool on);   // 设置地址复用
    void setReusePort(bool on);   // 设置端口复用
    void setKeepAlive(bool on);   // 设置长时间连接

private:
    const int sockfd_;
};