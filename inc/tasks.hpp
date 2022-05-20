/**
 * @file tasks.hpp
 * @brief Aggregates task queues
 */

#include <atomic>
#include <tuple>
#include <variant>

namespace tasks {

template<typename Q>
concept MsgQueue = requires(Q queue) {
    {Q::Message};
    {queue.send(Q::Message)} -> std::same_as<void>;
    {queue.try_read()} -> std::same_as<typename Q::Message>;
};

template<typename... MessageQueues>
class Tasks {
  public:
    using MessageTypes = 
        std::variant<typename MessageQueues::Message...>;
    static constexpr size_t TaskCount = std::variant_size_v<MessageTypes>;

    Tasks(MessageQueues&... handles) :
        _handles(handles...) {}
    
    template<typename Message>
    constexpr static auto get_task_idx() -> size_t {
        return MessageTypes(Message()).index();
    }

    template<size_t task, typename Message>
    auto send_to(const Message& msg) -> void {
        static_assert(task < TaskCount,
            "Invalid task index");
        std::get<task>(_handles).send(msg);
    }

  private:
    std::tuple<MessageQueues&...> _handles;
};

};