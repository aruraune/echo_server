#pragma once

#include <cstdint>

#include "message_type.hpp"

namespace echo_server::messages {

struct message_header {
  std::uint16_t total_size = {};
  message_type type = {};
  std::uint8_t sequence = {};
};

} // namespace echo_server::messages
