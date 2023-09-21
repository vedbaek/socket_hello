#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <cstdint> // uint32_t

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