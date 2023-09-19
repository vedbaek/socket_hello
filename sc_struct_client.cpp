#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345

// 消息类型枚举
enum MessageType {
    PERSON,
    ANIMAL,
    CAR
};

struct MessageHeader {
    MessageType type;
    int size;
};

struct Person {
    char name[20];
    int age;
};

struct Animal {
    char species[20];
    float weight;
};

struct Car {
    char brand[20];
    int year;
};

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddr;

    // 创建套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        std::cerr << "创建套接字失败" << std::endl;
        return -1;
    }

    // 设置服务器地址和端口
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    // 将IPv4地址从文本转换为二进制形式
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "转换服务器地址失败" << std::endl;
        return -1;
    }

    // 连接到服务器
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "连接服务器失败" << std::endl;
        return -1;
    }

    std::cout << "已连接到服务器" << std::endl;

    while (true)
    {
        // 发送消息给服务器
        int choice;
        std::cout << "请选择要发送的结构体类型：" << std::endl;
        std::cout << "1. Person" << std::endl;
        std::cout << "2. Animal" << std::endl;
        std::cout << "3. Car" << std::endl;
        std::cout << "0. 退出" << std::endl;
        std::cout << "请输入选择：";
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 0)
        {
            break;
        }

        // 构造消息头和对应的结构体数据
        MessageHeader header;
        char buffer[1024];

        switch (choice)
        {
            case 1:
            {
                // 构造 Person 结构体
                Person person;
                std::cout << "请输入姓名：";
                std::cin.getline(person.name, sizeof(person.name));
                std::cout << "请输入年龄：";
                std::cin >> person.age;
                std::cin.ignore();

                header.type = PERSON;
                header.size = sizeof(person);
                memcpy(buffer, &person, sizeof(person));
                break;
            }
            case 2:
            {
                // 构造 Animal 结构体
                Animal animal;
                std::cout << "请输入物种：";
                std::cin.getline(animal.species, sizeof(animal.species));
                std::cout << "请输入重量：";
                std::cin >> animal.weight;
                std::cin.ignore();

                header.type = ANIMAL;
                header.size = sizeof(animal);
                memcpy(buffer, &animal, sizeof(animal));
                break;
            }
            case 3:
            {
                // 构造 Car 结构体
                Car car;
                std::cout << "请输入品牌：";
                std::cin.getline(car.brand, sizeof(car.brand));
                std::cout << "请输入年份：";
                std::cin >> car.year;
                std::cin.ignore();

                header.type = CAR;
                header.size = sizeof(car);
                memcpy(buffer, &car, sizeof(car));
                break;
            }
            default:
                std::cout << "无效选择，请重新输入" << std::endl;
                continue;
        }

        // 发送消息头和结构体数据给服务器
        send(clientSocket, &header, sizeof(header), 0);
        send(clientSocket, buffer, header.size, 0);

        // 接收服务器响应
        memset(&header, 0, sizeof(header));
        int bytesRead = recv(clientSocket, &header, sizeof(header), 0);

        if (bytesRead <= 0)
        {
            std::cerr << "与服务器的连接已断开" << std::endl;
            break;
        }

        std::cout << "服务器响应：" << header.type << ", " << header
    }

    // 关闭客户端套接字
    close(clientSocket);
    std::cout << "客户端套接字已关闭" << std::endl;

    return 0;
}
