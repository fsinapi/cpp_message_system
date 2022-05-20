#include <iostream>

#include "my_tasks.hpp"
using namespace message_queue;
using namespace messages;

auto main() -> int {
    MessageQueue<Task1Message> queue_1;
    MessageQueue<Task2Message> queue_2;

    auto tasklist = MyTasks();

    // Try to send before registering
    std::cout << "sending early: " << 
        tasklist.send_to<TaskIndex::Task1>(Message1{.id=505}) << 
        std::endl;

    // Register the queues
    tasklist.register_queue<TaskIndex::Task1>(queue_1);
    tasklist.register_queue<TaskIndex::Task2>(queue_2);

    // Can't register multiple times
    try {
        tasklist.register_queue<TaskIndex::Task2>(queue_2);
    } catch(std::bad_exception &e) {
        std::cout << "Tried to set same queue twice: " << e.what() << std::endl;
    }

    std::cout << "Queue1 index: " 
        << TaskIndex::Task1
        << std::endl;
    std::cout << "Queue2 index: " 
        << MyTasks::get_task_idx<Task2Message>() 
        << std::endl;

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

    // If a message is in multiple messsage definitions, must
    // disambiguate with a variant constructor
    tasklist.send(Task2Message(Message2{.id=1, .payload=2}));

    return 0;
}
