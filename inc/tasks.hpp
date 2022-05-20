/**
 * @file tasks.hpp
 * @brief Aggregates task queues
 */

#include <atomic>
#include <tuple>
#include <variant>
#include <memory>
#include <optional>

namespace tasks {

template<typename Q>
concept MsgQueue = requires(Q queue, Q::Message msg) {
    { queue.send(msg) } -> std::same_as<void>;
    { queue.try_read() } -> std::same_as<std::optional<typename Q::Message>>;
};

/**
 * @brief Class to encapsulate a tasklist. This class functions as a central
 * mail-forwarding system in essence, redirecting messages to their 
 * appropriate messages.
 * 
 * @tparam MessageQueues 
 */
template<MsgQueue... MessageQueues>
class Tasks {
  public:
    using MessageTypes = 
        std::variant<typename MessageQueues::Message...>;
    
    static constexpr size_t TaskCount = std::variant_size_v<MessageTypes>;

    Tasks() :
        _handles(),
        _initialized{false} {}
    
    /**
     * @brief Register a queue handle with the task list
     * 
     * @tparam Idx The index of this queue
     * @tparam Queue Type of the message queue
     * @param queue Handle to the queue
     */
    template<size_t Idx, MsgQueue Queue>
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
    
    /**
     * @brief Get the index of a queue, as generated by the Task List
     * 
     * @tparam Message The type of the message
     * @return size_t A unique index for this message type
     */
    template<typename Message>
    constexpr static auto get_task_idx() -> size_t {
        return MessageTypes(Message()).index();
    }

    /**
     * @brief Send a message
     * 
     * @tparam Idx The index of the message to send to, as returned by
     *             \ref get_task_idx()
     * @tparam Message The type of message to send
     * @param msg The message to send
     * @return true if the message could be sent, false otherwise
     */
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

    /**
     * @brief Send a message and automatically deduce the mailbox
     * to forward it to.
     * 
     * @tparam Message The type of message to send
     * @param msg The message to send
     * @return true if the message could be sent, false otherwise
     */
    template<typename Message>
    auto send(const Message& msg) -> bool {
        constexpr auto idx = get_task_idx<Message>();
        return send_to<idx>(msg);
    }

  private:
    // Handle for each of the tasks
    std::tuple<MessageQueues*...> _handles;
    // Array mapping whether a task handle is initialized
    std::array<std::atomic_bool, TaskCount> _initialized;
};

};
