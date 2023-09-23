#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <cstdint> // uint32_t
#include <random> // random_device

int rand() {
    static std::random_device rd;  // a seed source for the random number engine
    static std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    static std::uniform_int_distribution<> distrib(1000, 9999);
    return distrib(gen);
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