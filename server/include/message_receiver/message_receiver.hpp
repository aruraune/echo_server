#pragma once

#include <boost/asio/awaitable.hpp>

#include "client_channel/client_channel.hpp"
#include "message_types/message_base.hpp"
#include "message_types/message_header.hpp"

namespace echo_server {

[[nodiscard]] auto receive_header(client_channel& channel)
    -> boost::asio::awaitable<messages::message_header>;

template <messages::EchoServerMessage T>
[[nodiscard]] auto receive_message(client_channel& channel,
                                   messages::message_header header)
    -> boost::asio::awaitable<T>;

} // namespace echo_server
