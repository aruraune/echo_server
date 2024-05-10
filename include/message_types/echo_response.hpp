#pragma once

#include <cstdint>
#include <vector>

#include "message_base.hpp"

namespace echo_server::messages {

struct echo_response : public message_base {
  std::uint16_t message_size = {};
  std::vector<std::byte> plain_message;
};

} // namespace echo_server::messages
