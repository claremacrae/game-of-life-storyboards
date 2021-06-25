#pragma once

#include <iostream>
#include <ostream>
#include "catch2/catch.hpp"
#include "ApprovalTests.hpp"
#include "GameOfLife.h"
class GameOfLife
{
    std::vector<std::string> aliveCells;
    std::string aliveCharacter = "X";
    std::string deadCharacter = ".";

public:
    explicit GameOfLife(std::function<int(int x, int y)> function)
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

    bool isAlive(int x, int y) const
    {
        return (std::find(aliveCells.begin(),
                          aliveCells.end(),
                          printCoordinate(x, y)) != aliveCells.end());
    }

    std::string printCoordinate(int x, int y) const
    {
        return std::to_string(x) + "," + std::to_string(y);
    }

    std::string printCell(int x, int y) const
    {
        return isAlive(x, y) ? aliveCharacter : deadCharacter;
    }

    std::string print(int width, int height) const
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

    GameOfLife advance() const
    {
        std::function<int(int, int)> function2 = [this](int x, int y)
        {
            // clang-format off
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
            // clang-format on
            return count == 3 || (count == 2 && this->isAlive(x, y));
        };
        auto newGame = GameOfLife(function2);
        newGame.setAliveCell(aliveCharacter);
        newGame.setDeadCell(deadCharacter);
        return newGame;
    }

    std::string setAliveCell(std::string alive)
    {
        aliveCharacter = alive;
        return aliveCharacter;
    }

    std::string setDeadCell(std::string dead)
    {
        deadCharacter = dead;
        return deadCharacter;
    }
};
