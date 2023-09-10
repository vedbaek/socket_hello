#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // 创建Socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Failed to create socket.\n";
        return 1;
    }

    // 设置服务器地址
    sockaddr_in server_addr{};
    int server_port = 8080;
    std::string server_ip = "127.0.0.1";
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
    server_addr.sin_port = htons(server_port);

    // 连接服务器
    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to connect to server.\n";
        close(client_socket);
        return 1;
    }

    // 发送数据给服务器
    std::string message = "Hello from client!";
    if (send(client_socket, message.c_str(), message.length(), 0) == -1) {
        std::cerr << "Failed to send data.\n";
        close(client_socket);
        return 1;
    }
    std::cout << "Sent message to server: " << message << "\n";

    // 等待响应
    char buffer[1024]{};
    int len = recv(client_socket, buffer, sizeof(buffer), 0);
    if (len == -1) {
        std::cerr << "Failed to receive data.\n";
        close(client_socket);
        return 1;
    }
    std::cout << "Received response from server: " << buffer << "\n";

    // 关闭Socket
    close(client_socket);
    
    return 0;
}
