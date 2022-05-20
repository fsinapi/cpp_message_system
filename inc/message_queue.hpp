/**
 * @file message_queue.hpp
 * @brief Provides FIFO for a message type
 */

#include <deque>
#include <optional>

namespace message_queue {

template<typename T>
class MessageQueue {
  public:
    using Message = T;
    MessageQueue() : _queue() {}

    auto send(const Message& msg) -> void {
        _queue.push_back(msg);
    }

    auto try_read() -> std::optional<Message> {
        if(_queue.size() > 0) {
            auto ret = _queue.at(0);
            _queue.pop_front();
            return ret;
        }
        return std::nullopt;
    }

    auto has_message() -> bool {
        return _queue.size() > 0;
    }

  private:
    std::deque<Message> _queue;
};

};