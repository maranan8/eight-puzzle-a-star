#pragma once

#include "Puzzle.hpp"
#include <vector>

class A_Star {
public:
    enum class Heuristic {
        NONE,
        MISPLACED_TILES,
        MANHATTAN_DISTANCE
    };

    struct Node {
        Puzzle puzzle;
        int g, h;
        int parentIndex;

        int getF() const {return g + h;}
    };    
    
    std::vector<Node> findPath(const Puzzle& start, Heuristic heuristic);
    int getSeachCost() const {return searchCost;} 
private:
    int searchCost = 0;

    int calculateHeuristic(const Puzzle::Board& board, Heuristic heuristic) const;
    int heuristic1(const Puzzle::Board& board) const; // number of misplaced tiles
    int heuristic2(const Puzzle::Board& board) const; // Manhattan distance

    std::vector<A_Star::Node> reconstructPath(const std::vector<Node>& nodes, int goalIndex) const;
};