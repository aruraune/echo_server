#include "test_client_crypto.hpp"

namespace echo_server::crypto {

auto encrypt(crypto_message_params args, std::vector<std::byte> message)
    -> std::vector<std::byte> {
  return decrypt(args, message);
}

} // namespace echo_server::crypto
