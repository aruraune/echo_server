#pragma once

#include <string_view>

namespace echo_server::auth {

class client_authenticator {
public:
  virtual auto authenticate(std::string_view username,
                            std::string_view password) -> void = 0;
};

} // namespace echo_server::auth
