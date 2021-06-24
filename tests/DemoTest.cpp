#define APPROVAL_TESTS_HIDE_DEPRECATED_CODE 1

#include "ApprovalTests.hpp"
#include "catch2/catch.hpp"
#include <ostream>
#include <iostream>

using namespace ApprovalTests;

class GameOfLife
{
    std::vector<std::string> aliveCells;

public:
    GameOfLife(std::function<int(int x, int y)> function)
    {
        for (int x = -1; x <= 6; ++x)
        {
            for (int y = -1; y <= 6; ++y)
            {
                if (function(x, y))
                {
                    aliveCells.push_back(printCoordinate(x, y));
                }
            }
        }
    }

    bool isAlive(int x, int y)
    {
        return (std::find(aliveCells.begin(),
                          aliveCells.end(),
                          printCoordinate(x, y)) != aliveCells.end());
    }

    std::string printCoordinate(int x, int y) const
    {
        return std::to_string(x) + "," + std::to_string(y);
    }

    std::string printCell(int x, int y)
    {
        return isAlive(x, y) ? "X" : ".";
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

    GameOfLife advance()
    {
//        auto function1 = function_;
        std::function<int(int, int)> function2 = [this](int x, int y)
        {
            int count =
                this->isAlive(x - 1, y - 1) +
                this->isAlive(x - 1, y - 0) +
                this->isAlive(x - 1, y + 1) +

                this->isAlive(x - 0, y - 1) +
//                this->isAlive(x - 0, y - 0) +
                this->isAlive(x - 0, y + 1) +

                this->isAlive(x + 1, y - 1) +
                this->isAlive(x + 1, y - 0) +
                this->isAlive(x + 1, y + 1);
            return count == 3 || (count == 2 && this->isAlive(x, y));
        };
        std::cout << "advance" << std::endl;
        //        function_ = function2;
        return GameOfLife(function2);
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
