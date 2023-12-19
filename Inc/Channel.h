#ifndef CHANNEL_H
#define CHANNEL_H

/*
 *   一个channel绑定一个fd，同时注册回调函数
 */

#include "EventLoop.h"
#include <functional>

namespace muduo {

class Channel {
  typedef std::function<void()> EventCallback;

public:
  Channel(EventLoop *ownerLoop, int fd);
  ~Channel();

  void handleEvent() {
    if (revents_ == kReadEvent) {
      readCallback_();
    } else if (revents_ == KWriteEvent) {
      writeCallback_();
    }
  }

  /*
   * 注册回调函数 const修饰引用防止在调用时修改
   */
  void setReadCallback(const EventCallback &cb) { readCallback_ = cb; }
  void setWrireCallback(const EventCallback &cb) { writeCallback_ = cb; }
  void setErrorCallback(const EventCallback &cb) { errorCallback_ = cb; }

  void enableReading() {
    events_ = kReadEvent;
    update();
  }
  void setrevents(int revents) { revents_ = revents; }
  void setindex(int ind) { index_ = ind; }

  int fd() { return fd_; }
  int events() { return events_; }
  int revents() { return revents_; }
  int index() { return index_; }
  EventLoop *ownerLoop() { return loop_; }

  void update() { loop_->updateChannel(this); }

private:
  static const int kReadEvent;
  static const int kNoneEvent;
  static const int KWriteEvent;

  // 事件回调函数
  EventCallback readCallback_;
  EventCallback writeCallback_;
  EventCallback errorCallback_;

  EventLoop *loop_;
  int fd_;
  int events_;
  int revents_;
  int index_; // 记录在channel在pollers链表中的位置
};

} // namespace muduo

#endif