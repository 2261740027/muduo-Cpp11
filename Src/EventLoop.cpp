#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"
#include <assert.h>
#include <cstddef>
#include <iostream>
#include <poll.h>
#include <sys/poll.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace muduo {

__thread EventLoop *t_loopInThisThread = nullptr;

EventLoop::EventLoop()
    : looping_(false), quite_(false), poller_(new Poller(this)),
      threadId_(syscall(SYS_gettid)) {
  if (t_loopInThisThread) {

  } else {
    t_loopInThisThread = this;
  }
}

EventLoop::~EventLoop() {
  assert(!looping_);
  t_loopInThisThread = nullptr;
}

void EventLoop::loop() {
  assert(!looping_);
  assertInLoopThread();
  looping_ = true;

  while (!quite_) {
    poller_->poll(1000, activeChannels_);
    for (auto it : activeChannels_) {
      it->handleEvent();
    }
  }
}

void EventLoop::quite() {
  assert(looping_);
  looping_ = false;
  quite_ = true;
}

void EventLoop::updateChannel(Channel *channel) {
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  poller_->updateChannel(channel);
}

} // namespace muduo