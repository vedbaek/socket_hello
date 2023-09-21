#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// 结构体类型
enum MessageType {
    PERSON,
    ANIMAL,
    CAR
};

// Person 结构体
struct Person {
    char name[20];
    int age;
};

// Animal 结构体
struct Animal {
    char species[20];
    float weight;
};

// Car 结构体
struct Car {
    char brand[20];
    int year;
};

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;

    // 创建套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    // 设置服务器地址和端口
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 服务器 IP 地址
    serverAddress.sin_port = htons(8888); // 服务器端口号

    // 连接服务器
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
        return -1;
    }

    // 发送 Person 结构体
    MessageType type = PERSON;
    send(clientSocket, &type, sizeof(type), 0);

    Person person;
    strncpy(person.name, "John", sizeof(person.name));
    person.age = 30;
    send(clientSocket, &person, sizeof(person), 0);

    // 发送 Animal 结构体
    type = ANIMAL;
    send(clientSocket, &type, sizeof(type), 0);

    Animal animal;
    strncpy(animal.species, "Lion", sizeof(animal.species));
    animal.weight = 200.5f;
    send(clientSocket, &animal, sizeof(animal), 0);

    // 发送 Car 结构体
    type = CAR;
    send(clientSocket, &type, sizeof(type), 0);

    Car car;
    strncpy(car.brand, "Toyota", sizeof(car.brand));
    car.year = 2022;
    send(clientSocket, &car, sizeof(car), 0);

    // 关闭客户端套接字
    close(clientSocket);

    return 0;
}
