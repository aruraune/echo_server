#pragma once

#include "message_header.hpp"

#include <concepts>

namespace echo_server::messages {

struct message_base {
  message_header header;
};

template <typename T>
concept EchoServerMessage = std::derived_from<T, message_base>;

} // namespace echo_server::messages
