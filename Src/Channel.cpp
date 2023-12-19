#include "Channel.h"
#include "EventLoop.h"
#include <sys/poll.h>

namespace muduo {

const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::KWriteEvent = POLLOUT;
const int Channel::kNoneEvent = 0;

Channel::Channel(EventLoop *ownerLoop, int fd) : loop_(ownerLoop), fd_(fd) {}

Channel::~Channel() {}

} // namespace muduo