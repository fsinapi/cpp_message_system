/**
 * @file tasks.hpp
 * @brief Aggregates task queues
 */

#include <atomic>
#include <tuple>
#include <variant>
#include <memory>

namespace tasks {

template<typename Q, typename Msg>
concept MsgQueue = requires(Q queue, Msg msg) {
    { queue.send(msg) } -> std::same_as<void>;
};

template<typename... MessageQueues>
class Tasks {
  public:
    using MessageTypes = 
        std::variant<typename MessageQueues::Message...>;
    
    static constexpr size_t TaskCount = std::variant_size_v<MessageTypes>;

    Tasks() :
        _handles(),
        _initialized{false} {}
    
    template<size_t Idx, typename Queue>
    requires MsgQueue<Queue, typename Queue::Message>
    auto register_queue(Queue &queue) -> void {
        static_assert(Idx < TaskCount,
            "Invalid task index");
        if(_initialized.at(Idx)) {
            // Error
            std::__throw_bad_exception();
        }
        std::get<Idx>(_handles) = &queue;
        _initialized.at(Idx) = true;
    }
    
    template<typename Message>
    constexpr static auto get_task_idx() -> size_t {
        return MessageTypes(Message()).index();
    }

    template<size_t Idx, typename Message>
    auto send_to(const Message& msg) -> bool {
        static_assert(Idx < TaskCount,
            "Invalid task index");
        if(!_initialized.at(Idx)) {
            return false;
        }
        std::get<Idx>(_handles)->send(msg);
        return true;
    }

  private:
    std::tuple<MessageQueues*...> _handles;
    std::array<std::atomic_bool, TaskCount> _initialized;
};

};