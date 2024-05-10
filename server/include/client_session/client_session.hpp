#pragma once

#include <cstdint>
#include <string>

namespace echo_server {

struct client_session {
  std::string uuid;
  std::string address;

  bool is_logged_in = false;

  std::uint8_t username_sum = {};
  std::uint8_t password_sum = {};
};

} // namespace echo_server
