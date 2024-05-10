#pragma once

#include <cstdint>

namespace echo_server::crypto {

struct crypto_message_params {
  std::uint8_t username_sum;
  std::uint8_t password_sum;
  std::uint8_t sequence;
};

} // namespace echo_server::crypto
