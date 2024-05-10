#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include "crypto_message_params.hpp"

namespace echo_server::crypto {

auto calculate_checksum(std::string_view data) -> std::uint8_t;

auto decrypt(crypto_message_params args, std::vector<std::byte> message)
    -> std::vector<std::byte>;

} // namespace echo_server::crypto
