#pragma once

#include <cstdint>
#include <memory>

#include "client_authenticator/client_authenticator.hpp"

namespace echo_server {

struct echo_server_config {
  std::uint16_t port = {};
  bool enable_decryption = true;

  std::shared_ptr<auth::client_authenticator> authenticator;
};

} // namespace echo_server
