#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <cstdint> // uint32_t
#include <random> // random_device
#include <netinet/in.h> // hton/ntoh

int rand() {
    static std::random_device rd;  // a seed source for the random number engine
    static std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    static std::uniform_int_distribution<> distrib(1000, 9999);
    return distrib(gen);
}

float ntohf(float value) {
    uint32_t temp = ntohl(*((uint32_t *)&value));
    return *((float *)&temp);
}

enum MsgType {
  TICK,
  HELLO,
};

struct MsgHead {
  MsgType type;
  uint32_t size;
};

struct MsgEcho: public MsgHead {
  MsgEcho(): MsgHead{TICK, sizeof(MsgEcho)} {}
};

struct MsgHello: public MsgHead {
  MsgHello(): MsgHead{HELLO, sizeof(MsgHello)} {}
  char data[20];
};

#endif // __MESSAGE_H__