#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8888
#define BUFFER_SIZE 1024

using namespace std;

// 定义3个结构体类型
struct Msg1 {
    int type;
    char data[20];
};

struct Msg2 {
    int type;
    float data;
};

struct Msg3 {
    int type;
    double data[5];
};

int main() {
    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        cerr << "Error: Failed to create socket." << endl;
        return 1;
    }

    // 绑定地址和端口
    struct sockaddr_in servaddr, cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        cerr << "Error: Failed to bind address." << endl;
        close(sockfd);
        return 1;
    }

    cout << "Server started. Listening on port " << PORT << endl;

    while (true) {
        // 接收请求
        char buffer[BUFFER_SIZE];
        socklen_t len = sizeof(cliaddr);
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
        if (n < 0) {
            cerr << "Error: Failed to receive message." << endl;
            continue;
        }

        // 处理请求
        int type = *((int *)buffer);  // 第一个4字节为消息类型
        cout << "Received message from client. Type: " << type << endl;

        // 根据消息类型解析数据，并进行相应的处理
        switch (type) {
            case 1:
                {
                    Msg1 *msg = (Msg1 *)buffer;  // 解析消息结构体
                    // 处理Msg1类型的数据，如打印消息内容
                    cout << "Msg1: " << msg->data << endl;
                    break;
                }
            case 2:
                {
                    Msg2 *msg = (Msg2 *)buffer;
                    // 处理Msg2类型的数据，如打印消息内容
                    cout << "Msg2: " << msg->data << endl;
                    break;
                }
            case 3:
                {
                    Msg3 *msg = (Msg3 *)buffer;
                    // 处理Msg3类型的数据，如打印消息内容
                    for (int i = 0; i < 5; i++) {
                        cout << msg->data[i] << ' ';
                    }
                    cout << endl;
                    break;
                }
            default:
                cout << "Unsupported message type." << endl;
        }

        // 回复客户端确认信息
        char reply[] = "Message received.";
        sendto(sockfd, reply, strlen(reply), 0, (struct sockaddr *)&cliaddr, len);
    }

    // 关闭套接字
    close(sockfd);

    return 0;
}