#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUFFER_SIZE = 1024;

void handleClient(int client_socket)
{
  char buffer[BUFFER_SIZE];
  size_t totalBytesReceived = 0;

  // 接收消息长度字段
  size_t bytesToReceive = sizeof(size_t);
  while (bytesToReceive > 0)
  {
    ssize_t bytesRead = recv(client_socket, buffer + totalBytesReceived, bytesToReceive, 0);
    if (bytesRead <= 0)
    {
      std::cerr << "接收消息长度字段失败" << std::endl;
      return;
    }

    totalBytesReceived += bytesRead;
    bytesToReceive -= bytesRead;
  }

  // 解析消息长度
  size_t messageLength;
  memcpy(&messageLength, buffer, sizeof(size_t));
  messageLength = ntohl(messageLength);

  // 接收消息内容
  totalBytesReceived = 0;
  bytesToReceive = messageLength;
  while (bytesToReceive > 0)
  {
    ssize_t bytesRead = recv(client_socket, buffer + totalBytesReceived, bytesToReceive, 0);
    if (bytesRead <= 0)
    {
      std::cerr << "接收消息内容失败" << std::endl;
      return;
    }

    totalBytesReceived += bytesRead;
    bytesToReceive -= bytesRead;
  }

  std::string message(buffer, messageLength);
  std::cout << "接收到消息：" << message << std::endl;
}

int main()
{
  int server_socket, client_socket;
  struct sockaddr_in server_address
  {
  }, client_address{};
  socklen_t client_address_size;

  // 创建 Socket 对象
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
  {
    std::cerr << "创建 Socket 失败" << std::endl;
    return -1;
  }

  // 设置地址重用选项
  int reuse = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
  {
    std::cerr << "设置地址重用失败" << std::endl;
    return -1;
  }

  // 绑定地址和端口
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(8888);
  if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
  {
    std::cerr << "绑定地址和端口失败" << std::endl;
    return -1;
  }

  // 监听连接请求
  if (listen(server_socket, 5) == -1)
  {
    std::cerr << "监听失败" << std::endl;
    return -1;
  }

  std::cout << "等待客户端连接..." << std::endl;
  client_address_size = sizeof(client_address);
  client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
  if (client_socket < 0)
  {
    std::cerr << "接受客户端连接失败" << std::endl;
    return -1;
  }

  std::cout << "客户端已连接" << std::endl;

  handleClient(client_socket);

  // 关闭连接
  close(client_socket);
  close(server_socket);

  return 0;
}
