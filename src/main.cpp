#include <iostream>

#include "my_tasks.hpp"
using namespace message_queue;
using namespace messages;
using namespace mytasks;

auto main() -> int {
    MessageQueue<Task1Message> queue_1;
    Queue2 queue_2;
    Queue3 queue_3;

    auto tasklist = MyTasks();

    // Try to send before registering
    std::cout << "sending early: " << 
        tasklist.send_to<TaskIndex::Task1>(Message1{.id=505}) << 
        std::endl;

    // Register the queues
    tasklist.register_queue(queue_1);
    tasklist.register_queue(queue_2);
    tasklist.register_queue(queue_3);

    // Can't register multiple times
    try {
        tasklist.register_queue(queue_2);
    } catch(std::bad_exception &e) {
        std::cout << "Tried to set same queue twice: " << e.what() << std::endl;
    }

    std::cout << "Queue2 index: " 
        << MyTasks::get_task_idx<Queue2>() 
        << std::endl;
#if 0
    // Send message to Queue 1
    tasklist.send_to<TaskIndex::Task1>(Message1{.id=505});

    auto rcv1 = queue_1.try_read();
    std::cout << "Received: " << rcv1.has_value() << std::endl;
    {
        auto msg = rcv1.value();
        std::cout << "Message 1 idx: " << msg.index() << std::endl;
    }

    tasklist.send_to<TaskIndex::Task2>(Message3{.id=0, .payload=4});
    auto rcv2 = queue_2.try_read();
    {
        auto msg = rcv2.value();
        std::cout << "Message 2 idx: " << msg.index() << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Sending message with deduced mailbox" << std::endl;
    tasklist.send(Message1{.id=999});
    auto rcv3 = queue_1.try_read();
    {
        std::cout << "Received: " << rcv3.has_value() << std::endl;
        auto msg = rcv3.value();
        std::cout << "Message 3 idx: " << msg.index() << std::endl;
        auto val = std::get<Message1>(msg).id;
        std::cout << "Message 3 value: " << val << std::endl;
    }
#endif 
    // works good
    tasklist.send<Queue2::Tag>((Message2{.id=1, .payload=2}));

    return 0;
}
