#include "client_authenticator/allow_all_client_authenticator.hpp"

#include <spdlog/spdlog.h>

namespace echo_server::auth {

[[nodiscard]] inline auto logger() -> std::shared_ptr<spdlog::logger> {
  static auto logger =
      spdlog::default_logger()->clone("allow_all_client_authenticator");
  return logger;
}

auto allow_all_client_authenticator::authenticate(
    std::string_view username, [[maybe_unused]] std::string_view password)
    -> void {
  logger()->info("Authenticated user: {}", username);
}

} // namespace echo_server::auth
