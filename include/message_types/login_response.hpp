#pragma once

#include "message_base.hpp"
#include "echo_status/login_status.hpp"

namespace echo_server::messages {

struct login_response : public message_base {
  echo_status::login_status status_code = {};
};

} // namespace echo_server::messages
