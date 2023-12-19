#include "EventLoop.h"
#include <iostream>

muduo::EventLoop loop;

int main() {

  loop.loop();

  return 0;
}