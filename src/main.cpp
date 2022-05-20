#include <iostream>

#include "my_tasks.hpp"
using namespace message_queue;
using namespace messages;

auto main() -> int {
    MessageQueue<Task1Message> queue_1;
    MessageQueue<Task2Message> queue_2;

    auto tasklist = MyTasks(queue_1, queue_2);

    std::cout << "Queue1: " << static_cast<size_t>(TaskIndex::Task1) << std::endl;
    std::cout << "Queue2: " << MyTasks::get_task_idx<Task2Message>() << std::endl;

    // Send message to Queue 1
    tasklist.send_to<TaskIndex::Task1>(Message1{.id=505});

    auto rcv = queue_1.try_read();
    std::cout << "Received: " << rcv.has_value() << std::endl;

    tasklist.send_to<TaskIndex::Task2>(Message3{.id=0, .payload=4});
    
    return 0;
}