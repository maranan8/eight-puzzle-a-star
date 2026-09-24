#pragma once

#include <vector>
#include <array>

class Puzzle {
public:
    using Board = std::array<int, 9>;

    enum class Move {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    explicit Puzzle(const Board& board);

    bool isSolvable() const;
    bool isGoalState() const;
    void printPuzzle() const;
    const Board& getBoard() const;
    std::vector<std::pair<Puzzle, Puzzle::Move>> getNeighbors() const;
private:
    Board board;

    static constexpr int goalState[9] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8
    };
};