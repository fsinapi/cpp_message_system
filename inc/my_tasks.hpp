
#include "tasks.hpp"
#include "messages.hpp"
#include "message_queue.hpp"

namespace mytasks {

using Queue1 = message_queue::MessageQueue<messages::Task1Message>;
using Queue2 = message_queue::MessageQueue<messages::Task2Message, 0>;
using Queue3 = message_queue::MessageQueue<messages::Task3Message, 1>;

using MyTasks = tasks::Tasks<Queue1, Queue2, Queue3>;

enum TaskIndex : std::size_t {
    Task1 = MyTasks::get_task_idx<Queue1>(),
    Task2 = MyTasks::get_task_idx<Queue2>(),
    Task3 = MyTasks::get_task_idx<Queue3>(),
    TaskNumber // Number of tasks
};

static_assert(TaskNumber == MyTasks::TaskCount,
    "TaskIndex count is wrong");

};
