#include <boost/asio/co_spawn.hpp>
#include <boost/asio/deferred.hpp>
#include <boost/asio/experimental/parallel_group.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/test/framework.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>

#include "client_authenticator/allow_all_client_authenticator.hpp"
#include "client_channel/client_channel.hpp"
#include "client_crypto/test_client_crypto.hpp"
#include "echo_server/echo_server.hpp"
#include "message_receiver/message_receiver.hpp"
#include "message_sender/test_message_sender.hpp"
#include "message_types/echo_request.hpp"
#include "message_types/echo_response.hpp"
#include "message_types/login_request.hpp"
#include "message_types/login_response.hpp"
#include "echo_status/login_status.hpp"

namespace echo_server::test {

inline constexpr auto test_tcp_port = std::uint16_t{31217};

BOOST_AUTO_TEST_SUITE(concurrency)

BOOST_AUTO_TEST_CASE(concurrent_clients) {
  constexpr auto num_clients = std::size_t{500};

  spdlog::set_level(spdlog::level::debug);

  auto logger = spdlog::default_logger()->clone(fmt::format(
      "test:{}",
      boost::unit_test::framework::current_test_case().p_name->c_str()));

  auto io_context = boost::asio::io_context{1};

  echo_server::spawn_server(
      io_context.get_executor(),
      {
          .port = test_tcp_port,
          .enable_decryption = true,
          .authenticator =
              echo_server::auth::allow_all_client_authenticator::create(),
      });

  auto client_task = [&]() -> boost::asio::awaitable<void> {
    const auto username =
        boost::uuids::to_string(boost::uuids::random_generator{}())
            .substr(0, 8);
    const auto password =
        boost::uuids::to_string(boost::uuids::random_generator{}())
            .substr(0, 8);

    auto socket = boost::asio::ip::tcp::socket{io_context};

    co_await socket.async_connect(
        {boost::asio::ip::address::from_string("127.0.0.1"), test_tcp_port},
        boost::asio::use_awaitable);

    auto channel = client_channel{std::move(socket)};

    const auto login_request_sequence =
        static_cast<std::uint8_t>(std::rand() % 256);

    co_await send_message<messages::login_request>{}(
        channel, login_request_sequence, username, password);

    auto login_response_header = co_await receive_header(channel);
    BOOST_CHECK(login_response_header.type ==
                messages::message_type::LOGIN_RESPONSE);
    BOOST_CHECK(login_response_header.sequence == login_request_sequence);

    const auto login_response =
        co_await receive_message<messages::login_response>(
            channel, std::move(login_response_header));

    logger->debug(
        "Login status code: {}",
        static_cast<
            std::underlying_type_t<decltype(login_response.status_code)>>(
            login_response.status_code));

    BOOST_CHECK(login_response.status_code == echo_status::login_status::OK);
    BOOST_CHECK(login_response.header.sequence == login_request_sequence);

    const auto echo_message = std::string{"This is an EchoServer unit test."};

    auto echo_message_data = std::vector<std::byte>{echo_message.size()};

    std::transform(echo_message.begin(), echo_message.end(),
                   echo_message_data.begin(),
                   [](char each) { return static_cast<std::byte>(each); });

    const auto echo_request_sequence =
        static_cast<std::uint8_t>(std::rand() % 256);

    const auto echo_message_encrypted = crypto::encrypt(
        {
            .username_sum = crypto::calculate_checksum(username),
            .password_sum = crypto::calculate_checksum(password),
            .sequence = echo_request_sequence,
        },
        echo_message_data);

    logger->debug("Requesting echo for encrypted message: {:X}",
                  spdlog::to_hex(echo_message_encrypted));

    co_await send_message<messages::echo_request>{}(
        channel, echo_request_sequence, echo_message_encrypted);

    auto echo_response_header = co_await receive_header(channel);
    BOOST_CHECK(echo_response_header.type ==
                messages::message_type::ECHO_RESPONSE);
    BOOST_CHECK(echo_response_header.sequence == echo_request_sequence);

    auto echo_response = co_await receive_message<messages::echo_response>(
        channel, std::move(echo_response_header));
    BOOST_CHECK(echo_response.header.sequence == echo_request_sequence);
    BOOST_CHECK(echo_response.plain_message == echo_message_data);

    echo_response.plain_message.push_back(std::byte{'\0'});

    auto echo_response_string = std::string{
        reinterpret_cast<const char*>(echo_response.plain_message.data())};
    BOOST_CHECK(echo_response_string == echo_message);
  };

  auto client_task_runner = [&]() -> boost::asio::awaitable<void> {
    BOOST_CHECK_NO_THROW(
        try { co_await client_task(); } catch (const std::exception& error) {
          logger->error("Fatal error: {}", error.what());
          throw;
        });
  };

  boost::asio::co_spawn(
      io_context,
      [&]() -> boost::asio::awaitable<void> {
        auto tasks = std::vector<decltype(boost::asio::co_spawn(
            io_context, client_task_runner, boost::asio::deferred))>{};
        tasks.reserve(num_clients);

        for (auto i = std::size_t{0}; i < num_clients; ++i) {
          tasks.emplace_back(boost::asio::co_spawn(
              io_context, client_task_runner, boost::asio::deferred));
        }

        co_await boost::asio::experimental::make_parallel_group(tasks)
            .async_wait(boost::asio::experimental::wait_for_all(),
                        boost::asio::use_awaitable);

        io_context.stop();
      },
      [](std::exception_ptr error) {
        if (error) {
          std::rethrow_exception(error);
        }
      });

  io_context.run();
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace echo_server::test
