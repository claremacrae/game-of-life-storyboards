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
        s << "Frame #" << frame << ":\n";
        s << function(frame) << "\n\n";
    }
    Approvals::verify(s.str());
}

class StoryBoard
{
private:
    std::stringstream s;
    int frameCount = 0;
public:
    StoryBoard& add(std::string frame)
    {
        if (frameCount == 0)
        {
            return add("Initial Frame", frame);
        }
        else
        {
            return add("Frame #" + std::to_string(frameCount), frame);
        }
    }

    StoryBoard& add(std::string title, std::string frame)
    {
        s << title << ":\n";
        s << frame << "\n\n";
        frameCount += 1;
        return *this;
    }

    StoryBoard& addFrames(
                    int numberOfFrames,
                    std::function<std::string(int)> function)
    {
        for (int frame = 1; frame <= numberOfFrames; ++frame)
        {
            add(function(frame));
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const StoryBoard& board)
    {
        os << board.s.str();
        return os;
    }

};

TEST_CASE("Demo Sequence")
{
    {
        GameOfLife game([](int x, int y)
                        { return 1 <= x && x <= 3 && y == 2; });
        GameOfLife lastGame = game;

        Approvals::verify(StoryBoard()
                              .add(game.print(5, 5))
                              .addFrames(5,
                                         [&](int frame)
                                         {
                                             lastGame = lastGame.advance();
                                             return lastGame.print(5, 5);
                                         }));
    }

    {
        GameOfLife game([](int x, int y)
                        { return 1 <= x && x <= 3 && y == 2; });

        StoryBoard story;

        story.add(game.print(5, 5));
        for (int i = 0; i < 5; ++i)
        {
            game = game.advance();
            story.add(game.print(5, 5));
        }
        Approvals::verify(story);
    }
}

TEST_CASE("Other Story Board Mechanisms")
{
    GameOfLife game([](int x, int y) { return 1 <= x && x <= 3 && y == 2; });

    StoryBoard story;

    story.add(game.print(5, 5));
    game = game.advance();
    story.add("Start game", game.print(5, 5));
    game = game.advance();
    story.add(game.print(5, 5));
    game = game.advance();
    story.add(game.print(5, 5));
    game = game.advance();
    story.add(game.print(5, 5));
    game = game.advance();
    story.add(game.print(5, 5));
    Approvals::verify(story);
}
