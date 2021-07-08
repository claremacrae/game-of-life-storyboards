#pragma once

#include <iostream>
#include <ostream>
#include <utility>
#include "catch2/catch.hpp"
#include "ApprovalTests.hpp"
#include "GameOfLife.h"

struct Point
{
    Point(int x, int y) : x_(x), y_(y)
    {
    }

    bool operator==(const Point& rhs) const
    {
        return x_ == rhs.x_ && y_ == rhs.y_;
    }

    bool operator!=(const Point& rhs) const
    {
        return !(rhs == *this);
    }
    int x_;
    int y_;
};

class GameOfLife
{
    std::vector<Point> aliveCells;
    std::string aliveCharacter = "X";
    std::string deadCharacter = ".";

public:
    explicit GameOfLife(const std::function<int(int, int)>& function)
    {
        std::vector<Point> points;
        for (int x = -1; x <= 6; ++x)
        {
            for (int y = -1; y <= 6; ++y)
            {
                const Point point = Point(x, y);
                points.push_back(point);
            }
        }
        *this = GameOfLife(function, points);
    }

    GameOfLife(const std::function<int(int, int)>& function,
               std::vector<Point>& points)
    {
        for (const auto& point : points)
        {
            if (function(point.x_, point.y_))
            {
                aliveCells.push_back(point);
            }
        }
    }

    bool isAlive(int x, int y) const
    {
        return (std::find(aliveCells.begin(),
                          aliveCells.end(),
                          Point(x, y)) != aliveCells.end());
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
        aliveCharacter = std::move(alive);
        return aliveCharacter;
    }

    std::string setDeadCell(std::string dead)
    {
        deadCharacter = std::move(dead);
        return deadCharacter;
    }
};
