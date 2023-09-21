#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <map>

#define PORT 12345

// 消息类型枚举
enum MessageType
{
    PERSON,
    ANIMAL,
    CAR
};

struct MessageHeader
{
    MessageType type;
    int size;
};

struct Person
{
    std::string name;
    int age;
};

struct Animal
{
    std::string species;
    float weight;
};

struct Car
{
    std::string brand;
    int year;
};

void handleClient(int clientSocket)
{
    char buffer[1024];
    int bytesRead;

    while (true)
    {
        // 接收消息头
        MessageHeader header;
        memset(&header, 0, sizeof(header));
        bytesRead = recv(clientSocket, &header, sizeof(header), 0);

        if (bytesRead <= 0)
        {
            break;
        }

        if (header.size > sizeof(buffer))
        {
            std::cerr << "数据大小超过缓冲区限制" << std::endl;
            break;
        }

        // 接收结构体数据
        memset(buffer, 0, sizeof(buffer));
        bytesRead = recv(clientSocket, buffer, header.size, 0);

        if (bytesRead <= 0)
        {
            break;
        }

        // 处理接收到的结构体数据
        switch (header.type)
        {
        case PERSON:
        {
            if (bytesRead != sizeof(Person))
            {
                std::cerr << "接收到的 Person 结构体数据大小不匹配" << std::endl;
                break;
            }

            Person person;
            memcpy(&person, buffer, sizeof(person));
            std::cout << "收到 Person 结构体消息：" << person.name << ", " << person.age << "岁" << std::endl;
            break;
        }
        case ANIMAL:
        {
            if (bytesRead != sizeof(Animal))
            {
                std::cerr << "接收到的 Animal 结构体数据大小不匹配" << std::endl;
                break;
            }

            Animal animal;
            memcpy(&animal, buffer, sizeof(animal));
            std::cout << "收到 Animal 结构体消息：" << animal.species << ", " << animal.weight << "kg" << std::endl;
            break;
        }
        case CAR:
        {
            if (bytesRead != sizeof(Car))
            {
                std::cerr << "接收到的 Car 结构体数据大小不匹配" << std::endl;
                break;
            }

            Car car;
            memcpy(&car, buffer, sizeof(car));
            std::cout << "收到 Car 结构体消息：" << car.brand << ", " << car.year << std::endl;
            break;
        }
        default:
            std::cerr << "未知的消息类型" << std::endl;
            break;
        }

        // 发送响应给客户端
        send(clientSocket, &header, sizeof(header), 0);
    }

    // 关闭客户端连接
    close(clientSocket);
    std::cout << "客户端连接已关闭" << std::endl;
}

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(struct sockaddr_in);

    // 创建套接字
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "创建套接字失败" << std::endl;
        return -1;
    }

    // 设置服务器地址和端口
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // 端口复用
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));

    // 绑定套接字到服务器地址和端口
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "绑定套接字失败" << std::endl;
        return -1;
    }

    // 开始监听连接请求
    if (listen(serverSocket, 5) < 0)
    {
        std::cerr << "开始监听连接请求失败" << std::endl;
        return -1;
    }

    std::cout << "等待客户端连接..." << std::endl;

    while (true)
    {
        // 接受客户端连接
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
        if (clientSocket < 0)
        {
            std::cerr << "接受客户端连接失败" << std::endl;
            continue;
        }

        // 处理客户端连接
        handleClient(clientSocket);
    }

    // 关闭服务器套接字
    close(serverSocket);
    std::cout << "服务器套接字已关闭" << std::endl;

    return 0;
}
