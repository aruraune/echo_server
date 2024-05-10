#pragma once

#include <cstdint>

namespace echo_server::echo_status {

enum class login_status : std::uint16_t { FAILED = 0, OK = 1 };

} // namespace echo_server::echo_status
