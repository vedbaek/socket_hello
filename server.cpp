#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // 创建Socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket.\n";
        return 1;
    }

    // 设置Socket地址
    sockaddr_in server_addr{}, client_addr{};
    int server_port = 8080;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(server_port);

    // 绑定Socket
    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to bind socket.\n";
        close(server_socket);
        return 1;
    }

    // 开始监听
    if (listen(server_socket, SOMAXCONN) == -1) {
        std::cerr << "Failed to listen on socket.\n";
        close(server_socket);
        return 1;
    }
    std::cout << "Server is listening on port " << server_port << "\n";

    // 等待连接
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);
    if (client_socket == -1) {
        std::cerr << "Failed to accept connection.\n";
        close(server_socket);
        return 1;
    }

    // 从客户端接收数据
    char buffer[1024]{};
    int len = recv(client_socket, buffer, sizeof(buffer), 0);
    if (len == -1) {
        std::cerr << "Failed to receive data.\n";
        close(client_socket);
        close(server_socket);
        return 1;
    }
    std::cout << "Received message from client: " << buffer << "\n";

    // 发送响应给客户端
    std::string message = "Hello from server!";
    if (send(client_socket, message.c_str(), message.length(), 0) == -1) {
        std::cerr << "Failed to send data.\n";
        close(client_socket);
        close(server_socket);
        return 1;
    }
    std::cout << "Sent response to client: " << message << "\n";

    // 关闭Socket
    close(client_socket);
    close(server_socket);

    return 0;
}
