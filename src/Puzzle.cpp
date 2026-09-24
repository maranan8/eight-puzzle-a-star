#include "../include/Puzzle.hpp"

#include <vector>
#include <iostream>
#include <algorithm>

Puzzle::Puzzle(const Board& board) : board(board) {}

const Puzzle::Board& Puzzle::getBoard() const {
    return board;
}

bool Puzzle::isSolvable() const {
    std::vector<int> arr;
    for (std::size_t i = 0; i < 9; i++) {
        if (board[i] == 0) {
            continue;
        }
        arr.push_back(board[i]);
    }

    int inversions = 0;
    for (std::size_t i = 0; i < 8; i++) {
        for (std::size_t j = i+1; j < 8; j++) {
            if (arr[i] > arr[j]) {
                inversions++;
            }
        }
    }
    
    return inversions % 2 == 0;
}

bool Puzzle::isGoalState() const {
    return std::equal(std::begin(board), std::end(board), std::begin(goalState));
}

void Puzzle::printPuzzle() const {
    for (std::size_t i = 0; i < 9; i++) {
        if (i % 3 == 0) {
            std::cout << std::endl;
        }
        std::cout << board[i] << " ";
    }
    std::cout << std::endl;
}

std::vector<std::pair<Puzzle, Puzzle::Move>> Puzzle::getNeighbors() const {
    std::vector<std::pair<Puzzle, Move>> neighbors;

    auto blankIterator = std::find(board.begin(), board.end(), 0);
    int blankPosition = static_cast<int>(std::distance(board.begin(), blankIterator));

    int blankRow = blankPosition / 3;
    int blankColumn = blankPosition % 3;

    auto addNeighbor = [&](int targetPosition, Move move) {
        Board nextBoard = board;

        std::swap(
            nextBoard[blankPosition],
            nextBoard[targetPosition]
        );

        neighbors.emplace_back(
            Puzzle(nextBoard),
            move
        );
    };

    if (blankRow > 0) {
        addNeighbor(
            blankPosition - 3,
            Move::UP
        );
    }

    if (blankRow < 2) {
        addNeighbor(
            blankPosition + 3,
            Move::DOWN
        );
    }

    if (blankColumn > 0) {
        addNeighbor(
            blankPosition - 1,
            Move::LEFT
        );
    }

    if (blankColumn < 2) {
        addNeighbor(
            blankPosition + 1,
            Move::RIGHT
        );
    }

    return neighbors;
}