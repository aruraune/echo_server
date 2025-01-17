#pragma once

#include <cstdint>

namespace echo_server::messages {

enum class message_type : std::uint8_t {
  LOGIN_REQUEST = 0,
  LOGIN_RESPONSE = 1,
  ECHO_REQUEST = 2,
  ECHO_RESPONSE = 3
};

} // namespace echo_server::messages
