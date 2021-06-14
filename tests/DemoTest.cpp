#define APPROVAL_TESTS_HIDE_DEPRECATED_CODE 1

#include "ApprovalTests.hpp"
#include "catch2/catch.hpp"
#include <ostream>

using namespace ApprovalTests;

class GameOfLife
{

public:
    std::string print(int width, int height)
    {
        return ". . . . .";
    }
};

void verifySequence(std::string initialFrame,
                    int numberOfFrames,
                    std::function<std::string(int)> function)
{
    std::stringstream s;
    s << "Initial Frame:\n";
    s << initialFrame << "\n\n";
    for (int frame = 1; frame <= numberOfFrames; ++frame)
    {
        s << "Frame #" << frame << ":\n";
        s << function(frame) << "\n\n";
    }
    Approvals::verify(s.str());
}

TEST_CASE("Demo Sequence")
{
    GameOfLife game;
    verifySequence(
        game.print(5, 5), 5, [&](int frame) { return game.print(5, 5); });
}
