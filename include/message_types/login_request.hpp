#pragma once

#include <string>

#include "message_base.hpp"

namespace echo_server::messages {

struct login_request : public message_base {
  std::string username;
  std::string password;
};

} // namespace echo_server::messages
