#include "message.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

float htonf(float value)
{
    uint32_t temp = htonl(*((uint32_t *)&value));
    return *((float *)&temp);
}

// 结构体类型
enum MessageType
{
    PERSON,
    ANIMAL,
    CAR
};

// Person 结构体
struct Person
{
    char name[20];
    int age;
};

// Animal 结构体
struct Animal
{
    char species[20];
    float weight;
};

// Car 结构体
struct Car
{
    char brand[20];
    int year;
};

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddress;

    // 创建套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    // 设置服务器地址和端口
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 服务器 IP 地址
    serverAddress.sin_port = htons(8888);                   // 服务器端口号

    // 连接服务器
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "Failed to connect to server" << std::endl;
        return -1;
    }

    int id = rand();
    while (true)
    {
        // // 发送 Person 结构体
        MessageType type = PERSON;
        // type = (MessageType)htonl((unsigned int)type);
        // write(clientSocket, &type, sizeof(type));

        // Person person;
        // strncpy(person.name, "John", sizeof(person.name));
        // person.age = htonl(30);
        // write(clientSocket, &person, sizeof(person));

        // // 发送 Animal 结构体
        // type = ANIMAL;
        // type = (MessageType)htonl((unsigned int)type);
        // write(clientSocket, &type, sizeof(type));

        // Animal animal;
        // strncpy(animal.species, "Lion", sizeof(animal.species));
        // animal.weight = htonf(222.555555f);
        // write(clientSocket, &animal, sizeof(animal));

        // 发送 Car 结构体
        type = CAR;
        type = (MessageType)htonl((unsigned int)type);
        write(clientSocket, &type, sizeof(type));

        Car car;
        strncpy(car.brand, "Toyota", sizeof(car.brand));
        car.year = htonl(id);
        write(clientSocket, &car, sizeof(car));

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    // 关闭客户端套接字
    close(clientSocket);

    return 0;
}
