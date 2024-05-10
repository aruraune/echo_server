#pragma once

#include <stdexcept>

namespace echo_server::exceptions {

class server_error : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

} // namespace echo_server::exceptions
