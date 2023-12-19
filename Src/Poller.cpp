#include "Poller.h"
#include "Channel.h"
#include "EventLoop.h"
#include <algorithm>
#include <cassert>
#include <memory>
#include <poll.h>

namespace muduo {

Poller::Poller(EventLoop *loop) : ownerLoop_(loop) {}
Poller::~Poller() {}

void Poller::poll(int timeoutMs, ChannelList &activeChannels) {
  int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);
  if (numEvents > 0) {
    fillActiveChannleList(numEvents, activeChannels);

  } else {
  }
}

void Poller::fillActiveChannleList(int numEvents,
                                   ChannelList &activeChannels) const {

  for (auto i : pollfds_) {

    if (numEvents <= 0) {
      return;
    }

    if (i.revents > 0) {
      auto ch = channels_.find(i.fd);
      assert(ch != channels_.end()); // 检查fd是非被管理
      Channel *channel = ch->second;

      assert(channel->fd() == ch->first); // 检查channel是否处理唤起fd事件
      channel->setrevents(i.revents);

      activeChannels.push_back(channel);
    }
    numEvents--;
  }
}

void Poller::updateChannel(Channel *channel) {

  auto ch = channels_.find(channel->fd());
  if (ch == channels_.end()) {
    // 如果不存在，就新建一个 pollfd，并将其添加到 pollfds_ 中

    pollfd pfd;
    pfd.fd = channel->fd();
    pfd.events = channel->events();
    pfd.revents = 0;
    pollfds_.push_back(std::move(pfd));

    channel->setindex(pollfds_.size() - 1);
    channels_.insert(std::make_pair(channel->fd(), channel));
  } else {
    // 有映射关系在使用时应当检查映射关系
    assert(channels_.find(channel->fd()) != channels_.end());
    assert(channels_[channel->fd()] == channel);
    int idx = channel->index();
    // 使用时应当检查容器，数组边界
    assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
    pollfd &pfd = pollfds_[channel->index()];
    // 检查文件描述符
    assert(channel->fd() == pfd.fd || pfd.fd == -1);
    pfd.events = channel->events();
    pfd.revents = 0;
  }
}

} // namespace muduo