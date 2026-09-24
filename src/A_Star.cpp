#include "../include/Puzzle.hpp"
#include "../include/A_Star.hpp"
#include <cmath>
#include <queue>
#include <map>
#include <algorithm>

int A_Star::heuristic1(const Puzzle::Board& board) const {
    int misplaced = 0;
    
    for (std::size_t i = 0; i < board.size(); i++) {
        if (board[i] != 0 && board[i] != static_cast<int>(i)) {
            misplaced++;
        }
    }
    return misplaced;
}

int A_Star::heuristic2(const Puzzle::Board& board) const {
    int distance = 0;

    for (std::size_t i = 0; i < board.size(); i++) {
        int tile = board[i];

        if (tile == 0) {
            continue;
        }

        int currentIndex = static_cast<int>(i);

        int currentRow = currentIndex / 3;
        int currentCol = currentIndex % 3;

        // Goal is {0, 1, 2, 3, 4, 5, 6, 7, 8}
        int targetRow = tile / 3;
        int targetCol = tile % 3;

        distance += std::abs(currentRow - targetRow) + std::abs(currentCol - targetCol);
    }

    return distance;
}

int A_Star::calculateHeuristic(const Puzzle::Board& board, Heuristic heuristic) const {
    switch (heuristic) {
        case Heuristic::MISPLACED_TILES:
            return heuristic1(board);

        case Heuristic::MANHATTAN_DISTANCE:
            return heuristic2(board);
    }

    return 0;
}

std::vector<A_Star::Node> A_Star::findPath(const Puzzle& start, Heuristic heuristic) {
    if (!start.isSolvable()) {
        return {};
    }

    std::vector<Node> nodes;
    searchCost = 0;

    using QueueEntry = std::pair<int, int>;

    std::priority_queue<
        QueueEntry,
        std::vector<QueueEntry>,
        std::greater<QueueEntry>
    > frontier;

    std::map<Puzzle::Board, int> bestG;

    Node startNode {
        start,
        0,
        calculateHeuristic(start.getBoard(), heuristic),
        -1
    };

    nodes.push_back(startNode);
    searchCost++;
    frontier.push({startNode.getF(), 0});
    bestG[start.getBoard()] = 0;

    while (!frontier.empty()) {
        int currentIndex = frontier.top().second;
        frontier.pop();

        Node currentNode = nodes[currentIndex];

        if (currentNode.g != bestG[currentNode.puzzle.getBoard()]) {
            continue;
        }

        if (currentNode.puzzle.isGoalState()) {
            return reconstructPath(nodes, currentIndex);
        }

        for (const auto& neighbor :
             currentNode.puzzle.getNeighbors()) {
            const Puzzle& neighborPuzzle = neighbor.first;

            int newG = currentNode.g + 1;

            auto existing =
                bestG.find(neighborPuzzle.getBoard());

            if (existing == bestG.end() ||
                newG < existing->second) {
                int newH = calculateHeuristic(
                    neighborPuzzle.getBoard(),
                    heuristic
                );

                Node childNode {
                    neighborPuzzle,
                    newG,
                    newH,
                    currentIndex
                };

                nodes.push_back(childNode);
                searchCost++;

                int childIndex =
                    static_cast<int>(nodes.size()) - 1;

                bestG[neighborPuzzle.getBoard()] = newG;

                frontier.push({
                    childNode.getF(),
                    childIndex
                });
            }
        }
    }
    return {};
}

std::vector<A_Star::Node> A_Star::reconstructPath(const std::vector<Node>& nodes, int goalIndex) const {
    std::vector<Node> path;
    int currentIndex = goalIndex;

    while (currentIndex != -1) {
        path.push_back(nodes[currentIndex]);
        currentIndex = nodes[currentIndex].parentIndex;
    }
    
    std::reverse(path.begin(), path.end());

    return path;
}