#ifndef POLLER_H
#define POLLER_H

#include "EventLoop.h"
#include <list>
#include <map>
#include <poll.h>
#include <vector>

namespace muduo {

class EventLoop;
class Channel;

typedef std::vector<Channel *> ChannelList;

class Poller {
public:
  Poller(EventLoop *loop);
  ~Poller();

  void poll(int timeoutMs, ChannelList &activeChannels);
  void updateChannel(Channel *channel);
  void assertInLoopThread() { ownerLoop_->assertInLoopThread(); }

private:
  void fillActiveChannleList(int numEvents, ChannelList &activeChannels) const;

  typedef std::vector<struct pollfd> fdList;
  typedef std::map<int, Channel *> channelMap;

  EventLoop *ownerLoop_;

  // 将一个fd和一个channel对应起来管理，当一个fd触发事件时唤醒对应channel处理事件
  channelMap channels_;
  fdList pollfds_;
};

} // namespace muduo

#endif