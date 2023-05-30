#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <cstring>
#include <csignal>

size_t readn(int fd,void* buf,size_t len, bool isBlock, bool isRead);
size_t wirten(int fd, void* buf, size_t len,bool isWrite = false);
int socket_bind_and_listen(int port);
bool setSocketNoBlock(int fd);
bool setSocketNoDelay(int fd);
void handleSigpipe();

#endif