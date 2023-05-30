#include "tools.h"
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "Lock.h"

int socket_bind_and_listen(int port)
{
    int listen_id = 0;
    if (listen_id = socket(AF_INET, SOCK_STREAM, 0) == -1)
    {
        return -1;
    }
    sockaddr_in server_addr;
    memset(&server_addr, '\x00', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((unsigned short)port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int reuse_opt = 1;
    if (setsockopt(listen_id, SOL_SOCKET, SO_REUSEADDR, &reuse_opt, sizeof(reuse_opt)) == -1)
    {
        return -1;
    }
    if (bind(listen_id, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        return -1;
    }
    if (listen(listen_id, 1024) == -1)
    {
        return -1;
    }
    return listen_id;
}

bool setSocketNoBlock(int fd)
{
    int flag = fcntl(fd, F_GETFD);
    if (flag == -1)
        return -1;
    flag |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flag) == -1)
        return false;
    return true;
}

bool setSocketNoDelay(int fd)
{
    int enable = 1;
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void *)&enable, sizeof(enable)) == -1)
        return false;
    return true;
}

size_t readn(int fd, void *buf, size_t len, bool isBlock, bool isRead)
{
    char *buffer = (char *)buf;
    size_t length = len;
    size_t haveRead = 0;
    while (length > 0)
    {
        size_t tmpRead = 0;
        if (isRead)
        {
            tmpRead = read(fd, buffer, length);
        }
        else
        {
            tmpRead = recv(fd, buffer, length, (isBlock ? 0 : MSG_DONTWAIT));
        }
        if (tmpRead < 0)
        {
            if (errno == EINTR)
                tmpRead = 0;
            else if (errno == EAGAIN)
                return haveRead;
            else
                return -1;
        }
        if (tmpRead == 0)
            break;
        haveRead += tmpRead;
        buffer += tmpRead;
        if (isBlock && static_cast<size_t>(tmpRead) < length)
            break;

        length -= tmpRead;
    }
    return haveRead;
}

size_t writen(int fd, void* buf, size_t len, bool isWrite){
    char *buffer = (char *)buf;
    size_t length = len;
    size_t haveWrite = 0;
    while(length>0){
                ssize_t tmpWrite = 0;

        if(isWrite)
            tmpWrite = write(fd, buffer, length);
        else
            tmpWrite = send(fd, buffer, length, 0);

        if(tmpWrite < 0)
        {
            if(errno == EINTR || errno == EAGAIN)
                tmpWrite = 0;
            else
                return -1;
        }
        if(tmpWrite == 0)
            break;
        haveWrite += tmpWrite;
        buffer += tmpWrite;
        length -= tmpWrite;
    }
    return haveWrite;
}