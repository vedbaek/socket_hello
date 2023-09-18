#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
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

    // 设置服务器的地址和端口
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servaddr.sin_port = htons(PORT);

    cout << "Client started. Connect to server " << SERVER_IP << ":" << PORT << endl;

    while (true) {
        // 发送请求
        int type;
        cout << "Please enter message type (1-3): ";
        cin >> type;
        if (type <= 0 || type > 3) {
            cerr << "Error: Unsupported message type." << endl;
            break;
        }

        void *msg;
        int size;
        switch (type) {
            case 1:
                {
                    Msg1 m;
                    m.type = type;
                    cout << "Please enter message data (max 20 chars): ";
                    cin >> m.data;
                    msg = (void *)&m;
                    size = sizeof(m);
                    break;
                }
            case 2:
                {
                    Msg2 m;
                    m.type = type;
                    cout << "Please enter message data (a float number): ";
                    cin >> m.data;
                    msg = (void *)&m;
                    size = sizeof(m);
                    break;
                }
            case 3:
                {
                    Msg3 m;
                    m.type = type;
                    cout << "Please enter message data (5 double numbers): ";
                    for (int i = 0; i < 5; i++) {
                        cin >> m.data[i];
                    }
                    msg = (void *)&m;
                    size = sizeof(m);
                    break;
                }
        }

        sendto(sockfd, msg, size, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

        // 接收确认信息
        char buffer[BUFFER_SIZE];
        socklen_t len = sizeof(servaddr);
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&servaddr, &len);
        if (n < 0) {
            cerr << "Error: Failed to receive confirmation message." << endl;
            continue;
        }

        buffer[n] = '\0';
        cout << "Server response: " << buffer << endl;
    }

    // 关闭套接字
    close(sockfd);

    return 0;
}