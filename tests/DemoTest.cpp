#include "ApprovalTests.hpp"
#include "catch2/catch.hpp"
#include "GameOfLife.h"
#include <ostream>

using namespace ApprovalTests;

class StoryBoard
{
private:
    std::stringstream output_;
    int frameCount_ = 0;
    bool addNewLineBeforeNextFrame_ = false;

public:
    StoryBoard& addDescription(const std::string& description)
    {
        output_ << description << "\n";
        addNewLineBeforeNextFrame_ = true;
        return *this;
    }

    StoryBoard& addDescriptionWithData(const std::string& description,
                                       const std::string& data)
    {
        output_ << description << ": " << data << "\n";
        addNewLineBeforeNextFrame_ = true;
        return *this;
    }

    StoryBoard& addFrame(const std::string& frame)
    {
        if (frameCount_ == 0)
        {
            return addFrame("Initial Frame", frame);
        }
        else
        {
            return addFrame("Frame #" + std::to_string(frameCount_), frame);
        }
    }

    StoryBoard& addFrame(const std::string& title, const std::string& frame)
    {
        if (addNewLineBeforeNextFrame_)
        {
            output_ << '\n';
            addNewLineBeforeNextFrame_ = false;
        }
        output_ << title << ":\n";
        output_ << frame << "\n\n";
        frameCount_ += 1;
        return *this;
    }

    StoryBoard& addFrames(int numberOfFrames,
                          const std::function<std::string(int)>& function)
    {
        for (int frame = 1; frame <= numberOfFrames; ++frame)
        {
            addFrame(function(frame));
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const StoryBoard& board)
    {
        os << board.output_.str();
        return os;
    }
};

TEST_CASE("Demo Sequence")
{
    {
        GameOfLife game(5, 5, [](int x, int y)
                        { return 1 <= x && x <= 3 && y == 2; });

        Approvals::verify(StoryBoard()
                              .addFrame(game.print(5, 5))
                              .addFrames(5,
                                         [&](int frame)
                                         {
                                             game = game.advance();
                                             return game.print(5, 5);
                                         }));
    }

    {
        GameOfLife game(5, 5, [](int x, int y)
                        { return 1 <= x && x <= 3 && y == 2; });

        StoryBoard story;

        story.addFrame(game.print(5, 5));
        for (int i = 0; i < 5; ++i)
        {
            game = game.advance();
            story.addFrame(game.print(5, 5));
        }
        Approvals::verify(story);
    }
}

TEST_CASE("Other Story Board Mechanisms")
{
    GameOfLife game(5, 5, [](int x, int y) { return 1 <= x && x <= 3 && y == 2; });

    StoryBoard story;
    story.addDescription("Game of Life");
    story.addFrame(game.print(5, 5));

    game = game.advance();
    story.addFrame("Start game", game.print(5, 5));

    game = game.advance();
    story.addFrame(game.print(5, 5));

    story.addDescriptionWithData("setting alive", game.setAliveCell("*"));
    story.addDescriptionWithData("setting dead", game.setDeadCell("_"));
    game = game.advance();
    story.addFrame(game.print(5, 5));

    game = game.advance();
    story.addFrame(game.print(5, 5));

    game = game.advance();
    story.addFrame(game.print(5, 5));

    Approvals::verify(story);
}

TEST_CASE("Demo 15-step blinker")
{
    int width = 11;
    int height = 18;
    {
        int x = 3;
        int y = 4;
        std::vector<Point> alive = {
            Point(x + 1,y + 1),
            Point(x + 2,y + 1),
            Point(x + 3,y + 1),
            Point(x + 1,y + 2),
//            Point(x + 2,y + 2),
            Point(x + 3,y + 2),
            Point(x + 1,y + 3),
            Point(x + 2,y + 3),
            Point(x + 3,y + 3),
            Point(x + 1,y + 4),
            Point(x + 2,y + 4),
            Point(x + 3,y + 4),
            Point(x + 1,y + 5),
            Point(x + 2,y + 5),
            Point(x + 3,y + 5),
            Point(x + 1,y + 6),
            Point(x + 2,y + 6),
            Point(x + 3,y + 6),
            Point(x + 1,y + 7),
//            Point(x + 2,y + 7),
            Point(x + 3,y + 7),
            Point(x + 1,y + 8),
            Point(x + 2,y + 8),
            Point(x + 3,y + 8),
        };
        GameOfLife game(alive);

        Approvals::verify(StoryBoard()
                              .addFrame(game.print(width, height))
                              .addFrames(15,
                                         [&](int frame)
                                         {
                                             game = game.advance();
                                             return game.print(width, height);
                                         }));
    }

    {
        auto alive = R"(
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . X X X .
. . . . X . X .
. . . . X X X .
. . . . X X X .
. . . . X X X .
. . . . X X X .
. . . . X . X .
. . . . X X X . )";

        GameOfLife game(alive);

        Approvals::verify(StoryBoard()
                              .addFrame(game.print(width, height))
                              .addFrames(15,
                                         [&](int frame)
                                         {
                                             game = game.advance();
                                             return game.print(width, height);
                                         }));
    }
}

