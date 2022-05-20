#include <variant>
#include <cstdint>
#include <concepts>

namespace messages {

struct Message1 {
    uint32_t id;
};

struct Message2 {
    uint32_t id;
    uint8_t payload;
};

struct Message3 {
    uint32_t id;
    float payload;
};

using Task1Message = std::variant<std::monostate, Message1, Message2>;
using Task2Message = std::variant<std::monostate, Message2, Message3>;

using Messages = std::variant<Task1Message, Task2Message>;

}; 