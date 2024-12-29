#include "DFS.h"
#include <vector>
#include <stack>
#include <utility>  // For std::pair
#include <SFML/Graphics.hpp> // for SFML
#include <iostream>

using namespace sf;
using namespace std;

// Internal helper function that does the real work
static bool DFSUtil(int x, int y, RenderWindow& window, int row, int column, char** contentsArray, vector<vector<bool>>& visited) {
    // Direction vectors for moving up, down, left, and right
    int dirX[4] = {0, -1, 0, 1};
    int dirY[4] = {-1, 0, 1, 0};

    // Stack to hold nodes to visit
    stack<pair<int, int>> stk;
    stk.push({x, y});
    visited[x][y] = true;

    while (!stk.empty()) {
        // Process window events to keep the window responsive
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        auto [cx, cy] = stk.top();
        stk.pop();

        // Clear the window before each new frame
        window.clear();
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < column; j++) {
                RectangleShape tile(Vector2f(15, 15));
                tile.setPosition(j * 15, i * 15);

                // Determine the color of each tile based on state
                if (contentsArray[i][j] == '#') {
                    tile.setFillColor(Color::Black);
                } else if (i == x && j == y) {
                    tile.setFillColor(Color::Green); // Start position
                } else if (contentsArray[i][j] == 'E') {
                    tile.setFillColor(Color::Red); // End position, ensure it stays red
                } else if (visited[i][j]) {
                    tile.setFillColor(Color::Magenta);
                } else {
                    tile.setFillColor(Color::White);
                }
                window.draw(tile);
            }
        }
        window.display();
        sleep(milliseconds(100)); // Slow down the animation

        if (contentsArray[cx][cy] == 'E') {
            std::cout << "Maze completed successfully!" << std::endl;
            return true; // Reached the end
        }

        // Explore neighbors
        for (int i = 3; i >= 0; i--) {  // Reverse the order to maintain traditional DFS order
            int nx = cx + dirX[i];
            int ny = cy + dirY[i];

            if (nx >= 0 && nx < row && ny >= 0 && ny < column && contentsArray[nx][ny] != '#' && !visited[nx][ny]) {
                visited[nx][ny] = true;
                stk.push({cx, cy});  // Push current node back to stack before neighbors to return to it
                stk.push({nx, ny});  // Push neighbor
                break;  // Break to simulate depth-first exploration
            }
        }
    }

    std::cout << "Failed to complete the maze." << std::endl;
    return false; // No more nodes to explore, and end not reached
}

// Public API function
void DFS(RenderWindow& window, int(&StartingPOS)[2], int row, int column, char** contentsArray) {
    vector<vector<bool>> visited(row, vector<bool>(column, false));
    if (!DFSUtil(StartingPOS[0], StartingPOS[1], window, row, column, contentsArray, visited)) {
        std::cout << "No solution found." << std::endl; // Additional message if DFSUtil returns false
    }
}
