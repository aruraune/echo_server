add_executable(test_echo_server)

# Tests Source
target_sources(
  test_echo_server
  PRIVATE
    src/main.cpp
    src/business_rules.cpp
    src/concurrency.cpp
    src/cipher.cpp
    src/client_authenticator/test_client_authenticator.cpp
    src/client_crypto/test_client_crypto.cpp
    src/message_receiver/test_message_receiver.cpp
    src/message_sender/test_message_sender.cpp
)

target_link_libraries(test_echo_server PRIVATE echo_server_lib ${Boost_LIBRARIES} spdlog::spdlog)

add_test(NAME business_rules COMMAND test_echo_server -t business_rules)
add_test(NAME concurrency COMMAND test_echo_server -t concurrency)
add_test(NAME cipher COMMAND test_echo_server -t cipher)
