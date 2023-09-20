#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUFFER_SIZE = 1024;

void sendMessage(int socket, const std::string& message) {
    size_t messageLength = message.length();
    size_t networkOrderLength = htonl(messageLength);
    
    // 发送消息长度字段
    ssize_t bytesSent = send(socket, &networkOrderLength, sizeof(size_t), 0);
    if (bytesSent <= 0) {
        std::cerr << "发送消息长度字段失败" << std::endl;
        return;
    }
    
    // 发送消息内容
    bytesSent = send(socket, message.c_str(), messageLength, 0);
    if (bytesSent <= 0) {
        std::cerr << "发送消息内容失败" << std::endl;
        return;
    }
}

int main() {
    int client_socket;
    struct sockaddr_in server_address{};
    char buffer[BUFFER_SIZE];

    // 创建 Socket 对象
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "创建 Socket 失败" << std::endl;
        return -1;
    }

    // 设置服务器地址和端口
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8888);
    if (inet_pton(AF_INET, "127.0.0.1", &(server_address.sin_addr)) <= 0) {
        std::cerr << "设置服务器地址失败" << std::endl;
        return -1;
    }

    // 连接服务器
    if (connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        std::cerr << "连接服务器失败" << std::endl;
        return -1;
    }

    std::cout << "已连接到服务器" << std::endl;

    std::string messages[] = {
            "Hello",
            "World",
            "This is a test message"
    };

    // 发送数据
    for (const auto &message : messages) {
        sendMessage(client_socket, message);
        usleep(100000);  // 为了演示粘包问题，故意加入延迟
    }

    // 关闭连接
    close(client_socket);

    return 0;
}
