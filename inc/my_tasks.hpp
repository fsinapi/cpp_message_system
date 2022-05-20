
#include "tasks.hpp"
#include "messages.hpp"
#include "message_queue.hpp"

using Queue1 = message_queue::MessageQueue<messages::Task1Message>;
using Queue2 = message_queue::MessageQueue<messages::Task2Message>;

using MyTasks = tasks::Tasks<Queue1, Queue2>;

enum TaskIndex : std::size_t {
    Task1 = MyTasks::get_task_idx<Queue1::Message>(),
    Task2 = MyTasks::get_task_idx<Queue2::Message>(),
    TaskNumber // Number of tasks
};

static_assert(TaskNumber == MyTasks::TaskCount,
    "TaskIndex count is wrong");
