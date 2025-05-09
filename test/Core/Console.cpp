#include <catch2/catch_all.hpp>
#include "Core/Console.h"

TEST_CASE("ConsoleLogBuffer Basic Operations", "[console]") {
    // Test setup - ideally should be in a fixture with Clear() capability
    // Currently requires knowledge of implementation details:
    Core::ConsoleLogBuffer::Clear();

    SECTION("Initial state is empty") {
        auto& logs = Core::ConsoleLogBuffer::GetMessages();
        REQUIRE(logs.empty());
    }

    SECTION("Adding first message") {
        Core::ConsoleLogBuffer::AddMessage("Test message 1");
        
        auto& logs = Core::ConsoleLogBuffer::GetMessages();
        REQUIRE(logs.size() == 1);
        REQUIRE(logs[0].message == "Test message 1");
        REQUIRE_FALSE(logs[0].timestamp.empty());
    }

    SECTION("Messages respect maximum capacity") {
        const int maxMessages = 100;
        
        // Fill the buffer
        for(int i = 0; i < maxMessages; ++i) {
            Core::ConsoleLogBuffer::AddMessage("Msg " + std::to_string(i));
        }
        
        auto& logs = Core::ConsoleLogBuffer::GetMessages();
        REQUIRE(logs.size() == maxMessages);
        
        // Add one more message to trigger overwrite
        Core::ConsoleLogBuffer::AddMessage("Overflow message");
        
        REQUIRE(logs.size() == maxMessages);
        REQUIRE(logs[0].message == "Overflow message");
    }

    SECTION("Timestamps have valid format") {
        Core::ConsoleLogBuffer::AddMessage("Timestamp test");
        auto& logs = Core::ConsoleLogBuffer::GetMessages();
        
        // Basic timestamp format validation (HH:MM:SS)
        REQUIRE(logs[0].timestamp.size() == 8);
        REQUIRE(logs[0].timestamp[2] == ':');
        REQUIRE(logs[0].timestamp[5] == ':');
    }

    SECTION("Circular buffer behavior") {
        const int maxMessages = 100;
        
        // Add messages to fill buffer + 1
        for(int i = 0; i < maxMessages + 1; ++i) {
            Core::ConsoleLogBuffer::AddMessage("Msg " + std::to_string(i));
        }
        
        auto& logs = Core::ConsoleLogBuffer::GetMessages();
        
        // Verify first message was overwritten
        REQUIRE(logs[0].message == "Msg " + std::to_string(maxMessages));
        // Verify second message remains
        REQUIRE(logs[1].message == "Msg 1");
    }
}
