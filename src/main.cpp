#include "../include/Puzzle.hpp"
#include "../include/A_Star.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <iomanip>
#include <chrono>

int main() {
    int iterations = 3;

    while (iterations--) {
        std::cout << "Depth ";
        if (iterations == 2) {
            std::cout << "< 6" << std::endl;
        } else if (iterations == 1) {
            std::cout << "9-15" << std::endl;
        } else {
            std::cout << "> 18" << std::endl;
        }

        int choice = 0;
        while (choice != 1 && choice != 2) {
            std::cout << "[1] Random" << std::endl << "[2] Manual Input" << std::endl;
            std::cin >> choice;   
            if (choice != 1 && choice != 2) {
                std::cout << "ERROR: Invalid choice." << std::endl;
            }
        }

        Puzzle::Board board{};

        if (choice == 1) {
            std::string filename;

            if (iterations == 2) {
                filename = "inputs/Length4.txt";
            } else if (iterations == 1) {
                filename = "inputs/Length12.txt";
            } else {
                filename = "inputs/Length20.txt";
            }

            std::ifstream inputFile(filename);

            if (!inputFile.is_open()) {
                std::cerr << "Error: Could not open "
                          << filename << '\n';
                return 1;
            }

            std::vector<Puzzle::Board> availableBoards;
            std::vector<int> values;
            std::string line;

            while (std::getline(inputFile, line)) {
                std::stringstream lineStream(line);
                int tile;

                // Separator lines contain no integers and are skipped.
                while (lineStream >> tile) {
                    values.push_back(tile);

                    if (values.size() == 9) {
                        Puzzle::Board nextBoard{};

                        for (std::size_t i = 0;
                             i < nextBoard.size();
                             i++) {
                            nextBoard[i] = values[i];
                        }

                        availableBoards.push_back(nextBoard);
                        values.clear();
                    }
                }
            }

            std::random_device randomDevice;
            std::mt19937 generator(randomDevice());

            std::uniform_int_distribution<std::size_t> distribution(
                0,
                availableBoards.size() - 1
            );

            board = availableBoards[distribution(generator)];
            std::cout << "Puzzle: " << std::endl;
            static_cast<Puzzle>(board).printPuzzle(); // not sure
        } else {
            std::cout << "Please enter your puzzle:" << std::endl;
            for (std::size_t i = 0; i < board.size(); i++) {
                std::cin >> board[i];
            }
        }

        Puzzle startPuzzle(board);

        int h_choice = -1;
        A_Star::Heuristic selectedHeuristic;
        while (h_choice != 1 && h_choice != 2) {
            std::cout << "Select H Function:" << std::endl << "[1] H1 (Misplaced Tiles)" << std::endl << "[2] H2 (Manhattan Distance)" << std::endl;
            std::cin >> h_choice;
            if (h_choice != 1 && h_choice != 2) {
                std::cout << "ERROR: Invalid choice." << std::endl;
            }
        }

        if (h_choice == 1) {
            selectedHeuristic = A_Star::Heuristic::MISPLACED_TILES;
        } else {
            selectedHeuristic = A_Star::Heuristic::MANHATTAN_DISTANCE;
        }

        A_Star solver;
        auto startTime = std::chrono::steady_clock::now();
        std::vector<A_Star::Node> path = solver.findPath(
            startPuzzle,
            selectedHeuristic
        );
        auto endTime = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> duration = endTime - startTime;

        if (path.empty()) {
            std::cout << "ERROR: No solution was found." << std::endl;
            iterations++;
            continue;
        }

        for (std::size_t step = 1; step < path.size(); step++) {
            std::cout << "Step: " << step;
            path[step].puzzle.printPuzzle();
        }

        std::cout << "Search Cost: " << solver.getSeachCost() << std::endl;
        std::cout << std::fixed << std::setprecision(6) << "Time: " << duration.count() << " ms" << std::endl << std::endl;
    }
}