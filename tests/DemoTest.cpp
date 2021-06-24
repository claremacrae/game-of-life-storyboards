#define APPROVAL_TESTS_HIDE_DEPRECATED_CODE 1

#include "ApprovalTests.hpp"
#include "catch2/catch.hpp"
#include "GameOfLife.h"
#include <ostream>
#include <iostream>

using namespace ApprovalTests;

void verifySequence(std::string initialFrame,
                    int numberOfFrames,
                    std::function<std::string(int)> function)
{
    std::stringstream s;
    s << "Initial Frame:\n";
    s << initialFrame << "\n\n";
    for (int frame = 1; frame <= numberOfFrames; ++frame)
    {
        std::cout << s.str() << std::endl;
        s << "Frame #" << frame << ":\n";
        s << function(frame) << "\n\n";
    }
    Approvals::verify(s.str());
}

TEST_CASE("Demo Sequence")
{
    GameOfLife game([](int x, int y) { return 1 <= x && x <= 3 && y == 2; });
    GameOfLife lastGame = game;
    verifySequence(
        game.print(5, 5), 5, [&](int frame) {
            lastGame = lastGame.advance();
//            return "Hi";
            return lastGame.print(5, 5);
        });
}
