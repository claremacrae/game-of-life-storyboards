#define APPROVAL_TESTS_HIDE_DEPRECATED_CODE 1

#include "ApprovalTests.hpp"
#include "catch2/catch.hpp"
#include <ostream>

using namespace ApprovalTests;

class GameOfLife
{
    std::function<int(int, int)> function_;

public:
    GameOfLife(std::function<int(int x, int y)> function) : function_(function)
    {
    }

    std::string printCell(int x, int y)
    {
        return function_(x, y) ? "X" : ".";
    }

    std::string print(int width, int height)
    {
        std::stringstream s;
        for (int y = 0; y < width; ++y)
        {
            for (int x = 0; x < height; ++x)
            {
                s << printCell(x, y) << " ";
            }
            s << '\n';
        }
        return s.str();
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
    GameOfLife game([](int x, int y) { return 1 <= x && x <= 3 && y == 2; });
    verifySequence(
        game.print(5, 5), 5, [&](int frame) { return game.print(5, 5); });
}
