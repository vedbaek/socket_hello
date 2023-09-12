#include <sys/socket.h> // socket()、bind() 和 connect()
#include <arpa/inet.h> // inet_pton() 和 inet_ntop() 函数，用于进行 IP 地址的二进制和字符串的转换
#include <unistd.h> // close()，用于关闭文件描述符，包括套接字
#include <string>
#include <iostream>

class SockBase {
public:
  explicit SockBase(unsigned port): socket_port(port) {
    // AF_INET使用IPv4
    // SOCK_STREAM使用TCP流式socket
    // 0 使用默认协议
    socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_id == -1) {
      std::err << "Failed to create socket" << std::endl;
    }
  }
  ~SockBase() { close(socket_id); }

private:
  int socket_id;
  unsigned socket_port;
};

class SockServer: public SockBase {
public:
  SockServer(unsigned port): SockBase(port) {

  }
}