#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <cassert>
#include <memory>
#include <pthread.h>
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <vector>

namespace muduo {
class Channel;
class Poller;
class EventLoop {

public:
  EventLoop();
  ~EventLoop();

  void loop();
  void quite();
  void assertInLoopThread() { assert(isInLoopThread()); }

  bool isInLoopThread() const { return threadId_ == syscall(SYS_gettid); }
  void updateChannel(Channel *channel);

private:
  typedef std::vector<Channel *> ChannelList;

  bool looping_;
  bool quite_;
  std::shared_ptr<Poller> poller_; // EventLoop通过poller管理channel
  ChannelList activeChannels_;
  const pid_t threadId_;
};
} // namespace muduo

#endif