#pragma once

#include <vector>

#include "message_base.hpp"

namespace echo_server::messages {

struct echo_request : public message_base {
  std::uint16_t message_size = {};
  std::vector<std::byte> cipher_message;
};

} // namespace echo_server::messages
